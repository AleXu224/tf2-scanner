import 'package:bpscanner/globals.dart';
import 'package:flutter/material.dart';

class ScanSettingsInput extends StatefulWidget {
  @override
  ScanSettingsInputState createState() => ScanSettingsInputState();
}

class ScanSettingsInputState extends State<ScanSettingsInput> {
  bool borderActive = false;
  bool inputActive = false;

  FocusNode _focus = new FocusNode();

  @override
  void initState() {
    super.initState();
    States.scanSettingsInput = setState;

    _focus.addListener(() {
      if (_focus.hasFocus) {
        inputActive = true;
      } else {
        inputActive = false;
      }
      setState(() {});
    });
  }

  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (e) {
        setState(() {
          borderActive = true;
        });
      },
      onExit: (e) {
        setState(() {
          borderActive = false;
        });
      },
      child: AnimatedContainer(
        curve: Curves.easeOutQuint,
        duration: Duration(milliseconds: 200),
        decoration: BoxDecoration(
          color: ThemeColors.primary,
          border: Border(
            left: BorderSide(
              color: borderActive || inputActive
                  ? ThemeColors.secondary
                  : Colors.transparent,
              width: 1,
            ),
            top: BorderSide(
              color: borderActive || inputActive
                  ? ThemeColors.secondary
                  : ThemeColors.primaryDark,
              width: 1,
            ),
          ),
        ),
        height: this.inputActive
            ? (Controllers.group.state ? 36 : 200)
            : (Controllers.group.state ? 36 : 128),
        child: Scrollbar(
          child: TextFormField(
            focusNode: _focus,
            controller: Controllers.scanSettingsInput.controller,
            cursorColor: ThemeColors.secondary,
            maxLines: Controllers.group.state ? 1 : null,
            decoration: InputDecoration(
              border: InputBorder.none,
              contentPadding: EdgeInsets.all(8),
              isDense: true,
              hintText: Controllers.group.state ? "Group link" : "User list",
            ),
          ),
        ),
      ),
    );
  }
}
