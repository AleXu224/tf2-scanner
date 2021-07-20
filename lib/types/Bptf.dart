// ignore_for_file: non_constant_identifier_names

class BptfPrices {
  List<Item> items = [];
  BptfPrices(Map<String, dynamic> data) {
    Map<String, dynamic> items_data = data["response"]["items"];

    // Get name
    for (String key in items_data.keys) {
      String name = key;

      // Get quality
      for (String key2 in items_data[key]["prices"].keys) {
        int quality = int.parse(key2);

        // Get tradable
        for (String key3 in items_data[key]["prices"][key2].keys) {
          bool tradable = key3 == "Tradable" ? true : false;

          // Get craftable
          for (String key4 in items_data[key]["prices"][key2][key3].keys) {
            bool craftable = key4 == "Craftable" ? true : false;

            // Items that do not have data
            if (items_data[key]["prices"][key2][key3][key4] is List<dynamic>) {
              items.add(new Item(
                name: name,
                quality: quality,
                tradable: tradable,
                craftable: craftable,
                item_data: items_data[key]["prices"][key2][key3][key4][0],
              ));
              continue;
            }

            // Get unusual effect or crate number
            for (String key5 in items_data[key]["prices"][key2][key3][key4].keys) {
              int? data_id = int.tryParse(key5);
              items.add(new Item(
                name: name,
                quality: quality,
                tradable: tradable,
                craftable: craftable,
                data_id: data_id,
                item_data: items_data[key]["prices"][key2][key3][key4][key5],
              ));
            }
          }
        }
      }
    }
  }
}

class Item {
  String name;
  int quality;
  bool tradable;
  bool craftable;
  int? data_id;
  double? value;
  String? currency;
  Item({
    required this.name,
    required this.quality,
    required this.tradable,
    required this.craftable,
    this.data_id,
    required Map<String, dynamic> item_data,
  }) {
    value = double.tryParse(item_data["value"].toString());
    currency = item_data["currency"];
  }
}
