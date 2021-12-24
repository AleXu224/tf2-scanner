import 'package:flutter/material.dart';
import 'globals.dart';
import 'package:http/http.dart';

void changeScreen(Widget newScreen) {
  Navigator.push(
    GlobalNav.navKey.currentContext!,
    PageRouteBuilder(
      pageBuilder: (context, animation, animation2) {
        return newScreen;
      },
      transitionDuration: Duration(milliseconds: 200),
      transitionsBuilder: (BuildContext context, Animation<double> animation, Animation<double> secondAnimation, Widget child) {
        var begin = Offset(-1.0, 0.0);
        var end = Offset.zero;
        var tween = Tween(begin: begin, end: end);
        var offsetAnimation = animation.drive(tween);
        return SlideTransition(
          position: offsetAnimation,
          child: child,
        );
      },
    ),
  );
}

void showSnack(BuildContext context, String message) {
  ScaffoldMessenger.of(context).showSnackBar(
    SnackBar(
      content: Text(
        message,
        style: TextStyle(
          color: ThemeColors.t,
          fontSize: 14,
        ),
      ),
      backgroundColor: ThemeColors.p,
    ),
  );
}

Future<FetchResponse> fetch({required String url}) async {
  var request = await get(Uri.parse(url));
  String response = request.body;
  bool success = true;

  if (request.statusCode != 200) success = false;

  return new FetchResponse(
    success: success,
    status: request.statusCode,
    body: response,
    request: request,
  );
}

List<String> regExpAll(RegExp exp, String search) {
  return exp.allMatches(search).map((e) => search.substring(e.start, e.end)).toList();
}

class FetchResponse {
  bool success;
  int status;
  String body;
  Response request;
  FetchResponse({
    required this.success,
    required this.status,
    required this.body,
    required this.request,
  });
}

class RequestLimiter {
  static List<int> requestQueue = [];
  // The ammount of requests allowed per second
  static int requestLimit = 30;

  // Returns the time until a request can be made
  // requestQueue stores the timestamps at which requests are going to be made
  // Only 30 requests per minute
  static int nextRequestTime() {
    RequestLimiter.cleanRequestQueue();
    int now = DateTime.now().millisecondsSinceEpoch;
    int nextRequest = 0;
    if (RequestLimiter.requestQueue.length != 0) nextRequest = RequestLimiter.requestQueue.last - now + (60 / requestLimit * 1000).round();
    RequestLimiter.requestQueue.add(now + nextRequest);
    return nextRequest;
  }

  // Cleans the requestQueue
  static void cleanRequestQueue() {
    RequestLimiter.requestQueue.removeWhere((e) => e < DateTime.now().millisecondsSinceEpoch - (60 / requestLimit * 1000).round());
  }
}
