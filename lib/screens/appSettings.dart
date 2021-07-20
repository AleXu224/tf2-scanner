import 'package:bpscanner/unfocuser.dart';
import 'package:flutter/material.dart';
import '../widgets/sidebar.dart';
import '../widgets/buttons.dart';
import 'package:bpscanner/globals.dart';
import '../widgets/textInput.dart';
import '../widgets/settingsList.dart';
import 'package:shared_preferences/shared_preferences.dart';

class AppSettings extends StatelessWidget {
  final bool showBackButton;
  AppSettings({this.showBackButton = true});
  @override
  Widget build(BuildContext context) {
    return Unfocuser(
      child: Scaffold(
        backgroundColor: ThemeColors.primary,
        body: Row(
          children: [
            if (showBackButton)
              Sidebar(
                topButtons: [
                  SideButton(
                    Icons.arrow_back,
                    "Back",
                    () {
                      Navigator.pop(context);
                    },
                  ),
                ],
              ),
            Expanded(
              child: Center(
                child: Container(
                  constraints: BoxConstraints.loose(
                    Size.fromWidth(600),
                  ),
                  child: Column(
                    children: [
                      SettingsList(
                        items: [
                          Padding(
                            padding: const EdgeInsets.only(
                              bottom: 12,
                              top: 12,
                            ),
                            child: Text(
                              "App settings",
                              style: TextStyle(
                                fontSize: 24,
                              ),
                            ),
                          ),
                          TextInput(
                            labelText: "Api key",
                            controller: Controllers.apiKey,
                          ),
                          Padding(
                            padding: const EdgeInsets.only(top: 12),
                            child: Row(
                              children: [
                                ElevatedButton(
                                  style: ButtonStyle(
                                    elevation: MaterialStateProperty.all(0),
                                    backgroundColor: MaterialStateProperty.all(
                                        ThemeColors.secondary),
                                  ),
                                  onPressed: () {
                                    if (Controllers
                                            .apiKey.controller.text.length !=
                                        32) {
                                      ScaffoldMessenger.of(context)
                                          .showSnackBar(
                                        SnackBar(
                                          content: Text(
                                            'Invalid api key',
                                            style: TextStyle(
                                              color: Colors.white70,
                                            ),
                                          ),
                                          backgroundColor:
                                              ThemeColors.primaryDark,
                                          behavior: SnackBarBehavior.floating,
                                          width: 200,
                                          elevation: 0,
                                        ),
                                      );
                                    } else {
                                      Scanner.config.saveApiKey();
                                      Navigator.pop(context);
                                    }
                                  },
                                  child: Text("SAVE"),
                                ),
                                Padding(
                                  padding: const EdgeInsets.only(left: 8),
                                  child: ElevatedButton(
                                    onPressed: () async {
                                      SharedPreferences prefs =
                                          await SharedPreferences.getInstance();
                                      prefs.clear();
                                      print("Cleared data");
                                    },
                                    child: Text("DELETE DATA"),
                                    style: ButtonStyle(
                                      elevation: MaterialStateProperty.all(0),
                                      backgroundColor:
                                          MaterialStateProperty.all(
                                        Colors.red[400],
                                      ),
                                    ),
                                  ),
                                )
                              ],
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
