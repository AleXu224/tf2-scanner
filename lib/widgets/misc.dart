import 'package:flutter/material.dart';

class CustomTooltip extends StatelessWidget {
  final String message;
  final Widget child;
  CustomTooltip({required this.message, required this.child});
  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: this.message,
      decoration: BoxDecoration(
        color: Color.fromRGBO(0, 0, 0, 0.8),
        borderRadius: BorderRadius.circular(5),
      ),
      padding: EdgeInsets.all(5),
      textStyle: TextStyle(
        color: Colors.white70,
        fontSize: 15,
        height: 1,
      ),
      preferBelow: false,
      child: this.child,
    );
  }
}
