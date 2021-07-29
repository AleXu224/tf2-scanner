import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

import '../globals.dart';

class LoadingCard extends StatefulWidget {
  final LoadingCardController controller;
  final String name;
  const LoadingCard({Key? key, required this.controller, required this.name}) : super(key: key);

  @override
  State<LoadingCard> createState() => _LoadingCardState();
}

class _LoadingCardState extends State<LoadingCard> {
  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.only(top: 8),
      height: 32,
      decoration: BoxDecoration(
        color: ThemeColors.p,
        borderRadius: BorderRadius.circular(4),
      ),
      child: Row(
        children: [
          Expanded(child: Container()),
          Padding(
            padding: EdgeInsets.only(right: 8),
            child: SizedBox(
              height: 16,
              width: 16,
              child: widget.controller.isLoading
                  ? CircularProgressIndicator(
                      color: ThemeColors.s,
                    )
                  : Icon(
                      Icons.done,
                      size: 20,
                      color: ThemeColors.t,
                    ),
            ),
          ),
          Text(
            widget.name,
            style: TextStyle(
              color: ThemeColors.t,
              fontSize: 14,
            ),
          ),
          Expanded(child: Container()),
        ],
      ),
    );
  }
}
