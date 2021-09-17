import 'dart:convert';
import 'dart:developer';

import 'package:bpscanner/globals.dart';
import 'package:flutter/material.dart';
import 'utilities.dart';
import 'package:bpscanner/types/GetPlayerSummaries.dart';

class Scanner {
  bool isScanning = false;

  scan(BuildContext context) async {
    List<String> ids = await this._getIds();

    int users = ids.length;
    if (users == 0) {
      Scaffold.of(context).openEndDrawer();
      return;
    }

    for (var i = 0; i < ids.length; i += 100) {
      // Split ids into subarrays of 100
      List<String> subList = ids.sublist(i, i + 100 <= ids.length ? i + 100 : ids.length);

      var usersRequest = await fetch(
        url: "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=${App.config.apiKey}&format=json&steamids=${subList.join(",")}",
      );
      if (!usersRequest.success) continue;

      List<User> users = List<User>.from(
        jsonDecode(usersRequest.body)["response"]["players"].map(
          (e) => User(e),
        ),
      );
      debugger();
      print("L");
    }
  }

  Future<List<String>> _getIds() async {
    int scanOption = Controllers.scanMode.option;
    String input = Controllers.scanSettingsInput.controller.text;
    if (scanOption == 0) {
      List<String> ids64 = regExpAll(RegExp(r'7656119[0-9]{10}'), input);
      List<String> ids3 = regExpAll(RegExp(r'\[U:1:[0-9]*\]'), input);
      ids3.forEach((element) => ids64.add(this.sid3to64(element)));

      return ids64;
    } else if (scanOption == 1) {
      int totalPages = 1;
      int currentPage = 0;
      int fails = 0;

      List<String> returnIds = [];

      while (currentPage < totalPages) {
        Uri url = Uri.parse(input);
        url = url.replace(
          path: (url.path + "/memberslistxml"),
          query: "xml=1&p=$currentPage",
        );
        FetchResponse groupRequest = await fetch(url: url.toString());

        if (groupRequest.request.headers["content-type"] != "text/xml;charset=UTF-8") {
          debugger();
          await Future.delayed(Duration(seconds: 5));
          fails++;
          if (fails > 5) break;
          continue;
        }

        RegExp totalPagesExp = RegExp("(?<=totalPages\>)[0-9]*(?=\<\/totalPages\>)");
        totalPages = int.parse(totalPagesExp.stringMatch(groupRequest.body) ?? "0");
        // Get max of 50 pages since some groups have thousands of pages
        totalPages = totalPages > 50 ? 50 : totalPages;

        List<String> ids = regExpAll(RegExp(r'7656119[0-9]{10}'), groupRequest.body);
        ids.forEach((e) => returnIds.add(e));

        debugger(when: totalPages == 0);
        currentPage++;
      }

      return returnIds;
    } else if (scanOption == 2) {}
    return [];
  }

  String sid3to64(String steamid) {
    steamid = steamid.replaceAll(RegExp(r'\[|\]'), "");
    List<int> _args = steamid.split(":").map((e) => int.parse(e)).toList();
    return "7656119${(_args[2] + 7960265728).toString()}";
  }
}
