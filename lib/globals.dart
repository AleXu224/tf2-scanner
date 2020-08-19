import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:flutter/material.dart';
import 'navigation.dart';
import 'widgets.dart' as w;
import 'logic.dart';
import 'locator.dart';

Color primary = Color(0xFF2D2E33);
Color primaryDark = Color(0xFF202124);
Color primaryDarkMid = Color(0xFF27282C);
Color primaryLight = Color(0xFF57585C);
Color secondary = Color(0xFF5D5DF2);
Color secondaryDark = Color(0xFF4E4ECC);
Color secondaryLight = Color(0xFF6161FF);

Color white05 = Color.fromRGBO(255, 255, 255, 0.05);
Color white10 = Color.fromRGBO(255, 255, 255, 0.1);
Color white20 = Color.fromRGBO(255, 255, 255, 0.2);
Color white50 = Color(0x80FFFFFF);
Color white60 = Color(0x99FFFFFF);
Color white80 = Color(0xCCFFFFFF);

final Map qualities = {
  0: {
    "name": "Normal",
    "color": Color(0xFFB2B2B2),
  },
  1: {
    "name": "Genuine",
    "color": Color(0xFF4D7455),
  },
  3: {
    "name": "Vintage",
    "color": Color(0xFF476291),
  },
  5: {
    "name": "Unusual",
    "color": Color(0xFF8650AC),
  },
  6: {
    "name": "Unique",
    "color": Color(0xFFFFD700),
  },
  7: {
    "name": "Community",
    "color": Color(0xFF70B04A),
  },
  8: {
    "name": "Valve",
    "color": Color(0xFFA50F79),
  },
  9: {
    "name": "Self-Made",
    "color": Color(0xFF70B04A),
  },
  11: {
    "name": "Strange",
    "color": Color(0xFFCF6A32),
  },
  13: {
    "name": "Haunted",
    "color": Color(0xFF38F3AB),
  },
  14: {
    "name": "Collector's",
    "color": Color(0xFFAA0000),
  },
  15: {
    "name": "Decorated Weapon",
    "color": Color(0xFFFAFAFA),
  },
};

final List qualitieList = [
  {
    "name": "Normal",
    "color": Color(0xFFB2B2B2),
    "index": 0,
  },
  {
    "name": "Genuine",
    "color": Color(0xFF4D7455),
    "index": 1,
  },
  {
    "name": "Vintage",
    "color": Color(0xFF476291),
    "index": 3,
  },
  {
    "name": "Unusual",
    "color": Color(0xFF8650AC),
    "index": 5,
  },
  {
    "name": "Unique",
    "color": Color(0xFFFFD700),
    "index": 6,
  },
  {
    "name": "Community",
    "color": Color(0xFF70B04A),
    "index": 7,
  },
  {
    "name": "Valve",
    "color": Color(0xFFA50F79),
    "index": 8,
  },
  {
    "name": "Self-Made",
    "color": Color(0xFF70B04A),
    "index": 9,
  },
  {
    "name": "Strange",
    "color": Color(0xFFCF6A32),
    "index": 11,
  },
  {
    "name": "Haunted",
    "color": Color(0xFF38F3AB),
    "index": 13,
  },
  {
    "name": "Collector's",
    "color": Color(0xFFAA0000),
    "index": 14,
  },
  {
    "name": "Decorated Weapon",
    "color": Color(0xFFFAFAFA),
    "index": 15,
  },
];

final NavigationService navigationService = locator<NavigationService>();

Config config;

dynamic schema;
dynamic prices;
dynamic skinsArray;
dynamic effects;

double keyPrice;

bool isScanning = false;
bool stopScan = false;

String scanStatus;

TextEditingController apiKey = new TextEditingController();

TextEditingController maxRef = new TextEditingController();
TextEditingController maxKeys = new TextEditingController();
TextEditingController minRef = new TextEditingController();
TextEditingController minKeys = new TextEditingController();
TextEditingController maxHistory = new TextEditingController();
TextEditingController maxHours = new TextEditingController();

bool untradable = false;
bool noValue = false;
bool skins = false;

bool groupScan = false;

String lastScan = "";

TextEditingController pagesScan = new TextEditingController();
TextEditingController pagesSkip = new TextEditingController();

TextEditingController inputList = new TextEditingController();

List<w.UserContainer> users = List();

Function homeState;
Function sideBarState;

Future<String> fetch(String url) async {
  var request = await HttpClient().getUrl(Uri.parse(url));
  var response = await request.close();
  var body = await response.transform(Utf8Decoder(allowMalformed: true)).join();
  return body;
}

Future<dynamic> fetchWithChecks(String url, [bool nullOnly = false]) async {
  var request = await HttpClient().getUrl(Uri.parse(url));
  var response = await request.close();

  if (response.statusCode == 403 && !nullOnly)
    return 403;
  else if (response.statusCode == 429) {
    await Future.delayed(Duration(seconds: 5));
    return null;
  } else if (response.statusCode != 200) return null;
  var body = await response.transform(Utf8Decoder(allowMalformed: true)).join();
  return body;
}
