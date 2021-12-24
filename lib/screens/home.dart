import 'package:bpscanner/unfocuser.dart';
import 'package:bpscanner/widgets/buttons.dart';
import 'package:bpscanner/widgets/appbar.dart';
import 'package:bpscanner/widgets/scanStatus.dart';
import 'package:bpscanner/widgets/userContainer.dart';
import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';
import 'package:bpscanner/utilities.dart';
import 'popup.dart';
import 'options.dart';

class Home extends StatefulWidget {
  @override
  _HomeState createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  void initState() {
    super.initState();
    App.config.init();
    App.users = List.generate(50, (index) => UserContainer());
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
        body: Stack(
          children: [
            // Main content
            Column(
              children: [
                AppBarCustom(
                  leftButtons: [
                    Builder(builder: (context) {
                      return AppBarButton(
                        displayText: App.scanner.isScanning
                            ? App.scanner.stopScanning
                                ? "Stopping scan..."
                                : "Stop scan"
                            : "Start scan",
                        action: () {
                          if (App.scanner.isScanning) {
                            App.scanner.stopScan();
                          } else {
                            App.scanner.scan(context);
                          }
                        },
                      );
                    }),
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
                      children: App.users,
                    ),
                  ),
                ),
              ],
            ),
            // Loading screen
            PopupScreen(),
            if (App.scanner.isScanning)
              Positioned(
                bottom: 0,
                right: 0,
                child: Padding(
                  padding: EdgeInsets.all(32),
                  child: ScanStatus(),
                ),
              ),
          ],
        ),
      ),
    );
  }
}
