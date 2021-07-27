import 'package:bpscanner/config.dart';
import 'package:flutter/material.dart';
import 'package:bpscanner/widgets/userContainer.dart';

class ThemeColors {
  static Color primary = Color(0xFF2D2E33);
  static Color primaryDark = Color(0xFF202124);
  static Color primaryDarkMid = Color(0xFF27282C);
  static Color secondary = Color(0xFF5D5DF2);
  static Color secondaryDark = Color(0xFF4E4ECC);
  static Color secondaryLight = Color(0xFF6161FF);
  static Color textInputBackground = Color(0xFF37383D);
  static Color textInputBackgroundHover = Color(0xFF424247);

  static Color p = Color(0xFF212121);
  static Color pL = Color(0xFF2C2C2C);
  static Color pD = Color(0xFF0D0D0D);
  static Color s = Color(0xFF23C681);
  static Color t = Color(0xFFB8B8B8);
}

class GlobalNav {
  static GlobalKey<NavigatorState> navKey = GlobalKey<NavigatorState>();
}

class States {
  static Function(void Function()) scanSettingsInput = (Function function) {};
  static Function() setState = () {};
}

class Controllers {
  static TextController maxRef = new TextController();
  static TextController maxKeys = new TextController();
  static TextController minRef = new TextController();
  static TextController minKeys = new TextController();
  static TextController maxHours = new TextController();
  static TextController maxHistories = new TextController();
  static SwitchController untradable = new SwitchController();
  static SwitchController noValue = new SwitchController();
  static SwitchController skins = new SwitchController();
  static SwitchController group = new SwitchController();
  static TextController pages = new TextController();
  static TextController skip = new TextController();
  static TextController scanSettingsInput = new TextController();
  static TextController apiKey = new TextController();
  static ListController scanMode = new ListController();
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

class Scanner {
  static Config config = new Config();
  static List<UserContainer> users = [];
}
