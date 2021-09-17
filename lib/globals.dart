import 'package:bpscanner/config.dart';
import 'package:bpscanner/scanner.dart';
import 'package:flutter/material.dart';
import 'package:bpscanner/widgets/userContainer.dart';

class ThemeColors {
  static Color p = Color(0xFF212121);
  static Color pL = Color(0xFF2C2C2C);
  static Color pD = Color(0xFF0D0D0D);
  static Color s = Color(0xFF23C681);
  static Color t = Color(0xFFB8B8B8);
  static Color tL = Color(0xFFFFFFFF);
  static Color e = Color(0xFFB22D2D);
  static Color eL = Color(0xFFE63939);
}

class GlobalNav {
  static GlobalKey<NavigatorState> navKey = GlobalKey<NavigatorState>();
}

class States {
  static Function(void Function()) scanSettingsInput = (Function function) {};
  static Function() setState = () {};
}

class Controllers {
  static ListController scanMode = new ListController();
  static TextController maxRef = new TextController();
  static TextController maxKeys = new TextController();
  static TextController minRef = new TextController();
  static TextController minKeys = new TextController();
  static TextController maxHours = new TextController();
  static TextController maxHistories = new TextController();
  static SwitchController untradable = new SwitchController();
  static SwitchController noValue = new SwitchController();
  static SwitchController skins = new SwitchController();
  static TextController scanSettingsInput = new TextController();
  static TextController apiKey = new TextController();

  static LoadingCardController schemaCard = new LoadingCardController();
  static LoadingCardController bptfCard = new LoadingCardController();
  static LoadingCardController skinsCard = new LoadingCardController();
  static LoadingCardController effectsCard = new LoadingCardController();
}

class ListController {
  int option = 0;
}

class SwitchController {
  bool state = false;
}

class TextController {
  TextEditingController controller = new TextEditingController();
}

class LoadingCardController {
  bool isLoading = true;
}

class App {
  static Config config = new Config();
  static List<UserContainer> users = [];
  static bool showPopup = false;
  static int popupScreen = 0;
  static List<Widget> loadingData = [];
  static Scanner scanner = new Scanner();
}
