import 'dart:convert';
import 'dart:io';
import 'package:flutter/material.dart';
import 'widgets.dart' as w;

import 'globals.dart' as g;

class Config {
  String apiKey;
  int lastUpdate;

  double maxRef = -1;
  int maxKeys = -1;
  double minRef = -1;
  int minKeys = -1;
  int maxHistory = -1;
  int maxHours = -1;

  bool untradable = false;
  bool noValue = false;
  bool skins = true;

  bool groupScan = false;
  int pagesScan = 1;
  int pagesSkip = 0;

  double totalMinPrice;

  initialize() async {
    File configFile = File("config.json");
    bool exists = await configFile.exists();

    if (!exists) {
      g.navigationService.navigateTo("/settings", arguments: false);
      return;
    }

    String configContent = await configFile.readAsString();
    Map config = jsonDecode(configContent);

    this.lastUpdate = config["lastUpdate"];
    this.maxRef = config["maxRef"];
    this.maxKeys = config["maxKeys"];
    this.minRef = config["minRef"];
    this.minKeys = config["minKeys"];
    this.maxHistory = config["maxHistory"];
    this.untradable = config["untradable"];
    this.noValue = config["noValue"];
    this.skins = config["skins"];
    this.groupScan = config["groupScan"];
    this.pagesScan = config["pagesScan"];
    this.pagesSkip = config["pagesSkip"];

    if (config["apiKey"] == null || !validateApiKey(config["apiKey"])) {
      g.navigationService.navigateTo("/settings", arguments: false);
      print("api key is not valid");
      return false;
    }

    this.apiKey = config["apiKey"];
    fetchRequirements();
  }

  bool validateApiKey(String key) {
    if (key.length != 32)
      return false;
    else
      return true;
  }

  setApiKey(String newKey) {
    if (!validateApiKey(newKey)) return false;
    this.apiKey = newKey;
    store();
    apply();
    g.navigationService.goBack();
    fetchRequirements();
  }

  Map toJson() {
    Map config = Map();
    config["apiKey"] = this.apiKey;
    config["lastUpdate"] = this.lastUpdate;
    config["maxRef"] = this.maxRef;
    config["maxKeys"] = this.maxKeys;
    config["minRef"] = this.minRef;
    config["minKeys"] = this.minKeys;
    config["maxHistory"] = this.maxHistory;
    config["untradable"] = this.untradable;
    config["noValue"] = this.noValue;
    config["skins"] = this.skins;
    config["groupScan"] = this.groupScan;
    config["pagesScan"] = this.pagesScan;
    config["pagesSkip"] = this.pagesSkip;
    return config;
  }

  store() async {
    File configFile = File("config.json");
    await configFile.writeAsString(jsonEncode(this.toJson()));
    return;
  }

  obtain() {
    this.untradable = g.untradable;
    this.noValue = g.noValue;
    this.skins = g.skins;
    this.groupScan = g.groupScan;

    this.maxRef = double.tryParse(g.maxRef.text) ?? -1;
    this.maxKeys = int.tryParse(g.maxKeys.text) ?? -1;
    this.minRef = double.tryParse(g.minRef.text) ?? -1;
    this.minKeys = int.tryParse(g.minKeys.text) ?? -1;
    this.maxHours = int.tryParse(g.maxHours.text) ?? -1;
    this.maxHistory = int.tryParse(g.maxHistory.text) ?? -1;
    this.pagesScan = int.tryParse(g.pagesScan.text) ?? 1;
    this.pagesSkip = int.tryParse(g.pagesSkip.text) ?? 0;

    this.totalMinPrice = (this.minKeys >= 0 ? this.minKeys : 0) +
        (this.minRef >= 0 ? this.minRef : 0) / g.keyPrice;
  }

  apply() {
    g.apiKey.text = this.apiKey.toString();

    g.maxRef.text = this.maxRef < 0 ? "" : this.maxRef.toString();
    g.maxKeys.text = this.maxKeys < 0 ? "" : this.maxKeys.toString();
    g.minRef.text = this.minRef < 0 ? "" : this.minRef.toString();
    g.minKeys.text = this.minKeys < 0 ? "" : this.minKeys.toString();
    g.maxHistory.text = this.maxHistory < 0 ? "" : this.maxHistory.toString();
    g.maxHours.text = this.maxHours < 0 ? "" : this.maxHours.toString();

    g.untradable = this.untradable;
    g.noValue = this.noValue;
    g.skins = this.skins;

    g.groupScan = this.groupScan;
    g.pagesScan.text = this.pagesScan == 1 ? "" : this.pagesScan.toString();
    g.pagesSkip.text = this.pagesSkip == 0 ? "" : this.pagesSkip.toString();
  }
}

void fetchRequirements() async {
  g.navigationService.navigateTo("/splash");

  File schemaFile = File("schema.json");
  bool schemaFileExists = await schemaFile.exists();
  if (!schemaFileExists) {
    var schema = jsonDecode("[]");
    int next = 0;
    while (true) {
      var _schemaJson = jsonDecode(await g.fetch(
          "http://api.steampowered.com/IEconItems_440/GetSchemaItems/v0001/?key=${g.config.apiKey}&language=en_US&start=$next"));
      schema.addAll(_schemaJson["result"]["items"]);

      if (_schemaJson["result"]["next"] == null) {
        break;
      } else {
        next = _schemaJson["result"]["next"];
      }
    }

    schemaFile.writeAsString(jsonEncode(schema));
  }

  File bptfFile = File("bptf.json");
  bool bptfFileExists = await bptfFile.exists();
  if (!bptfFileExists) {
    var bptfSchema = jsonDecode(await g.fetch(
        "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/schema_bptf.json"));
    bptfFile.writeAsString(jsonEncode(bptfSchema));
  }

  File skinsFile = File("skins.json");
  bool skinsFileExists = await skinsFile.exists();
  if (!skinsFileExists) {
    var skinsPage = jsonDecode(await g.fetch(
        "https://raw.githubusercontent.com/AleXu224/bptf_pricelist/master/skins.json"));
    skinsFile.writeAsString(jsonEncode(skinsPage));
  }

  File effectsFile = File("effects.json");
  bool effectsFileExists = await effectsFile.exists();
  if (!effectsFileExists) {
    var effectsJson = jsonDecode(await g.fetch(
        "https://raw.githubusercontent.com/mninc/tf2-effects/master/effects.json"));
    effectsFile.writeAsString(jsonEncode(effectsJson));
  }

  g.navigationService.goBack();
  initializeGlobalVariables();
}

void initializeGlobalVariables() async {
  File schemaFile = File("schema.json");
  g.schema = jsonDecode(await schemaFile.readAsString());

  File bptfFile = File("bptf.json");
  g.prices = jsonDecode(await bptfFile.readAsString());

  File skinsFile = File("skins.json");
  g.skinsArray = jsonDecode(await skinsFile.readAsString());

  g.keyPrice = g.prices["response"]["items"]["Mann Co. Supply Crate Key"]
      ["prices"]["6"]["Tradable"]["Craftable"][0]["value"];

  File effectsFile = File("effects.json");
  g.effects = jsonDecode(await effectsFile.readAsString());

  g.config.apply();
}

class SplashScreen extends StatefulWidget {
  @override
  _SplashScreenState createState() => _SplashScreenState();
}

class _SplashScreenState extends State<SplashScreen> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: g.primaryDark,
      body: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              w.titleText("Configuring app..."),
            ],
          ),
          Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              w.subtitleText("This is gonna take a few moments..."),
            ],
          )
        ],
      ),
    );
  }
}
