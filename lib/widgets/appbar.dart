import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class AppBarCustom extends StatelessWidget {
  final List<Widget>? leftButtons;
  final List<Widget>? rightButtons;
  const AppBarCustom({Key? key, this.leftButtons, this.rightButtons}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 48,
      color: ThemeColors.p,
      child: Row(
        children: [
          ...(leftButtons ?? []),
          Expanded(child: Container()),
          ...(rightButtons ?? []),
        ],
      ),
    );
  }
}
