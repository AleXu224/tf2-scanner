import 'package:bpscanner/screens/home.dart';
import "package:flutter/material.dart";
import 'globals.dart';

void main() {
  runApp(Scanner());
}

class Scanner extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      title: "TF2 Scanner",
      debugShowCheckedModeBanner: false,
      home: Home(),
      navigatorKey: GlobalNav.navKey,
    );
  }
}
