import 'dart:convert';
import 'dart:developer';

import 'package:bpscanner/globals.dart';
import 'package:bpscanner/user.dart';
import 'package:flutter/material.dart';
import 'utilities.dart';
import 'package:bpscanner/types/GetPlayerSummaries.dart';

class Scanner {
  bool isScanning = false;
  bool stopScanning = false;

  stopScan() {
    stopScanning = true;
    States.setState();
  }

  scan(BuildContext context) async {
    if (this.isScanning) {
      return;
    }
    this.isScanning = true;
    Controllers.scanStatus.status = "Starting scan";
    States.setState();
    List<String> ids = await this._getIds();

    if (ids.length == 0) {
      Scaffold.of(context).openEndDrawer();
      this.isScanning = false;
      this.stopScanning = false;
      States.setState();
      return;
    }

    print(ids.length);

    for (var i = 0; i < ids.length; i += 100) {
      if (this.stopScanning) {
        break;
      }
      // Split ids into subarrays of 100
      List<String> subList =
          ids.sublist(i, i + 100 <= ids.length ? i + 100 : ids.length);

      Controllers.scanStatus.status =
          "Getting player summaries $i/${ids.length}";
      Controllers.scanStatus.redraw();

      var usersRequest = await fetch(
        url:
            "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=${App.config.apiKey}&format=json&steamids=${subList.join(",")}",
      );
      if (!usersRequest.success) continue;

      List<User> users = List<User>.from(
        jsonDecode(usersRequest.body)["response"]["players"].map(
          (e) => User(PlayerSummaries(e)),
        ),
      );

      for (var j = 0; j < users.length; j++) {
        var user = users[j];
        if (this.stopScanning) {
          break;
        }
        Controllers.scanStatus.status =
            "Getting inventory ${i + j}/${ids.length}";
        Controllers.scanStatus.redraw();

        await user.GetInventory();
      }

      // debugger();
      // print("L");
    }
    this.isScanning = false;
    this.stopScanning = false;
    States.setState();
  }

  Future<List<String>> _getIds() async {
    int scanOption = Controllers.scanMode.option;
    String input = Controllers.scanSettingsInput.controller.text;
    if (scanOption == 0) {
      // Scan by ID
      List<String> ids64 = regExpAll(RegExp(r'7656119[0-9]{10}'), input);
      List<String> ids3 = regExpAll(RegExp(r'\[U:1:[0-9]*\]'), input);
      ids3.forEach((element) => ids64.add(this.sid3to64(element)));

      return ids64;
    } else if (scanOption == 1) {
      // Group scan
      int totalPages = 1;
      int currentPage = 0;
      int fails = 0;

      List<String> returnIds = [];

      while (currentPage < totalPages) {
        if (stopScanning) {
          break;
        }

        Controllers.scanStatus.status =
            "Getting group members for page $currentPage/$totalPages";
        Controllers.scanStatus.redraw();

        Uri url = Uri.parse(input);
        url = url.replace(
          path: (url.path + "/memberslistxml"),
          query: "xml=1&p=$currentPage",
        );
        FetchResponse groupRequest = await fetch(url: url.toString());

        if (groupRequest.request.headers["content-type"] !=
            "text/xml;charset=UTF-8") {
          debugger();
          await Future.delayed(Duration(seconds: 5));
          fails++;
          if (fails > 5) break;
          continue;
        }

        RegExp totalPagesExp =
            RegExp("(?<=totalPages\>)[0-9]*(?=\<\/totalPages\>)");
        totalPages =
            int.parse(totalPagesExp.stringMatch(groupRequest.body) ?? "0");
        // Get max of 50 pages since some groups have thousands of pages
        totalPages = totalPages > 50 ? 50 : totalPages;

        List<String> ids =
            regExpAll(RegExp(r'7656119[0-9]{10}'), groupRequest.body);
        ids.forEach((e) => returnIds.add(e));

        debugger(when: totalPages == 0);
        currentPage++;
      }

      return returnIds;
    } else if (scanOption == 2) {
      // Profile scan
      String steamid;
      Uri url = Uri.parse(input);
      if (url.path.contains("/profiles/")) {
        // Steamid url
        steamid = url.path.replaceAll("/profiles/", "");
        if (steamid.endsWith("/"))
          steamid = steamid.substring(0, steamid.length - 1);
      } else {
        // Vanity url
        String? steamidFromVanity = await this.vanityUriToSteamid(url);
        if (steamidFromVanity == null) return [];
        steamid = steamidFromVanity;
      }

      List<String>? friends = await this.getFriends(steamid);
      if (friends == null) return [];

      List<String> returnIds = [];
      // TODO: remove the limitation
      for (var i = 0; i < friends.length && i < 50; i++) {
        if (stopScanning) {
          break;
        }
        // print progress to scanStatus
        Controllers.scanStatus.status =
            "Getting ids ${i + 1}/${friends.length}";
        Controllers.scanStatus.redraw();
        List<String>? friendsFriends = await this.getFriends(friends[i]);
        if (friendsFriends == null) continue;
        returnIds.addAll(friendsFriends);
      }

      // check for duplicates in returnIds
      returnIds = returnIds.toSet().toList();

      return returnIds;
    }
    return [];
  }

  Future<List<String>?> getFriends(String steamid) async {
    Uri url = Uri.parse(
        "https://api.steampowered.com/ISteamUser/GetFriendList/v1/?key=${App.config.apiKey}&format=json&steamid=$steamid&relationship=friend");
    FetchResponse friendsRequest = await fetch(url: url.toString());
    if (!friendsRequest.success) return null;

    List<String> friends = List<String>.from(
      jsonDecode(friendsRequest.body)["friendslist"]["friends"].map(
        (e) => e["steamid"],
      ),
    );

    return friends;
  }

  Future<String?> vanityUriToSteamid(Uri url) async {
    // Get a vanity url and return a steamid
    String vanity = url.path.replaceAll("/id/", "");
    if (vanity.endsWith("/")) vanity = vanity.substring(0, vanity.length - 1);
    FetchResponse response = await fetch(
      url:
          "https://api.steampowered.com/ISteamUser/ResolveVanityURL/v1/?key=${App.config.apiKey}&vanityurl=$vanity&format=json",
    );
    if (!response.success) return null;
    return jsonDecode(response.body)["response"]["steamid"];
  }

  String sid3to64(String steamid) {
    steamid = steamid.replaceAll(RegExp(r'\[|\]'), "");
    List<int> _args = steamid.split(":").map((e) => int.parse(e)).toList();
    return "7656119${(_args[2] + 7960265728).toString()}";
  }
}
