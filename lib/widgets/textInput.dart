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
    return ListTile(
      contentPadding: EdgeInsets.all(0),
      title: Padding(
        padding: const EdgeInsets.only(
          bottom: 4,
        ),
        child: Text(
          widget.labelText,
          style: TextStyle(
            fontSize: 14,
            color: Colors.white70,
          ),
        ),
      ),
      subtitle: TextFormField(
        controller: widget.controller.controller,
        cursorColor: ThemeColors.secondary,
        validator: (a) {
          return "oh no";
        },
        decoration: InputDecoration(
          border: OutlineInputBorder(),
          isDense: true,
          contentPadding: EdgeInsets.all(12),
          enabledBorder: OutlineInputBorder(
            borderSide: BorderSide(
              color: Colors.white24,
            ),
          ),
          focusedBorder: OutlineInputBorder(
            borderSide: BorderSide(
              color: ThemeColors.secondary,
              width: 2,
            ),
          ),
        ),
      ),
    );
  }
}
