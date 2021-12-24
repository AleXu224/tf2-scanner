import 'package:bpscanner/utilities.dart';
import 'package:bpscanner/widgets/buttons.dart';
import 'package:bpscanner/widgets/listInput.dart';
import 'package:bpscanner/widgets/textInput.dart';
import 'package:bpscanner/widgets/switchInput.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class OptionsDrawer extends StatelessWidget {
  const OptionsDrawer({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Drawer(
        child: Container(
          color: ThemeColors.p,
          child: Column(
            children: [
              Tabs(),
              OptionsContent(),
            ],
          ),
        ),
      ),
    );
  }
}

class OptionsContent extends StatelessWidget {
  const OptionsContent({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: TabBarView(
        children: [
          Stack(
            children: [
              ListView(
                controller: ScrollController(),
                children: [
                  ListInput(labelText: "Scan Mode", controller: Controllers.scanMode, options: ["Server", "Group", "Friendlist"]),
                  TextInput(labelText: "Max Refined", controller: Controllers.maxRef),
                  TextInput(labelText: "Max Keys", controller: Controllers.maxKeys),
                  TextInput(labelText: "Min Refined", controller: Controllers.minRef),
                  TextInput(labelText: "Min Keys", controller: Controllers.minKeys),
                  TextInput(labelText: "Max Hours", controller: Controllers.maxHours),
                  TextInput(labelText: "Max Histories", controller: Controllers.maxHistories),
                  SwitchInput(switchText: "Untradable Items", controller: Controllers.untradable),
                  SwitchInput(switchText: "Unpriced Items", controller: Controllers.noValue),
                  SwitchInput(switchText: "Skins", controller: Controllers.skins),
                  Container(height: Controllers.scanMode.option == 0 ? 128 + 16 : 52 + 16),
                ],
              ),
              Align(
                alignment: Alignment.bottomCenter,
                child: OptionsInput(),
              ),
            ],
          ),
          ListView(
            children: [
              TextInput(labelText: "Api key", controller: Controllers.apiKey),
              OptionsButton(
                name: "Save",
                action: () {
                  // Check if api key looks valid
                  Controllers.apiKey.controller.text = Controllers.apiKey.controller.text.replaceAll(" ", "");
                  if (Controllers.apiKey.controller.text.length != 32) {
                    showSnack(context, "Api key is invalid.");
                    return;
                  }

                  App.config.saveApiKey();
                },
              ),
              OptionsButton(
                name: "Reset Application Settings",
                action: () async {
                  SharedPreferences prefs = await SharedPreferences.getInstance();
                  await prefs.clear();
                  Navigator.pop(context);
                  App.config.init();
                },
                isImportant: true,
              ),
            ],
          ),
        ],
      ),
    );
  }
}

class OptionsInput extends StatefulWidget {
  const OptionsInput({Key? key}) : super(key: key);

  @override
  _OptionsInputState createState() => _OptionsInputState();
}

class _OptionsInputState extends State<OptionsInput> {
  String h = "If you see this message then something went really wrong";
  FocusNode _focus = new FocusNode();
  BoxBorder b = Border.all(color: ThemeColors.pL, width: 2);
  int prevScanMode = Controllers.scanMode.option;
  @override
  void initState() {
    super.initState();
    _focus.addListener(() {
      setState(() {
        if (_focus.hasFocus) {
          b = Border.all(color: ThemeColors.s, width: 2);
        } else {
          b = Border.all(color: ThemeColors.pL, width: 2);
        }
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    switch (Controllers.scanMode.option) {
      case 0:
        h = "User list";
        break;
      case 1:
        h = "Group link";
        break;
      case 2:
        h = "Profile link";
        break;
    }
    return Padding(
      padding: const EdgeInsets.all(8),
      child: Material(
        type: MaterialType.card,
        color: Colors.transparent,
        elevation: 4,
        child: AnimatedContainer(
          duration: Duration(milliseconds: 100),
          height: Controllers.scanMode.option == 0 ? 128 : 52,
          decoration: BoxDecoration(
            color: ThemeColors.pL,
            borderRadius: BorderRadius.circular(4),
            border: b,
          ),
          child: TextFormField(
            focusNode: _focus,
            controller: Controllers.scanSettingsInput.controller,
            cursorColor: ThemeColors.s,
            maxLines: Controllers.scanMode.option != 0 ? 1 : null,
            style: TextStyle(
              color: ThemeColors.t,
              fontSize: 14,
              height: 1.2,
            ),
            expands: Controllers.scanMode.option != 0 ? false : true,
            decoration: InputDecoration(
              border: InputBorder.none,
              contentPadding: EdgeInsets.all(14),
              hintText: h,
            ),
          ),
        ),
      ),
    );
  }
}

class Tabs extends StatelessWidget {
  const Tabs({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return ConstrainedBox(
      constraints: BoxConstraints(maxHeight: 48, minHeight: 48),
      child: TabBar(
        tabs: [
          Tab(text: "Scan"),
          Tab(text: "App"),
        ],
        indicatorColor: ThemeColors.s,
        labelStyle: TextStyle(fontSize: 16),
      ),
    );
  }
}
