import 'package:bpscanner/widgets/buttons.dart';
import 'package:flutter/material.dart';

import '../globals.dart';

class Sidebar extends StatefulWidget {
  final List<SideButton> topButtons;
  final List<SideButton> bottomButtons;
  Sidebar({required this.topButtons, this.bottomButtons = const []});
  @override
  _SidebarState createState() => _SidebarState();
}

class _SidebarState extends State<Sidebar> {
  @override
  Widget build(BuildContext context) {
    return Container(
      width: 60,
      color: ThemeColors.primaryDark,
      child: Column(
        children: [
          ...widget.topButtons,
          Expanded(
            child: Container(),
          ),
          ...widget.bottomButtons,
        ],
      ),
    );
  }
}
