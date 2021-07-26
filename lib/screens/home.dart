import 'package:bpscanner/unfocuser.dart';
import 'package:bpscanner/widgets/buttons.dart';
import 'package:bpscanner/widgets/appbar.dart';
import 'package:bpscanner/widgets/userContainer.dart';
import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';
import 'package:bpscanner/utilities.dart';
import 'options.dart';

class Home extends StatefulWidget {
  @override
  _HomeState createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  void initState() {
    super.initState();
    Scanner.config.init();
    Scanner.users = List.generate(50, (index) => UserContainer());
    States.setState = () {
      setState(() {});
    };
  }

  @override
  Widget build(BuildContext context) {
    return Unfocuser(
      child: Scaffold(
        endDrawer: OptionsDrawer(),
        backgroundColor: ThemeColors.pD,
        body: Column(
          children: [
            AppBarCustom(
              leftButtons: [
                AppBarButton(
                  displayText: "Start Scan",
                  // TODO: start scan
                  action: () {},
                ),
              ],
              rightButtons: [
                Builder(builder: (context) {
                  return AppBarButton(
                    displayText: "Options",
                    action: () {
                      Scaffold.of(context).openEndDrawer();
                    },
                  );
                }),
              ],
            ),
            Expanded(
              child: Scrollbar(
                child: ListView(
                  children: Scanner.users,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
