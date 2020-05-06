import 'package:flutter/material.dart';
import 'globals.dart' as g;
import 'widgets.dart' as w;

class ScanInfo extends StatefulWidget {
  @override
  _ScanInfoState createState() => _ScanInfoState();
}

class _ScanInfoState extends State<ScanInfo> {
  Color inputColor = g.primary;
  FocusNode _focus = new FocusNode();

  bool isGroupScan = g.groupScan;
  double inputHeight = 120;
  String placeholderText = "User list";
  int maxLines;

  BoxBorder idleBorder =
      Border(top: BorderSide(color: g.primaryDark, width: 1));

  BoxBorder activeBorder = Border(
    top: BorderSide(color: g.secondary, width: 1),
    left: BorderSide(color: g.secondary, width: 1),
  );

  BoxBorder currentBorder =
      Border(top: BorderSide(color: g.primaryDark, width: 1));

  @override
  void initState() {
    super.initState();
    if (isGroupScan) {
      setState(() {
        inputHeight = 36;
        placeholderText = "Group link";
        maxLines = 1;
      });
    }
    _focus.addListener(() {
      if (_focus.hasFocus) {
        setState(() {
          if (!isGroupScan) inputHeight = 300;
          currentBorder = activeBorder;
          inputColor = g.primaryDark;
        });
      } else {
        setState(() {
          if (!isGroupScan) inputHeight = 120;
          currentBorder = idleBorder;
          inputColor = g.primary;
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: g.primary,
        resizeToAvoidBottomInset: false,
        body: GestureDetector(
          onTap: () {
            FocusScopeNode currentFocus = FocusScope.of(context);
            if (!currentFocus.hasPrimaryFocus) {
              currentFocus.unfocus();
            }
          },
          child: Row(
            children: [
              w.scanInfoSideBar(),
              Expanded(
                child: Column(
                  children: [
                    w.optionList(
                      child: Column(
                        children: [
                          w.titleText("Scan settings"),
                          w.subtitleText(
                              "Leave the fields empty for their default value"),
                          w.ValueInput(
                            text: "Max refined in user's inventory",
                            textController: g.maxRef,
                          ),
                          w.ValueInput(
                            text: "Max keys in user's inventory",
                            textController: g.maxKeys,
                          ),
                          w.ValueInput(
                            text: "Minimum item price (refined)",
                            textController: g.minRef,
                          ),
                          w.ValueInput(
                            text: "Minimum item price (keys)",
                            textController: g.minKeys,
                          ),
                          w.ValueInput(
                            text: "Max saved history states",
                            textController: g.maxHistory,
                          ),
                          w.ValueInput(
                            text: "Max hours",
                            textController: g.maxHours,
                          ),
                          w.SwitchInput(
                            text: "Show items that are untradable",
                            value: g.untradable,
                            setter: (newVal) {
                              g.untradable = newVal;
                            },
                          ),
                          w.SwitchInput(
                            text: "Show items without a value",
                            value: g.noValue,
                            setter: (newVal) {
                              g.noValue = newVal;
                            },
                          ),
                          w.SwitchInput(
                            text: "Show weapon skins",
                            value: g.skins,
                            setter: (newVal) {
                              g.skins = newVal;
                            },
                          ),
                          w.titleText("Group settings"),
                          w.SwitchInput(
                            text: "Group scan",
                            value: g.groupScan,
                            setter: (newVal) {
                              g.groupScan = newVal;
                              isGroupScan = newVal;
                              if (isGroupScan) {
                                setState(() {
                                  inputHeight = 36;
                                  placeholderText = "Group link";
                                  maxLines = 1;
                                });
                              } else {
                                setState(() {
                                  if (_focus.hasFocus)
                                    inputHeight = 300;
                                  else
                                    inputHeight = 120;
                                  placeholderText = "User list";
                                  maxLines = null;
                                });
                              }
                            },
                          ),
                          w.ValueInput(
                            text: "Pages to scan",
                            textController: g.pagesScan,
                          ),
                          w.ValueInput(
                            text: "Pages to skip",
                            textController: g.pagesSkip,
                          ),
                        ],
                      ),
                    ),
                    MouseRegion(
                      onEnter: (event) {
                        if (_focus.hasFocus) return;
                        setState(() {
                          inputColor = g.primaryDarkMid;
                        });
                      },
                      onExit: (event) {
                        if (_focus.hasFocus) return;
                        setState(() {
                          inputColor = g.primary;
                        });
                      },
                      child: AnimatedContainer(
                        duration: Duration(milliseconds: 200),
                        height: inputHeight,
                        decoration: BoxDecoration(
                          color: inputColor,
                          border: currentBorder,
                        ),
                        child: TextField(
                          controller: g.inputList,
                          maxLines: maxLines,
                          focusNode: _focus,
                          style: TextStyle(
                            fontSize: 15,
                            height: 1,
                            color: g.white80,
                          ),
                          decoration: InputDecoration(
                              border: InputBorder.none,
                              contentPadding: EdgeInsets.all(10),
                              hintText: placeholderText,
                              hintStyle: TextStyle(
                                  fontSize: 15, height: 1, color: g.white20)),
                        ),
                      ),
                    )
                  ],
                ),
              )
            ],
          ),
        ));
  }
}
