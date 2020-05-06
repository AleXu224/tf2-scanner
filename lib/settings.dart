import 'package:flutter/material.dart';
import 'globals.dart' as g;
import 'widgets.dart' as w;

class Settings extends StatefulWidget {
  final bool displayBack;
  Settings(this.displayBack);
  @override
  _SettingsState createState() => _SettingsState();
}

class _SettingsState extends State<Settings> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: g.primaryDark,
      body: GestureDetector(
        onTap: () {
          FocusScopeNode currentFocus = FocusScope.of(context);
          if (!currentFocus.hasPrimaryFocus) {
            currentFocus.unfocus();
          }
        },
        child: Row(
          children: [
            w.settingsSideBar(widget.displayBack),
            w.optionList(
                child: Column(
              children: [
                w.titleText("Settings"),
                w.subtitleText("All fields are required"),
                w.ValueInput(
                  text: "Steam api key",
                  textController: g.apiKey,
                  isLarge: true,
                ),
                Container(
                  margin: EdgeInsets.only(top: 10),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.end,
                    children: [
                      w.ActionButton(
                        "Save",
                        () {
                          g.config.setApiKey(g.apiKey.text);
                        },
                      ),
                    ],
                  ),
                ),
              ],
            ))
          ],
        ),
      ),
    );
  }
}
