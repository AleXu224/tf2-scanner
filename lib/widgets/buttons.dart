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

class SideButton extends StatefulWidget {
  final IconData buttonIcon;
  final String tooltip;
  final Function action;
  SideButton(this.buttonIcon, this.tooltip, this.action);
  @override
  _SideButtonState createState() => _SideButtonState();
}

class _SideButtonState extends State<SideButton> {
  Color bgColor = ThemeColors.primary;
  double radius = 20;
  Color iconColor = Colors.white60;
  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (PointerEvent details) {
        setState(() {
          this.bgColor = ThemeColors.secondaryLight;
          this.radius = 10;
          this.iconColor = Colors.white70;
        });
      },
      onExit: (PointerEvent details) {
        setState(() {
          this.bgColor = ThemeColors.primary;
          this.radius = 20;
          this.iconColor = Colors.white60;
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
            width: 40,
            height: 40,
            margin: EdgeInsets.all(10),
            decoration: BoxDecoration(
              borderRadius: BorderRadius.all(
                Radius.circular(this.radius),
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
