import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class TextInput extends StatefulWidget {
  final String labelText;
  final TextController controller;
  TextInput({required this.labelText, required this.controller});

  @override
  _TextInputState createState() => _TextInputState();
}

class _TextInputState extends State<TextInput> {
  @override
  Widget build(BuildContext context) {
    return Container(
      padding: EdgeInsets.only(top: 16, right: 16, left: 16),
      child: Column(
        children: [
          Align(
            child: Text(
              widget.labelText,
              style: TextStyle(
                fontSize: 16,
                color: ThemeColors.t,
              ),
            ),
            alignment: Alignment.centerLeft,
          ),
          TextFormField(
            controller: widget.controller.controller,
            cursorColor: ThemeColors.s,
            style: TextStyle(
              color: ThemeColors.t,
              fontSize: 12,
              height: 1.2,
              textBaseline: TextBaseline.ideographic,
            ),
            decoration: InputDecoration.collapsed(hintText: "").copyWith(
              border: OutlineInputBorder(),
              isDense: true,
              isCollapsed: true,
              contentPadding: EdgeInsets.only(
                top: 14,
                bottom: 14,
                left: 4,
                right: 4,
              ),
              focusColor: ThemeColors.pL,
              fillColor: ThemeColors.pL,
              hoverColor: ThemeColors.pL,
              filled: true,
              enabledBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: ThemeColors.pL,
                ),
              ),
              focusedBorder: OutlineInputBorder(
                borderSide: BorderSide(
                  color: ThemeColors.s,
                  width: 2,
                ),
                borderRadius: BorderRadius.circular(4),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
