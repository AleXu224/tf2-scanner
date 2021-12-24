import 'dart:convert';
import 'dart:developer';

import 'package:bpscanner/types/GetPlayerSummaries.dart';
import 'package:bpscanner/user.dart';
import 'package:bpscanner/globals.dart';
import 'package:bpscanner/utilities.dart';
import 'types/inventory.dart';

class Inventory {
  late PlayerSummaries user;
  bool success = true;
  Inventory(PlayerSummaries user) {
    this.user = user;
  }

  getInventory() async {
    late InventoryJson inventory;
    int tryCount = 0;
    print("Getting inventory of ${user.steamid}");

    while (tryCount < 5) {
      if (App.scanner.stopScanning) {
        success = false;
        return;
      }

      await Future.delayed(
          Duration(milliseconds: RequestLimiter.nextRequestTime()));

      FetchResponse response = await fetch(
          url:
              "https://steamcommunity.com/inventory/${this.user.steamid}/440/2?l=english&count=4000");

      App.timestamps.add(DateTime.now().millisecondsSinceEpoch);

      for (int i = 0; i < App.timestamps.length; i++) {
        if (DateTime.now().millisecondsSinceEpoch - App.timestamps[i] > 60000) {
          App.timestamps.removeAt(i);
          i--;
        }
      }

      print("Requests per minute: ${App.timestamps.length}");

      if (response.status == 429) {
        // wait 5 seconds then retry
        // await Future.delayed(Duration(seconds: 5));
        print(429);
        tryCount++;
        continue;
      }

      if (response.success == false) {
        print(response.status);
        success = false;
        return;
      }

      // InventoryJson inventory = InventoryJson(jsonDecode(response.body));
      break;
    }
  }
}
