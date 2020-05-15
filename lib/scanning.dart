import 'dart:convert';
import 'dart:math';
import 'dart:developer';

import 'globals.dart' as g;
import 'widgets.dart' as w;

void startScan() async {
  if (g.isScanning) {
    print("Scan scan, already scanning");
    return;
  }
  RegExp exp64 = RegExp(r'7656119[0-9]{10}');
  List ids = List();
  g.config.obtain();

  String scanningInput = "";

  if (g.config.groupScan) {
    var input = g.inputList.text;
    if (!input.contains(RegExp(r'.*\/$'))) input += "/";

    for (int i = g.config.pagesSkip + 1;
        i <= g.config.pagesScan + g.config.pagesSkip;
        i++) {
      var page = await g
          .fetchWithChecks(input + "memberslistxml?xml=1&p=" + i.toString());

      if (page == null || page == 403) {
        break;
      } else
        scanningInput += page;
    }
  } else
    scanningInput = g.inputList.text;

  exp64.allMatches(scanningInput).forEach((element) {
    ids.add(scanningInput.substring(element.start, element.end));
  });

  RegExp exp3 = RegExp(r'\[U:1:[0-9]{9}\]');
  List ids3 = List();
  exp3.allMatches(scanningInput).forEach((element) {
    ids3.add(scanningInput.substring(element.start, element.end));
  });

  for (var id in ids3) {
    List<String> _splitId = id.split(":");
    _splitId[0] = _splitId[0].replaceFirst("[", "");
    _splitId[2] = _splitId[2].replaceFirst("]", "");
    List<int> _args = List(3);
    _args[0] = 0;
    _args[1] = int.parse(_splitId[1]);
    _args[2] = int.parse(_splitId[2]);

    int y;
    double z;

    if (_args[2] % 2 == 0) {
      y = 0;
      z = _args[2] / 2;
    } else {
      y = 1;
      z = (_args[2] - 1) / 2;
    }
    String _id = "7656119${((z * 2) + 7960265728 + y).toStringAsFixed(0)}";
    ids.add(_id);
  }
  print(ids);

  if (ids.length < 1) return;

  int toScan = ids.length;
  int scanned = 0;

  g.isScanning = true;
  g.sideBarState();

  var len = ids.length;
  int size = 100;
  List<List<dynamic>> idParts = [];

  for (var i = 0; i < len; i += size) {
    var end = (i + size < len) ? i + size : len;
    idParts.add(ids.sublist(i, end));
  }

  for (var idList in idParts) {
    if (g.stopScan) break;

    String idString = "";
    for (int i = 0; i < idList.length; i++) {
      String id = idList[i];
      idString += id;
      if (i != idList.length - 1) idString += ",";
    }

    var playersData = jsonDecode(await g.fetch(
        "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=${g.config.apiKey}&format=json&steamids=$idString"));
    List<Player> players = List();
    for (var p in playersData["response"]["players"]) {
      players.add(Player(p));
    }

    for (Player player in players) {
      scanned++;
      g.scanStatus = "$scanned/$toScan";
      g.homeState();
      if (g.stopScan) break;
      if (player.visibility == 1 || player.visibility == 2) continue;
      await player.getInventory();
      if (!player.inventory.success) continue;
      if (player.inventory.scrap / 9 > g.config.maxRef && g.config.maxRef >= 0)
        continue;
      if (player.inventory.keys > g.config.maxKeys && g.config.maxKeys >= 0)
        continue;

      List<Item> displayItems = List();

      for (Item item in player.inventory.items) {
        if (item.isCurrency) continue;

        if (item.currency != null && g.config.totalMinPrice > 0) {
          if (item.currency == 1 && (item.priceValue) < g.config.totalMinPrice)
            continue;
          if (item.currency == 2 && item.priceValue < g.config.totalMinPrice)
            continue;
          if (item.currency == 3 && 1.33 < g.config.totalMinPrice) continue;
        }

        if (item.currency == 4 && !g.config.skins) continue;

        if (item.currency == null && !g.config.noValue) continue;

        if (!item.tradable && !g.config.untradable) continue;

        displayItems.add(item);
      }

      if (displayItems.length == 0)
        continue;
      else
        addPlayer(player, displayItems);
    }
  }
  g.stopScan = false;
  g.isScanning = false;
  g.sideBarState();
  print("Scanning done");
}

void addPlayer(Player player, List<Item> displayItems) async {
  Comparator<Item> priceSorter =
      (a, b) => (b.priceValue ?? 0).compareTo(a.priceValue ?? 0);

  displayItems.sort(priceSorter);

  await player.getHistory();
  await player.getHours();
  await player.getLevel();

  if (player.histories == null && g.config.maxHistory >= 0)
    return;
  else if (player.histories != null &&
      g.config.maxHistory >= 0 &&
      player.histories > g.config.maxHistory) return;

  if (player.hours == null && g.config.maxHours >= 0)
    return;
  else if (player.hours != null &&
      g.config.maxHours >= 0 &&
      player.hours > g.config.maxHours) return;

  g.users.add(w.UserContainer(player, displayItems));
  g.homeState();
}

class Player {
  String steamid;
  String name;
  String avatarUrl;
  String playerId;
  int visibility;
  int level;
  int histories;
  int hours;
  Inventory inventory;
  Player(dynamic data) {
    this.steamid = data["steamid"];
    this.name = data["personaname"];
    this.avatarUrl = data["avatarmedium"];
    this.playerId = "${data["steamid"]}_${Random().nextInt(256000)}";
    print(this.playerId);
    this.visibility = data["communityvisibilitystate"];
  }
  Future<bool> getInventory() async {
    this.inventory = Inventory();
    await this.inventory.getInventory(this.steamid);
    return true;
  }

  Future<bool> getHours() async {
    var gamesPage = await g.fetchWithChecks(
        "http://api.steampowered.com/IPlayerService/GetOwnedGames/v0001/?key=${g.config.apiKey}&steamid=$steamid&format=json&include_played_free_games=1");
    var games;
    if (gamesPage != null)
      games = jsonDecode(gamesPage);
    else
      return false;

    for (var game in games["response"]["games"] ?? []) {
      if (game["appid"] == 440) hours = (game["playtime_forever"] / 60).round();
    }

    if (hours == null)
      return false;
    else
      return true;
  }

  Future<bool> getHistory() async {
    String historyPage =
        await g.fetchWithChecks("https://backpack.tf/profiles/$steamid");
    if (historyPage == null) return false;

    if (historyPage.contains(RegExp(r'Most Recent[\S\s]*<\/select'))) {
      String match = RegExp(r'Most Recent[\S\s]*<\/select')
          .firstMatch(historyPage)
          .group(0);
      var matches = RegExp(r'value', caseSensitive: false).allMatches(match);
      histories = matches.length;
    } else
      histories = 0;

    return true;
  }

  Future<bool> getLevel() async {
    var levelPage = await g.fetchWithChecks(
        "https://api.steampowered.com/IPlayerService/GetSteamLevel/v1/?key=${g.config.apiKey}&steamid=$steamid");
    var level;
    if (levelPage != null)
      level = jsonDecode(levelPage);
    else
      return false;

    this.level = level["response"]["player_level"];
    return true;
  }
}

class Inventory {
  List<Item> items = List();
  int scrap = 0;
  int keys = 0;
  bool success = true;
  Future<void> getInventory(String steamid) async {
    print("Getting the inventory of $steamid");
    dynamic inventoryJson;
    int badTryCounter = 0;
    while (true) {
      await Future.delayed(Duration(milliseconds: 100));
      var inventoryResponse = await g.fetchWithChecks(
          "https://steamcommunity.com/inventory/$steamid/440/2?l=english&count=4000");
      if (inventoryResponse == null) {
        if (g.stopScan) {
          success = false;
          return;
        }
        badTryCounter++;
        print("bad response #$badTryCounter ($steamid)");
        continue;
      } else if (inventoryResponse == 403) {
        success = false;
        return;
      }
      inventoryJson = jsonDecode(inventoryResponse);
      break;
    }

    if (inventoryJson["success"] == null || inventoryJson["success"] != 1) {
      success = false;
      return;
    }

    List matches = List();

    for (var asset in inventoryJson["assets"] ?? []) {
      for (var description in inventoryJson["descriptions"]) {
        if (description["classid"] == asset["classid"]) {
          matches.add(description);
          break;
        }
      }
    }
    for (var i in matches) {
      Item it = Item(i);
      if (it.name == "Scrap Metal") {
        if (it.tradable) scrap++;
        it.isCurrency = true;
      }
      if (it.name == "Reclaimed Metal") {
        if (it.tradable) scrap += 3;
        it.isCurrency = true;
      }
      if (it.name == "Refined Metal") {
        if (it.tradable) scrap += 9;
        it.isCurrency = true;
      }
      if (it.name == "Mann Co. Supply Crate Key") {
        if (it.tradable) keys++;
        it.isCurrency = true;
      }
      items.add(it);
    }
    return;

    // for (var i in inventoryJson["result"]["items"]) {
    //   Item it = Item(i);
    //   if (it.name == "Scrap Metal") scrap++;
    //   if (it.name == "Reclaimed Metal") scrap += 3;
    //   if (it.name == "Refined Metal") scrap += 9;
    //   if (it.name == "Mann Co. Supply Crate Key") keys++;
    //   items.add(it);
    // }

    // return;
  }
}

class Item {
  String name;
  String nameComplete;
  int craftable = 1;
  int australium = -1;
  int killstreak = 0;
  int quality;
  int quality2;
  String qualityName;
  String quality2Name;
  String image;
  int skinId;
  int target;
  String targetName;
  bool tradable = true;
  int effect;
  String effectName;
  String effectImage;
  int crate;
  bool isCrate = false;
  int currency;
  double priceValue;
  String displayPrice;
  bool isStrangePart = false;
  bool isCurrency = false;
  Item(dynamic data) {
    if (data["tradable"] == 0) tradable = false;
    nameComplete = data["market_name"];
    name = data["market_name"];
    name = name.replaceFirst(RegExp(r'^The '), "");
    image =
        "https://steamcommunity-a.akamaihd.net/economy/image/${data["icon_url"]}";
    if (name.contains("Killstreak ")) {
      killstreak = 1;
      name = name.replaceFirst("Killstreak ", "");
    }
    if (name.contains("Specialized ")) {
      killstreak = 2;
      name = name.replaceFirst("Specialized ", "");
    }
    if (name.contains("Professional ")) {
      killstreak = 3;
      name = name.replaceFirst("Professional ", "");
    }
    if (name.contains("Australium ") && !name.contains("Gold")) {
      australium = 1;
      name = name.replaceFirst("Australium ", "");
    }

    for (var tag in data["tags"] ?? []) {
      String category = tag["category"];
      if (category == "Quality") {
        for (var qualityData in g.qualitieList) {
          if (qualityData["name"] == tag["localized_tag_name"]) {
            quality = qualityData["index"];
            qualityName = qualityData["name"];
            break;
          }
        }
      } else if (category == "Type") {
        if (tag["localized_tag_name"] == "Crate") {
          isCrate = true;
        } else if (tag["localized_tag_name"] == "Strange Part") {
          isStrangePart = true;
        }
      }
    }

    for (var description in data["descriptions"] ?? []) {
      String value = description["value"];
      if (value.contains("★ Unusual Effect: ") && !value.contains("''")) {
        // effectName = RegExp(r'(?!\s)[\w\s]*$').firstMatch(value).group(0);
        effectName = value.replaceFirst("★ Unusual Effect: ", "");
      } else if (value
          .contains("( Not Tradable, Marketable, or Usable in Crafting )")) {
        craftable = -1;
      }
    }

    if (!isStrangePart) name = name.replaceFirst("$qualityName ", "");

    for (var qualityData in g.qualitieList) {
      if (name.contains("${qualityData["name"]} ")) {
        if (qualityData["index"] == 11 && isStrangePart) continue;
        name = name.replaceFirst("${qualityData["name"]} ", "");
        quality2 = qualityData["index"];
        quality2Name = qualityData["name"];
        break;
      }
    }

    if (quality == 15) {
      for (var k in g.skinsArray.keys) {
        var v = g.skinsArray[k];
        if (name.contains(v)) {
          name = name.replaceFirst("$v ", "");
          skinId = int.parse(k);
          break;
        }
      }
    }

    if (name.contains("#")) {
      crate = int.parse(RegExp(r'\d+').firstMatch(name).group(0));
      name = name.replaceFirst(" Series #$crate", "");
      name = name.replaceFirst(" #$crate", "");
    }

    if (name.contains(" Unusualifier")) {
      targetName = RegExp(r'^[\w:\s]+(?=\s)').firstMatch(name).group(0);
      name = name.replaceFirst("$targetName ", "");
      for (var it in g.schema) {
        if (it["item_name"] == targetName) {
          target = it["defindex"];
          break;
        }
      }
    }

    if (quality == 5 && effectName != null) {
      effect = g.effects[effectName];
      debugger(when: effect == null);
      nameComplete = nameComplete.replaceFirst("Unusual", effectName);
      effectImage =
          "https://backpack.tf/images/440/particles/${effect}_94x94.png";
    }

    String _name = "${australium == 1 ? "Australium " : ""}$name";
    String _displayQuality = (quality2 ?? quality).toString();
    if (quality == 5) _displayQuality = quality.toString();
    String _craftable = "${craftable == 1 ? "Craftable" : "Non-Craftable"}";

    dynamic p1 = g.prices["response"]["items"][_name] ?? {};
    dynamic p2 = p1["prices"] ?? {};
    dynamic p3 = p2[_displayQuality] ?? {};
    dynamic p4 = p3["Tradable"] ?? {};
    dynamic priceIndex = p4[_craftable];

    if (priceIndex == null) {
      displayPrice = "No value";
      priceValue = 0;
      return;
    }

    int _serial = 0;
    if (quality == 5 || quality2 == 5) {
      if (target != null)
        _serial = target;
      else if (effect != null) _serial = effect;
    } else if (isCrate) {
      priceIndex[0] == null ? _serial = crate : _serial = 0;
    }
    dynamic prices = priceIndex[_serial] ?? priceIndex["$_serial"];
    if (prices == null) {
      if (priceIndex.keys.length == 1)
        prices = priceIndex[priceIndex.keys.first];
      else {
        displayPrice = "No value";
        priceValue = 0;
        return;
      }
    }

    if (prices["currency"] == "keys")
      currency = 1;
    else if (prices["currency"] == "metal")
      currency = 2;
    else
      currency = 3;

    if (currency == 1) priceValue = prices["value"].toDouble();
    if (currency == 2) priceValue = prices["value"] / g.keyPrice;
    if (currency == 3) priceValue = 1.33 / g.keyPrice;
    if (currency == null) priceValue = 0;

    if (currency == 1) displayPrice = "${prices["value"]} Keys";
    if (currency == 2) displayPrice = "${prices["value"]} Ref";
    if (currency == 3) displayPrice = "1.33 Ref";
    if (!tradable) {
      displayPrice = "No value";
      priceValue = 0;
    }

    if (skinId != null) currency = 4;
  }
}
