// ignore_for_file: non_constant_identifier_names

class SchemaItem {
  late String name;
  late int defindex;
  late String item_class;
  late String item_type_name;
  late String item_name;
  late bool proper_name;
  late String? item_slot;
  late String? model_player;
  late int item_quality;
  late String? image_inventory;
  late int min_ilevel;
  late int max_ilevel;
  late String? image_url;
  late String? image_url_large;
  late String? craft_class;
  late String? craft_material_type;
  late Map<String, bool> capabilities;
  late List<String>? used_by_classes;
  late String? item_description;
  late List<Attribute>? attributes;
  late String? drop_type;
  late String? item_set;
  late String? holiday_restriction;
  late Tool? tool;

  // Todo: make the contructor
  SchemaItem(Map<String, dynamic> data) {
    name = data["name"];
    defindex = data["defindex"];
    item_class = data["item_class"];
    item_type_name = data["item_type_name"];
    item_name = data["item_name"];
    proper_name = data["proper_name"];
    item_slot = data["item_slot"];
    model_player = data["model_player"];
    item_quality = data["item_quality"];
    image_inventory = data["image_inventory"];
    min_ilevel = data["min_ilevel"];
    max_ilevel = data["max_ilevel"];
    image_url = data["image_url"];
    image_url_large = data["image_url_large"];
    craft_class = data["craft_class"];
    craft_material_type = data["craft_material_type"];
    capabilities = Map<String, bool>.from(data["capabilities"]);
    used_by_classes = data["used_by_classes"] == null ? null : new List<String>.from(data["used_by_classes"]);
    item_description = data["item_description"];
    attributes = data["attributes"] == null ? null : new List<Attribute>.from(data["attributes"].map((e) => Attribute(e)));
    drop_type = data["drop_type"];
    item_set = data["item_set"];
    holiday_restriction = data["holiday_restriction"];
    tool = data["tool"] == null ? null : Tool(data["tool"]);
  }
}

class Attribute {
  late String name;
  late String a_class; // class
  late double value;

  Attribute(Map<String, dynamic> data) {
    name = data["name"];
    a_class = data["class"];
    value = double.parse(data["value"].toString());
  }
}

class Tool {
  late String type;
  late Map<String, bool>? usage_capabilities;
  late String? use_string;
  late String? restriction;

  Tool(Map<String, dynamic> data) {
    type = data["type"];
    usage_capabilities = data["usage_capabilities"] == null ? null : Map<String, bool>.from(data["usage_capabilities"]);
    use_string = data["use_string"];
    restriction = data["restriction"];
  }
}
