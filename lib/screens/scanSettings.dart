import 'package:bpscanner/widgets/scanSettingsInput.dart';
import 'package:bpscanner/widgets/switchInput.dart';
import 'package:bpscanner/widgets/textInput.dart';
import 'package:flutter/material.dart';
import '../widgets/sidebar.dart';
import '../widgets/buttons.dart';
import 'package:bpscanner/globals.dart';
import '../widgets/settingsList.dart';
import 'package:bpscanner/unfocuser.dart';
import '../widgets/delegateWithFixedHeight.dart';

class ScanSettings extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Unfocuser(
      child: Scaffold(
        backgroundColor: ThemeColors.primary,
        body: Row(
          children: [
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
              child: Scrollbar(
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
                            "Scan settings",
                            style: TextStyle(
                              fontSize: 24,
                            ),
                          ),
                        ),
                        GridView(
                          gridDelegate:
                              SliverGridDelegateWithMaxCrossAxisExtentFixedHeight(
                            maxCrossAxisExtent: 400,
                            crossAxisSpacing: 24,
                          ),
                          shrinkWrap: true,
                          physics: NeverScrollableScrollPhysics(),
                          children: [
                            TextInput(
                              labelText: "Max refined in user's inventory",
                              controller: Controllers.maxRef,
                            ),
                            TextInput(
                              labelText: "Max keys in user's inventory",
                              controller: Controllers.maxKeys,
                            ),
                            TextInput(
                              labelText: "Minimum item price (refined)",
                              controller: Controllers.minRef,
                            ),
                            TextInput(
                              labelText: "Minimum item price (keys)",
                              controller: Controllers.minKeys,
                            ),
                            TextInput(
                              labelText: "Max hours",
                              controller: Controllers.maxHours,
                            ),
                            TextInput(
                              labelText: "Max histories",
                              controller: Controllers.maxHistories,
                            ),
                          ],
                        ),
                        SwitchInput(
                          switchText: "Show items that are untradable",
                          controller: Controllers.untradable,
                        ),
                        SwitchInput(
                          switchText: "Show items without a value",
                          controller: Controllers.noValue,
                        ),
                        SwitchInput(
                          switchText: "Show weapon skins",
                          controller: Controllers.skins,
                        ),
                        SwitchInput(
                          switchText: "Group scan",
                          controller: Controllers.group,
                          stateUpdate: () {
                            States.scanSettingsInput(() {});
                          },
                        ),
                        GridView(
                          gridDelegate:
                              SliverGridDelegateWithMaxCrossAxisExtentFixedHeight(
                            maxCrossAxisExtent: 400,
                            crossAxisSpacing: 24,
                          ),
                          shrinkWrap: true,
                          physics: NeverScrollableScrollPhysics(),
                          children: [
                            TextInput(
                              labelText: "Pages to scan",
                              controller: Controllers.pages,
                            ),
                            TextInput(
                              labelText: "Pages to skip",
                              controller: Controllers.skip,
                            ),
                          ],
                        ),
                      ],
                    ),
                    ScanSettingsInput(),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
