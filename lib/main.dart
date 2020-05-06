import 'navigation.dart';
import 'package:flutter/material.dart';
import 'widgets.dart' as widgets;
import 'globals.dart' as g;
import 'logic.dart';
import 'router.dart' as router;
import 'locator.dart';

void main() {
  runApp(MyApp());
}

void init() {
  g.config = Config();
  g.config.initialize();
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  // List<Widget> users = List.generate(20, (i) => widgets.UserContainer());

  @override
  void initState() {
    super.initState();
    setupLocator();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: "Squizel's scanner",
      theme: widgets.theme(),
      debugShowCheckedModeBanner: false,
      onGenerateRoute: router.generateRoute,
      initialRoute: "/",
      navigatorKey: locator<NavigationService>().navigatorKey,
    );
  }
}

class Home extends StatefulWidget {
  @override
  _HomeState createState() => _HomeState();
}

class _HomeState extends State<Home> {
  @override
  void initState() {
    super.initState();
    g.homeState = () => setState(() {});
    init();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: g.primary,
      body: Row(
        children: [
          widgets.SideBar(),
          Expanded(
            child: Scrollbar(
              child: ListView.builder(
                itemCount: g.users.length,
                itemBuilder: (buildContext, i) {
                  return g.users[i];
                },
              ),
            ),
          ),
        ],
      ),
    );
  }
}
