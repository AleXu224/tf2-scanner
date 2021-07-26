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

class OptionsInput extends StatelessWidget {
  const OptionsInput({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.red,
      height: 128,
      child: Container(),
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
