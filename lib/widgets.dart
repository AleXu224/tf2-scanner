import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';
import 'globals.dart' as g;
import "scanning.dart" as s;
import 'package:transparent_image/transparent_image.dart' as k;
import 'package:open_url/open_url.dart' as open_url;

import 'scanning.dart';

ThemeData theme() {
  return ThemeData(
      primaryColor: Color(0xFF2D2E33),
      primaryColorDark: Color(0xFF202124),
      primaryColorLight: Color(0xFF57585C),
      accentColor: Color(0xFF5D5DF2),
      visualDensity: VisualDensity.adaptivePlatformDensity);
}

class SideButton extends StatefulWidget {
  final IconData buttonIcon;
  final String tooltip;
  final Function action;
  const SideButton(this.buttonIcon, this.action, this.tooltip);
  @override
  _SideButtonState createState() => _SideButtonState();
}

class _SideButtonState extends State<SideButton> {
  Color bgColor = g.primary;
  double radius = 20;
  Color icColor = g.white60;
  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (PointerEvent details) {
        setState(() {
          bgColor = g.secondaryLight;
          radius = 10;
          icColor = g.white80;
        });
      },
      onExit: (PointerEvent details) {
        setState(() {
          bgColor = g.primary;
          radius = 20;
          icColor = g.white60;
        });
      },
      child: GestureDetector(
        onTap: () {
          widget.action();
        },
        child: CustomTooltip(
          message: widget.tooltip,
          child: AnimatedContainer(
            width: 40,
            height: 40,
            margin: EdgeInsets.all(10),
            duration: Duration(milliseconds: 200),
            decoration: BoxDecoration(
                borderRadius: BorderRadius.all(Radius.circular(radius)),
                color: bgColor),
            child: Icon(
              widget.buttonIcon,
              color: icColor,
            ),
          ),
        ),
      ),
    );
  }
}

Widget filler() {
  return Expanded(child: Container());
}

// void openScanInfo(context) {
//   Navigator.push(
//     context,
//     PageRouteBuilder(
//         pageBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation) {
//           return ScanInfo();
//         },
//         transitionDuration: Duration(milliseconds: 200),
//         transitionsBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation, Widget child) {
//           var begin = Offset(-1.0, 0.0);
//           var end = Offset.zero;
//           var tween = Tween(begin: begin, end: end);
//           var offsetAnimation = animation.drive(tween);
//           return SlideTransition(
//             position: offsetAnimation,
//             child: child,
//           );
//         }),
//   );
// }

// void openSettings(context, {bool displayBack = true}) {
//   Navigator.push(
//     context,
//     PageRouteBuilder(
//         pageBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation) {
//           return Settings(displayBack);
//         },
//         transitionDuration: Duration(milliseconds: 200),
//         transitionsBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation, Widget child) {
//           var begin = Offset(-1.0, 0.0);
//           var end = Offset.zero;
//           var tween = Tween(begin: begin, end: end);
//           var offsetAnimation = animation.drive(tween);
//           return SlideTransition(
//             position: offsetAnimation,
//             child: child,
//           );
//         }),
//   );
// }

// void openWindow(context, window) {
//   Navigator.push(
//     context,
//     PageRouteBuilder(
//         pageBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation) {
//           return window;
//         },
//         transitionDuration: Duration(milliseconds: 200),
//         transitionsBuilder: (BuildContext context, Animation<double> animation,
//             Animation<double> secondAnimation, Widget child) {
//           var begin = Offset(-1.0, 0.0);
//           var end = Offset.zero;
//           var tween = Tween(begin: begin, end: end);
//           var offsetAnimation = animation.drive(tween);
//           return SlideTransition(
//             position: offsetAnimation,
//             child: child,
//           );
//         }),
//   );
// }

// void closeNavigator(context) {
//   Navigator.pop(context);
// }

class SideBar extends StatefulWidget {
  @override
  _SideBarState createState() => _SideBarState();
}

class _SideBarState extends State<SideBar> {
  @override
  void initState() {
    super.initState();
    g.sideBarState = () {
      setState(() {});
    };
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 60,
      color: g.primaryDark,
      child: Column(
        children: [
          SideButton(
            Icons.menu,
            () {
              g.navigationService.navigateTo("/scanInfo");
            },
            "Scan options",
          ),
          if (!g.isScanning || g.stopScan)
            SideButton(Icons.play_arrow, s.startScan, "Start scan"),
          if (g.isScanning && !g.stopScan)
            SideButton(Icons.stop, () {
              setState(() {
                g.stopScan = true;
              });
            }, "Stop Scan"),
          filler(),
          SideButton(
            Icons.settings,
            () {
              g.navigationService.navigateTo("/settings", arguments: true);
            },
            "Settings",
          ),
        ],
      ),
    );
  }
}

Widget scanInfoSideBar() {
  return Container(
    width: 60,
    color: g.primaryDark,
    child: Column(
      children: [
        SideButton(
          Icons.arrow_back,
          () {
            g.navigationService.goBack();
          },
          "Back",
        ),
        SideButton(Icons.play_arrow, () {
          s.startScan();
          g.navigationService.goBack();
        }, "Start scan"),
      ],
    ),
  );
}

Widget settingsSideBar(bool closeButton) {
  return Container(
    width: 60,
    color: g.primaryDark,
    child: Column(
      children: [
        closeButton
            ? SideButton(
                Icons.arrow_back, () => g.navigationService.goBack(), "Back")
            : Text(""),
      ],
    ),
  );
}

Widget titleText(String text) {
  return Align(
    child: Text(
      text,
      style: TextStyle(
        color: g.white60,
        fontSize: 25,
        fontWeight: FontWeight.w600,
      ),
    ),
    alignment: Alignment.centerLeft,
  );
}

Widget subtitleText(String text) {
  return Align(
    child: Text(
      text,
      style: TextStyle(
        color: g.white50,
        fontSize: 15,
        fontWeight: FontWeight.w400,
      ),
    ),
    alignment: Alignment.centerLeft,
  );
}

Widget optionList({Widget child}) {
  return Expanded(
    child: Scrollbar(
      child: ListView(
        children: [
          Row(
            children: [
              filler(),
              Container(
                child: child,
                width: 600,
              ),
              filler()
            ],
          )
        ],
      ),
    ),
  );
}

class ValueInput extends StatefulWidget {
  final String text;
  final TextEditingController textController;
  final bool isLarge;
  ValueInput({this.text, this.textController, this.isLarge = false});
  @override
  _ValueInputState createState() => _ValueInputState();
}

class _ValueInputState extends State<ValueInput> {
  FocusNode _focus = new FocusNode();

  double h = 1;
  double w = 60;
  bool isActive = false;

  double initialWidth = 60;

  Color lineColor = g.primaryLight;
  Color inputColor = g.white05;
  @override
  void initState() {
    super.initState();
    if (widget.isLarge) initialWidth = 210;
    setState(() {
      w = initialWidth;
    });
    _focus.addListener(() {
      if (_focus.hasFocus) {
        w = 600;
        h = 2;
        isActive = true;
        lineColor = g.secondary;
        setState(() {});
      } else {
        w = initialWidth;
        h = 1;
        isActive = false;
        lineColor = g.primaryLight;
        setState(() {});
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 30,
      margin: EdgeInsets.only(top: 10),
      child: Stack(
        children: [
          Container(
            child: Row(
              children: [
                Text(
                  widget.text,
                  style: TextStyle(color: g.white60, fontSize: 16),
                ),
                filler(),
                MouseRegion(
                  onEnter: (event) {
                    setState(() {
                      w = 600;
                      inputColor = g.white10;
                    });
                  },
                  onExit: (event) {
                    setState(() {
                      inputColor = g.white05;
                      if (!isActive) {
                        w = initialWidth;
                      }
                    });
                  },
                  child: AnimatedContainer(
                    duration: Duration(milliseconds: 200),
                    decoration: BoxDecoration(
                        color: inputColor,
                        borderRadius: BorderRadius.only(
                          topLeft: Radius.circular(5),
                          topRight: Radius.circular(5),
                        )),
                    width: initialWidth,
                    height: 30,
                    child: TextField(
                      controller: widget.textController,
                      focusNode: _focus,
                      style: TextStyle(
                        fontSize: 20,
                        height: 1,
                        color: g.white60,
                      ),
                      decoration: InputDecoration(
                        border: InputBorder.none,
                        contentPadding: EdgeInsets.all(5),
                      ),
                    ),
                  ),
                ),
              ],
            ),
          ),
          Align(
            child: AnimatedContainer(
              duration: Duration(milliseconds: 200),
              height: h,
              width: w,
              color: lineColor,
            ),
            alignment: Alignment.bottomRight,
          ),
        ],
      ),
    );
  }
}

class SwitchInput extends StatefulWidget {
  final String text;
  final bool value;
  final Function setter;
  SwitchInput({this.text, this.value, this.setter});
  @override
  _SwitchInputState createState() => _SwitchInputState();
}

class _SwitchInputState extends State<SwitchInput> {
  bool isSwitched = false;
  @override
  void initState() {
    super.initState();
    setState(() {
      isSwitched = widget.value;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      height: 30,
      margin: EdgeInsets.only(top: 10),
      child: Container(
        child: Row(
          children: [
            Text(
              widget.text,
              style: TextStyle(color: g.white60, fontSize: 16),
            ),
            filler(),
            Switch(
              value: isSwitched,
              inactiveThumbColor: g.primaryLight,
              onChanged: (bool newVal) {
                widget.setter(newVal);
                setState(() {
                  isSwitched = newVal;
                });
              },
            ),
          ],
        ),
      ),
    );
  }
}

class ActionButton extends StatefulWidget {
  final String text;
  final Function action;
  ActionButton(this.text, this.action);
  @override
  _ActionButtonState createState() => _ActionButtonState();
}

class _ActionButtonState extends State<ActionButton> {
  Color backgroundColor = g.primary;
  double radius = 20;
  Color textColor = g.white60;
  @override
  Widget build(BuildContext context) {
    return MouseRegion(
      onEnter: (event) {
        setState(() {
          backgroundColor = g.secondary;
          radius = 10;
          textColor = g.white80;
        });
      },
      onExit: (event) {
        setState(() {
          backgroundColor = g.primary;
          radius = 20;
          textColor = g.white60;
        });
      },
      child: GestureDetector(
        onTap: widget.action,
        child: AnimatedContainer(
          height: 40,
          decoration: BoxDecoration(
            color: backgroundColor,
            borderRadius: BorderRadius.all(
              Radius.circular(radius),
            ),
          ),
          padding: EdgeInsets.all(10),
          duration: Duration(milliseconds: 200),
          child: Text(
            widget.text,
            style: TextStyle(fontSize: 20, height: 1, color: textColor),
          ),
        ),
      ),
    );
  }
}

class UserButton extends StatefulWidget {
  final IconData icon;
  final String message;
  final Function action;
  UserButton({this.icon, this.message, this.action});
  @override
  _UserButtonState createState() => _UserButtonState();
}

class _UserButtonState extends State<UserButton> {
  Color buttonColor = g.primary;
  double radius = 20;
  Color iconColor = g.white60;
  @override
  Widget build(BuildContext context) {
    return CustomTooltip(
      message: widget.message,
      child: MouseRegion(
        onEnter: (event) {
          setState(() {
            buttonColor = g.secondary;
            radius = 10;
            iconColor = g.white80;
          });
        },
        onExit: (event) {
          setState(() {
            buttonColor = g.primary;
            radius = 20;
            iconColor = g.white60;
          });
        },
        child: GestureDetector(
          onTap: widget.action,
          child: AnimatedContainer(
            duration: Duration(milliseconds: 200),
            child: Icon(
              widget.icon,
              color: iconColor,
            ),
            margin: EdgeInsets.symmetric(vertical: 35 / 2, horizontal: 5),
            height: 40,
            width: 40,
            decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(radius),
                color: buttonColor),
          ),
        ),
      ),
    );
  }
}

class InfoCard extends StatelessWidget {
  final String message;
  final String tooltipMessage;
  InfoCard({this.message, this.tooltipMessage});
  @override
  Widget build(BuildContext context) {
    return CustomTooltip(
      message: this.tooltipMessage,
      child: MouseRegion(
        child: AnimatedContainer(
          duration: Duration(milliseconds: 200),
          height: 30,
          padding: EdgeInsets.symmetric(horizontal: 10, vertical: 15 / 2),
          margin: EdgeInsets.symmetric(horizontal: 5, vertical: 10),
          decoration: BoxDecoration(
              color: g.primary, borderRadius: BorderRadius.circular(15)),
          child: Text(
            this.message,
            style: TextStyle(
              color: g.white60,
              fontSize: 15,
              height: 1,
            ),
          ),
        ),
      ),
    );
  }
}

class UserContainer extends StatefulWidget {
  final Player player;
  final List<Item> items;
  UserContainer(this.player, this.items);
  @override
  _UserContainerState createState() => _UserContainerState();
}

class _UserContainerState extends State<UserContainer> {
  @override
  Widget build(BuildContext context) {
    return Container(
      color: g.primaryDark,
      child: Column(
        children: [
          Container(
            height: 75,
            child: Row(
              children: [
                CustomTooltip(
                  message: widget.player.name,
                  child: GestureDetector(
                    onTap: () {
                      open_url.openUrl(
                          "https://steamcommunity.com/profiles/${widget.player.steamid}");
                    },
                    child: Container(
                      height: 55,
                      width: 55,
                      margin: EdgeInsets.all(10),
                      decoration: BoxDecoration(
                          borderRadius: BorderRadius.circular(55),
                          color: g.secondary),
                      child: ClipRRect(
                        borderRadius: BorderRadius.circular(55),
                        child: FadeInImage(
                          placeholder: MemoryImage(k.kTransparentImage),
                          image: NetworkImage(widget.player.avatarUrl),
                        ),
                      ),
                    ),
                  ),
                ),
                UserButton(
                  message: "Add friend",
                  icon: Icons.add,
                  action: () {
                    open_url.openUrl(
                        "steam://friends/add/${widget.player.steamid}");
                  },
                ),
                UserButton(
                  message: "Backpack.tf page",
                  icon: Icons.account_circle,
                  action: () {
                    open_url.openUrl(
                        "https://backpack.tf/profiles/${widget.player.steamid}");
                  },
                ),
                UserButton(
                  message: "Expand user",
                  icon: Icons.zoom_out_map,
                  action: () {
                    g.navigationService.navigateTo(
                      '/inventory',
                      arguments: {
                        "player": widget.player,
                        "items": widget.items,
                      },
                    );
                  },
                ),
                Expanded(
                  child: ListView.builder(
                    scrollDirection: Axis.horizontal,
                    itemCount: widget.items.length,
                    itemBuilder: (context, i) {
                      return InventoryItem(widget.items[i]);
                    },
                  ),
                ),
                UserButton(
                  message: "Remove user",
                  icon: Icons.delete_forever,
                  action: () {
                    g.users.removeWhere((element) =>
                        element.player.playerId == widget.player.playerId);
                    g.homeState();
                  },
                ),
              ],
            ),
          ),
          Container(
            height: 50,
            child: Row(
              children: [
                InfoCard(
                  message:
                      "${(widget.player.inventory.scrap / 9 * 100).floorToDouble() / 100} Ref",
                  tooltipMessage: "Tradable refined in inventory",
                ),
                InfoCard(
                  message: "${widget.player.inventory.keys} Keys",
                  tooltipMessage: "Tradable keys in inventory",
                ),
                InfoCard(
                  message:
                      "${widget.player.hours == null ? "Private" : widget.player.hours.toString() + " Hrs"}",
                  tooltipMessage: "Hours",
                ),
                InfoCard(
                  message:
                      "${widget.player.level == null ? "Private" : "Level " + widget.player.level.toString()}",
                  tooltipMessage: "Level",
                ),
                InfoCard(
                  message:
                      "${widget.player.histories == null ? "Unknown" : widget.player.histories.toString() + " States"}",
                  tooltipMessage: "Saved history states on backpack.tf",
                ),
              ],
            ),
          )
        ],
      ),
      margin: EdgeInsets.only(bottom: 10),
    );
  }
}

class CustomTooltip extends StatelessWidget {
  final String message;
  final Widget child;
  CustomTooltip({this.message, this.child});
  @override
  Widget build(BuildContext context) {
    return Tooltip(
      message: message,
      decoration: BoxDecoration(
        color: Color.fromRGBO(0, 0, 0, 0.8),
        borderRadius: BorderRadius.circular(5),
      ),
      padding: EdgeInsets.all(5),
      textStyle: TextStyle(color: g.white80, fontSize: 15, height: 1),
      preferBelow: false,
      child: child,
    );
  }
}

class InventoryItem extends StatelessWidget {
  final Item item;
  InventoryItem(this.item);
  @override
  Widget build(BuildContext context) {
    return Container(
      height: 65,
      width: 65,
      decoration: BoxDecoration(
        color: g.qualities[item.quality2 ?? item.quality]["color"],
        borderRadius: BorderRadius.only(
          topLeft: Radius.circular(10),
          topRight: Radius.circular(10),
          bottomLeft: Radius.circular(15),
          bottomRight: Radius.circular(15),
        ),
      ),
      margin: EdgeInsets.all(5),
      child: Stack(
        children: [
          Container(
            height: 40,
            decoration: BoxDecoration(
              color: g.qualities[item.quality]["color"],
              borderRadius: BorderRadius.only(
                topLeft: Radius.circular(10),
                topRight: Radius.circular(10),
              ),
            ),
            margin: EdgeInsets.only(top: 5, bottom: 20),
          ),
          if (item.effectImage != null)
            FadeInImage(
              placeholder: MemoryImage(k.kTransparentImage),
              image: NetworkImage(item.effectImage),
            ),
          Tooltip(
            message: item.nameComplete,
            decoration: BoxDecoration(
              color: Color.fromRGBO(0, 0, 0, 0.8),
              borderRadius: BorderRadius.circular(5),
            ),
            padding: EdgeInsets.all(5),
            textStyle: TextStyle(color: g.white80, fontSize: 15, height: 1),
            preferBelow: false,
            child: FadeInImage(
              placeholder: MemoryImage(k.kTransparentImage),
              image: NetworkImage(item.image),
            ),
          ),
          Container(
            height: 20,
            margin: EdgeInsets.only(top: 45),
            decoration: BoxDecoration(
              color: g.primary,
              borderRadius: BorderRadius.only(
                bottomLeft: Radius.circular(10),
                bottomRight: Radius.circular(10),
              ),
            ),
            child: Center(
              child: Text(
                item.displayPrice,
                style: TextStyle(
                  color: g.white80,
                  fontSize: 10,
                  height: 1,
                  fontWeight: FontWeight.w600,
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class InventoryItemWide extends StatelessWidget {
  final Item item;
  InventoryItemWide(this.item);
  @override
  Widget build(BuildContext context) {
    return Container(
      height: 75,
      margin: EdgeInsets.only(bottom: 5),
      color: g.primaryDark,
      child: Row(
        children: [
          Container(
            height: 65,
            width: 65,
            decoration: BoxDecoration(
              color: g.qualities[item.quality2 ?? item.quality]["color"],
              borderRadius: BorderRadius.circular(10),
            ),
            margin: EdgeInsets.only(right: 5),
            child: Stack(
              children: [
                Container(
                  height: 60,
                  decoration: BoxDecoration(
                    color: g.qualities[item.quality]["color"],
                    borderRadius: BorderRadius.circular(10),
                  ),
                  margin: EdgeInsets.only(top: 5),
                ),
                if (item.effectImage != null)
                  FadeInImage(
                    placeholder: MemoryImage(k.kTransparentImage),
                    image: NetworkImage(item.effectImage),
                  ),
                Tooltip(
                  message: item.nameComplete,
                  decoration: BoxDecoration(
                    color: Color.fromRGBO(0, 0, 0, 0.8),
                    borderRadius: BorderRadius.circular(5),
                  ),
                  padding: EdgeInsets.all(5),
                  textStyle:
                      TextStyle(color: g.white80, fontSize: 15, height: 1),
                  preferBelow: false,
                  child: FadeInImage(
                    placeholder: MemoryImage(k.kTransparentImage),
                    image: NetworkImage(item.image),
                  ),
                ),
              ],
            ),
          ),
          UserButton(
            icon: Icons.more,
            message: "Backpack.tf listings",
            action: () {},
          ),
          InfoCard(
            message: item.displayPrice,
            tooltipMessage: "Item price",
          ),
          InfoCard(
            message: item.nameComplete,
            tooltipMessage: "Item name",
          ),
        ],
      ),
    );
  }
}
