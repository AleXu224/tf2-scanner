import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class OptionsDrawer extends StatelessWidget {
  const OptionsDrawer({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Drawer(
      child: Container(
        color: ThemeColors.p,
      ),
    );
  }
}
