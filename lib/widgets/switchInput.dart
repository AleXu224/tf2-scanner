import 'package:bpscanner/globals.dart';
import 'package:flutter/material.dart';

defaultValue() {}

class SwitchInput extends StatefulWidget {
  final String switchText;
  final SwitchController controller;
  final Function() stateUpdate;
  SwitchInput({
    required this.switchText,
    required this.controller,
    this.stateUpdate = defaultValue,
  });
  @override
  _SwitchInputState createState() => _SwitchInputState();
}

class _SwitchInputState extends State<SwitchInput> {
  @override
  Widget build(BuildContext context) {
    return ListTile(
      contentPadding: EdgeInsets.all(0),
      title: Text(
        widget.switchText,
        style: TextStyle(color: Colors.white70),
      ),
      trailing: Switch(
        value: widget.controller.state,
        onChanged: (newState) {
          setState(() {
            widget.controller.state = newState;
          });
          widget.stateUpdate();
        },
        activeColor: ThemeColors.secondary,
      ),
    );
  }
}
