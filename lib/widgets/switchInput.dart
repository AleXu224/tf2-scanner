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
    return Container(
      height: 16,
      margin: EdgeInsets.only(top: 16, right: 16, left: 16),
      child: Row(
        children: [
          Text(
            widget.switchText,
            style: TextStyle(
              fontSize: 16,
              color: ThemeColors.t,
              height: 1,
            ),
          ),
          Expanded(child: Container()),
          SwitchInputButton(controller: widget.controller),
        ],
      ),
    );
  }
}

class SwitchInputButton extends StatefulWidget {
  final SwitchController controller;
  const SwitchInputButton({Key? key, required this.controller}) : super(key: key);

  @override
  _SwitchInputButtonState createState() => _SwitchInputButtonState();
}

class _SwitchInputButtonState extends State<SwitchInputButton> {
  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        widget.controller.state = !widget.controller.state;
        setState(() {});
      },
      child: AnimatedContainer(
        duration: Duration(milliseconds: 100),
        height: 16,
        width: 16,
        margin: EdgeInsets.only(right: 8),
        decoration: BoxDecoration(
          color: widget.controller.state ? ThemeColors.s : ThemeColors.p,
          borderRadius: BorderRadius.circular(4),
          border: Border.all(
            color: ThemeColors.s,
            width: 2,
            style: BorderStyle.solid,
          ),
        ),
      ),
    );
  }
}
