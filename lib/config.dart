import 'dart:convert';

import 'package:bpscanner/widgets/loadingCard.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'utilities.dart';
import 'globals.dart';
import 'dart:io';
import 'types/itemSchema.dart';
import 'types/Bptf.dart';
import 'types/effects.dart';
import 'types/skins.dart';

class Config {
  double maxRef = -1;
  int maxKeys = -1;
  double minRef = -1;
  double minKeys = -1;
  int maxHistory = -1;
  int maxHours = -1;

  bool untradable = false;
  bool noValue = false;
  bool skins = true;

  bool groupScan = false;
  int pagesScan = 1;
  int pagesSkip = 0;

  String apiKey = "";

  List<SchemaItem> itemSchema = [];
  late BptfPrices bptfPrices;
  late Effects effects;
  late Skins skinsData;

  init() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    maxRef = prefs.getDouble("maxRef") ?? -1;
    maxKeys = prefs.getInt("maxKeys") ?? -1;
    minRef = prefs.getDouble("minRef") ?? -1;
    minKeys = prefs.getDouble("minKeys") ?? -1;
    maxHistory = prefs.getInt("maxHistory") ?? -1;
    maxHours = prefs.getInt("maxHours") ?? -1;

    untradable = prefs.getBool("untradable") ?? false;
    noValue = prefs.getBool("noValue") ?? false;
    skins = prefs.getBool("skins") ?? true;

    groupScan = prefs.getBool("groupScan") ?? true;
    pagesScan = prefs.getInt("pagesScan") ?? 1;
    pagesSkip = prefs.getInt("pagesSkip") ?? 0;

    apiKey = prefs.getString("apiKey") ?? "";
    if (apiKey == "") {
      Scanner.popupScreen = 0;
      Scanner.showPopup = true;
      States.setState();
    } else {
      fetchRequirements();
    }
  }

  int get totalMinPrice {
    // TODO: totalMinPrice getter
    return 1;
  }

  saveApiKey() async {
    apiKey = Controllers.apiKey.controller.text;
    SharedPreferences prefs = await SharedPreferences.getInstance();
    prefs.setString("apiKey", apiKey);
    fetchRequirements();
  }

  getValues() {
    maxRef = double.tryParse(Controllers.maxRef.controller.text) ?? -1;
    maxKeys = int.tryParse(Controllers.maxKeys.controller.text) ?? -1;
    minRef = double.tryParse(Controllers.minRef.controller.text) ?? -1;
    minKeys = double.tryParse(Controllers.minKeys.controller.text) ?? -1;
    maxHistory = int.tryParse(Controllers.maxHistories.controller.text) ?? -1;
    maxHours = int.tryParse(Controllers.maxHours.controller.text) ?? -1;

    untradable = Controllers.untradable.state;
    noValue = Controllers.noValue.state;
    skins = Controllers.skins.state;
  }

  setValues() {
    Controllers.maxRef.controller.text = maxRef == -1 ? "" : maxRef.toString();
    Controllers.maxKeys.controller.text = maxKeys == -1 ? "" : maxKeys.toString();
    Controllers.minRef.controller.text = minRef == -1 ? "" : minRef.toString();
    Controllers.minKeys.controller.text = minKeys == -1 ? "" : minKeys.toString();
    Controllers.maxHistories.controller.text = maxHistory == -1 ? "" : maxHistory.toString();
    Controllers.maxHours.controller.text = maxHours == -1 ? "" : maxHours.toString();

    Controllers.untradable.state = untradable;
    Controllers.noValue.state = noValue;
    Controllers.skins.state = skins;
  }

  initializeFiles() async {
    List<dynamic> schemaFile = json.decode(await File("schema.json").readAsString());
    schemaFile.forEach(
      (element) {
        this.itemSchema.add(SchemaItem(element));
      },
    );

    Map<String, dynamic> bptfFile = json.decode(await File("bptf.json").readAsString());
    this.bptfPrices = BptfPrices(bptfFile);

    Map<String, dynamic> effectsFile = json.decode(await File("effects.json").readAsString());
    this.effects = Effects(effectsFile);

    Map<String, dynamic> skinsFile = json.decode(await File("skins.json").readAsString());
    this.skinsData = Skins(skinsFile);
  }

  fetchRequirements() async {
    // Initialize popup screen
    Scanner.showPopup = true;
    Scanner.popupScreen = 1;
    Scanner.loadingData = [];

    Controllers.schemaCard.isLoading = true;
    Scanner.loadingData.insert(
      0,
      new LoadingCard(
        controller: Controllers.schemaCard,
        name: "Getting Item Schema",
      ),
    );
    States.setState();

    File schemaFile = File("schema.json");
    bool schemaFileExists = await schemaFile.exists();
    if (!schemaFileExists) {
      var schema = jsonDecode("[]");
      int next = 0;
      while (true) {
        FetchResponse schemaJsonResponse = await fetch(
          url: "http://api.steampowered.com/IEconItems_440/GetSchemaItems/v0001/?key=${this.apiKey}&language=en_US&start=$next",
        );
        // If response code wasn't 200 then try again
        if (!schemaJsonResponse.success) {
          if (schemaJsonResponse.status == 403) {
            Scanner.popupScreen = 0;
            Scanner.showPopup = true;
            States.setState();
            showSnack(GlobalNav.navKey.currentContext!, "Api key was invalid. Make sure you typed it correctly and that there are no spaces");
            return;
          }
          await Future.delayed(Duration(seconds: 5));
          continue;
        }

        var _schemaJson = jsonDecode(schemaJsonResponse.body);
        schema.addAll(_schemaJson["result"]["items"]);

        if (_schemaJson["result"]["next"] == null) {
          break;
        } else {
          next = _schemaJson["result"]["next"];
        }
      }
      await schemaFile.writeAsString(jsonEncode(schema));
    }
    Controllers.schemaCard.isLoading = false;

    Controllers.bptfCard.isLoading = true;
    Scanner.loadingData.insert(
      0,
      new LoadingCard(
        controller: Controllers.bptfCard,
        name: "Getting Bptf Prices",
      ),
    );
    States.setState();
    File bptfFile = File("bptf.json");
    bool bptfFileExists = await bptfFile.exists();
    if (!bptfFileExists) {
      FetchResponse bptfSchemaResponse = await fetch(
        url: "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/schema_bptf.json",
      );
      if (!bptfSchemaResponse.success) {
        Future.delayed(Duration(seconds: 5));
        fetchRequirements();
        return;
      }
      var bptfSchema = jsonDecode(bptfSchemaResponse.body);
      await bptfFile.writeAsString(jsonEncode(bptfSchema));
    }
    Controllers.bptfCard.isLoading = false;

    Controllers.skinsCard.isLoading = true;
    Scanner.loadingData.insert(
      0,
      new LoadingCard(
        controller: Controllers.skinsCard,
        name: "Getting Skins Data",
      ),
    );
    States.setState();
    File skinsFile = File("skins.json");
    bool skinsFileExists = await skinsFile.exists();
    if (!skinsFileExists) {
      FetchResponse skinsPageResponse = await fetch(
        url: "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/skins.json",
      );
      if (!skinsPageResponse.success) {
        Future.delayed(Duration(seconds: 5));
        fetchRequirements();
        return;
      }
      var skinsPage = jsonDecode(skinsPageResponse.body);
      await skinsFile.writeAsString(jsonEncode(skinsPage));
    }
    Controllers.skinsCard.isLoading = false;

    Controllers.effectsCard.isLoading = true;
    Scanner.loadingData.insert(
      0,
      new LoadingCard(
        controller: Controllers.effectsCard,
        name: "Getting Effects",
      ),
    );
    States.setState();
    File effectsFile = File("effects.json");
    bool effectsFileExists = await effectsFile.exists();
    if (!effectsFileExists) {
      FetchResponse effectsJsonResponse = await fetch(
        url: "https://raw.githubusercontent.com/mninc/tf2-effects/master/effects.json",
      );
      if (!effectsJsonResponse.success) {
        Future.delayed(Duration(seconds: 5));
        fetchRequirements();
        return;
      }
      var effectsJson = jsonDecode(effectsJsonResponse.body);
      await effectsFile.writeAsString(jsonEncode(effectsJson));
    }
    Controllers.effectsCard.isLoading = false;
    Scanner.showPopup = false;
    States.setState();

    this.setValues();
    this.initializeFiles();
  }
}
