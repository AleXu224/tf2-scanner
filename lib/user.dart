import 'package:bpscanner/inventory.dart';
import 'package:bpscanner/types/GetPlayerSummaries.dart';

import 'types/GetPlayerSummaries.dart';
import 'package:bpscanner/inventory.dart';

class User {
  late PlayerSummaries playerSummaries;
  late Inventory inventory;

  User(PlayerSummaries playerSummaries) {
    this.playerSummaries = playerSummaries;
  }

  GetInventory() async {
    inventory = new Inventory(this.playerSummaries);
    await inventory.getInventory();
  }
}
