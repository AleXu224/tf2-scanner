import 'package:bpscanner/globals.dart';
import 'types/inventory.dart';

class Item {
  bool tradable = true;
  Item(InventoryDescriptionJson data) {
    if (data.tradable == 0) tradable = false;
  }
}
