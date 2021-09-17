import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class ListInput extends StatefulWidget {
  final ListController controller;
  final List<String> options;
  final String labelText;
  const ListInput({Key? key, required this.controller, required this.options, required this.labelText}) : super(key: key);

  @override
  _ListInputState createState() => _ListInputState();
}

class _ListInputState extends State<ListInput> {
  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.only(top: 16, right: 16, left: 16),
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
          ClipRRect(
            borderRadius: BorderRadius.circular(4),
            child: Container(
              height: 32,
              color: ThemeColors.pL,
              child: Row(
                children: List.generate(
                  widget.options.length,
                  (index) => ListInputButton(
                    index: index,
                    options: widget.options,
                    controller: widget.controller,
                  ),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class ListInputButton extends StatefulWidget {
  final int index;
  final List<String> options;
  final ListController controller;
  const ListInputButton({Key? key, required this.index, required this.options, required this.controller}) : super(key: key);

  @override
  _ListInputButtonState createState() => _ListInputButtonState();
}

class _ListInputButtonState extends State<ListInputButton> {
  Color bg = ThemeColors.pL;
  Color t = ThemeColors.t;
  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: GestureDetector(
        onTap: () {
          widget.controller.option = widget.index;
          Controllers.scanSettingsInput.controller.text = "";
          Controllers.scanSettingsInput.controller.selection = TextSelection.collapsed(offset: 0);
          States.setState();
        },
        child: MouseRegion(
          onEnter: (e) {
            bg = ThemeColors.s;
            t = ThemeColors.p;
            setState(() {});
          },
          onExit: (e) {
            bg = ThemeColors.pL;
            t = ThemeColors.t;
            setState(() {});
          },
          child: AnimatedContainer(
            duration: Duration(milliseconds: 100),
            color: widget.controller.option == widget.index ? ThemeColors.s : bg,
            child: Center(
              child: Text(
                widget.options[widget.index],
                style: TextStyle(
                  color: widget.controller.option == widget.index ? ThemeColors.p : t,
                  height: 1,
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
