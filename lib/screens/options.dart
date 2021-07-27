import 'package:bpscanner/widgets/listInput.dart';
import 'package:bpscanner/widgets/textInput.dart';
import 'package:bpscanner/widgets/switchInput.dart';
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
              OptionsInput(),
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
          ListView(
            children: [
              TextInput(labelText: "Settings1", controller: Controllers.maxRef),
              SwitchInput(switchText: "Settings2", controller: Controllers.skins),
              ListInput(labelText: "Settings3", controller: Controllers.scanMode, options: ["Server", "Group", "Scrape"]),
            ],
          ),
          ListView(
            children: [
              TextInput(labelText: "Api key", controller: Controllers.apiKey),
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
        Controllers.scanSettingsInput.controller.text = Controllers.scanSettingsInput.controller.value.text.replaceAll("\n", " ");
        break;
      case 2:
        Controllers.scanSettingsInput.controller.text = Controllers.scanSettingsInput.controller.value.text.replaceAll("\n", " ");
        h = "Profile link";
        break;
    }
    return AnimatedContainer(
      duration: Duration(milliseconds: 100),
      height: Controllers.scanMode.option == 0 ? 128 : 52,
      margin: EdgeInsets.all(16),
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
