import 'package:bpscanner/globals.dart';
import 'package:bpscanner/widgets/misc.dart';
import 'package:flutter/material.dart';

class UserButton extends StatefulWidget {
  final IconData buttonIcon;
  final String tooltip;
  final Function action;
  UserButton(this.buttonIcon, this.tooltip, this.action);
  @override
  _UserButtonState createState() => _UserButtonState();
}

class _UserButtonState extends State<UserButton> {
  Color bgColor = ThemeColors.pL;
  double radius = 4;
  Color iconColor = ThemeColors.t;
  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (PointerEvent details) {
        setState(() {
          this.bgColor = ThemeColors.s;
          this.iconColor = ThemeColors.p;
        });
      },
      onExit: (PointerEvent details) {
        setState(() {
          this.bgColor = ThemeColors.pL;
          this.iconColor = ThemeColors.t;
        });
      },
      child: GestureDetector(
        onTap: () {
          widget.action();
        },
        child: CustomTooltip(
          message: widget.tooltip,
          child: AnimatedContainer(
            duration: Duration(milliseconds: 200),
            width: 32,
            height: 32,
            decoration: BoxDecoration(
              borderRadius: BorderRadius.all(
                Radius.circular(4),
              ),
              color: this.bgColor,
            ),
            child: Icon(
              widget.buttonIcon,
              color: this.iconColor,
            ),
          ),
        ),
      ),
    );
  }
}

class AppBarButton extends StatefulWidget {
  final String displayText;
  final Function action;
  const AppBarButton({Key? key, required this.displayText, required this.action}) : super(key: key);

  @override
  State<AppBarButton> createState() => _AppBarButtonState();
}

class _AppBarButtonState extends State<AppBarButton> {
  bool isHovered = false;
  Color bgc = ThemeColors.pL;
  Color tc = ThemeColors.t;
  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        widget.action();
      },
      child: MouseRegion(
        onEnter: (e) {
          bgc = ThemeColors.s;
          tc = ThemeColors.p;
          setState(() {});
        },
        onExit: (e) {
          bgc = ThemeColors.pL;
          tc = ThemeColors.t;
          setState(() {});
        },
        child: AnimatedContainer(
          duration: Duration(milliseconds: 100),
          height: 48,
          color: bgc,
          padding: EdgeInsets.only(
            left: 16,
            right: 16,
          ),
          child: Center(
            child: Text(
              widget.displayText,
              style: TextStyle(
                color: tc,
                fontSize: 16,
              ),
            ),
          ),
        ),
      ),
    );
  }
}

class OptionsButton extends StatefulWidget {
  final String name;
  final Function action;
  final bool isImportant;
  const OptionsButton({Key? key, required this.name, required this.action, this.isImportant: false}) : super(key: key);

  @override
  _OptionsButtonState createState() => _OptionsButtonState();
}

class _OptionsButtonState extends State<OptionsButton> {
  Color b = ThemeColors.pL;
  Color t = ThemeColors.t;
  @override
  void initState() {
    super.initState();
    b = widget.isImportant ? ThemeColors.e : ThemeColors.pL;
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        widget.action();
      },
      child: MouseRegion(
        onEnter: (e) {
          b = widget.isImportant ? ThemeColors.eL : ThemeColors.s;
          t = widget.isImportant ? ThemeColors.tL : ThemeColors.p;
          setState(() {});
        },
        onExit: (e) {
          b = widget.isImportant ? ThemeColors.e : ThemeColors.pL;
          t = ThemeColors.t;
          setState(() {});
        },
        child: AnimatedContainer(
          duration: Duration(milliseconds: 100),
          height: 32,
          margin: EdgeInsets.only(top: 16, left: 16, right: 16),
          decoration: BoxDecoration(
            color: b,
            borderRadius: BorderRadius.circular(4),
          ),
          child: Center(
            child: Text(
              widget.name,
              style: TextStyle(
                color: t,
                fontSize: 14,
              ),
            ),
          ),
        ),
      ),
    );
  }
}
