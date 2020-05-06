import 'package:flutter/material.dart';
import 'main.dart';
import 'scanInfo.dart';
import 'settings.dart';
import 'inventory.dart';
import 'logic.dart';

Route<dynamic> generateRoute(RouteSettings settings) {
  switch (settings.name) {
    case '/':
      return SlideRoute(widget: Home());
    case '/scanInfo':
      return SlideRoute(widget: ScanInfo());
    case '/settings':
      bool displayBack = settings.arguments;
      return SlideRoute(widget: Settings(displayBack));
    case '/inventory':
      var args = settings.arguments;
      return SlideRoute(
          widget: Inventory(
        player: (args as Map)["player"],
        items: (args as Map)["items"],
      ));
    case '/splash':
      return SlideRoute(widget: SplashScreen());
    default:
      return SlideRoute(widget: Home());
  }
}

class SlideRoute extends PageRouteBuilder {
  Widget widget;
  SlideRoute({this.widget})
      : super(
          pageBuilder: (BuildContext context, Animation<double> animation,
              Animation<double> secondAnimation) {
            return widget;
          },
          transitionDuration: Duration(milliseconds: 200),
          transitionsBuilder: (BuildContext context,
              Animation<double> animation,
              Animation<double> secondAnimation,
              Widget child) {
            var begin = Offset(-1.0, 0.0);
            var end = Offset.zero;
            var tween = Tween(begin: begin, end: end);
            var offsetAnimation = animation.drive(tween);
            return SlideTransition(
              position: offsetAnimation,
              child: child,
            );
          },
        );
}
