// export interface Inventory {
//     assets?:               Asset[];
//     descriptions?:         InventoryDescription[];
//     total_inventory_count: number;
//     success:               number;
//     rwgrsn:                number;
// }

// export interface Asset {
//     appid:      number;
//     contextid:  string;
//     assetid:    string;
//     classid:    string;
//     instanceid: string;
//     amount:     string;
// }

// export interface InventoryDescription {
//     appid:                         number;
//     classid:                       string;
//     instanceid:                    string;
//     currency:                      number;
//     background_color:              string;
//     icon_url:                      string;
//     icon_url_large:                string;
//     descriptions?:                 DescriptionDescription[];
//     tradable:                      number;
//     actions:                       Action[];
//     name:                          string;
//     name_color:                    string;
//     type:                          string;
//     market_name:                   string;
//     market_hash_name:              string;
//     commodity:                     number;
//     market_tradable_restriction:   number;
//     market_marketable_restriction: number;
//     marketable:                    number;
//     tags:                          Tag[];
//     market_actions?:               Action[];
//     fraudwarnings?:                string[];
// }

// export interface Action {
//     link: string;
//     name: string;
// }

// export interface DescriptionDescription {
//     value:  string;
//     color?: string;
// }

// export interface Tag {
//     category:                string;
//     internal_name:           string;
//     localized_category_name: string;
//     localized_tag_name:      string;
//     color?:                  string;
// }
// data is received from the API in the form of a JSON object

class InventoryJson {
  late List<AssetJson>? assets;
  late List<InventoryDescriptionJson>? descriptions;
  late int totalInventoryCount;
  late int success;
  late int rwgrsn;

  InventoryJson(Map<String, dynamic> data) {
    assets = data['assets'] == null ? null : List<AssetJson>.from(data['assets'].map((x) => AssetJson(x)));
    descriptions = data['descriptions'] == null ? null : List<InventoryDescriptionJson>.from(data['descriptions'].map((x) => InventoryDescriptionJson(x)));
    totalInventoryCount = data['total_inventory_count'];
    success = data['success'];
    rwgrsn = data['rwgrsn'];
  }
}

class AssetJson {
  late int appid;
  late String contextid;
  late String assetid;
  late String classid;
  late String instanceid;
  late String amount;

  AssetJson(Map<String, dynamic> data) {
    appid = data['appid'];
    contextid = data['contextid'];
    assetid = data['assetid'];
    classid = data['classid'];
    instanceid = data['instanceid'];
    amount = data['amount'];
  }
}

class InventoryDescriptionJson {
  late int appid;
  late String classid;
  late String instanceid;
  late int currency;
  late String backgroundColor;
  late String iconUrl;
  late String iconUrlLarge;
  late List<DescriptionDescriptionJson>? descriptions;
  late int tradable;
  late List<ActionJson> actions;
  late String name;
  late String nameColor;
  late String type;
  late String marketName;
  late String marketHashName;
  late int commodity;
  late int marketTradableRestriction;
  late int marketMarketableRestriction;
  late int marketable;
  late List<TagJson> tags;
  late List<ActionJson>? marketActions;
  late List<String>? fraudwarnings;

  InventoryDescriptionJson(Map<String, dynamic> data) {
    appid = data['appid'];
    classid = data['classid'];
    instanceid = data['instanceid'];
    currency = data['currency'];
    backgroundColor = data['background_color'];
    iconUrl = data['icon_url'];
    iconUrlLarge = data['icon_url_large'];
    descriptions =
        data['descriptions'] == null ? null : new List<DescriptionDescriptionJson>.from(data['descriptions'].map((x) => new DescriptionDescriptionJson(x)));
    tradable = data['tradable'];
    actions = new List<ActionJson>.from(data['actions'].map((x) => new ActionJson(x)));
    name = data['name'];
    nameColor = data['name_color'];
    type = data['type'];
    marketName = data['market_name'];
    marketHashName = data['market_hash_name'];
    commodity = data['commodity'];
    marketTradableRestriction = data['market_tradable_restriction'];
    marketMarketableRestriction = data['market_marketable_restriction'];
    marketable = data['marketable'];
    tags = new List<TagJson>.from(data['tags'].map((x) => new TagJson(x)));
    marketActions = data['market_actions'] == null ? null : new List<ActionJson>.from(data['market_actions'].map((x) => new ActionJson(x)));
    fraudwarnings = data['fraudwarnings'] == null ? null : new List<String>.from(data['fraudwarnings'].map((x) => x as String));
  }
}

class ActionJson {
  late String link;
  late String name;

  ActionJson(Map<String, dynamic> data) {
    link = data['link'];
    name = data['name'];
  }
}

class DescriptionDescriptionJson {
  late String value;
  late String? color;

  DescriptionDescriptionJson(Map<String, dynamic> data) {
    value = data['value'];
    color = data['color'];
  }
}

class TagJson {
  late String category;
  late String internalName;
  late String localizedCategoryName;
  late String localizedTagName;
  late String? color;

  TagJson(Map<String, dynamic> data) {
    category = data['category'];
    internalName = data['internal_name'];
    localizedCategoryName = data['localized_category_name'];
    localizedTagName = data['localized_tag_name'];
    color = data['color'];
  }
}
