import 'package:flutter/material.dart';

class SettingsList extends StatelessWidget {
  final List<Widget> items;
  SettingsList({required this.items});
  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: Center(
        child: Container(
          padding: EdgeInsets.only(
            left: 8,
            right: 8,
          ),
          constraints: BoxConstraints.loose(
            Size.fromWidth(600),
          ),
          width: 600,
          child: ListView(
            children: items,
          ),
        ),
      ),
    );
  }
}
