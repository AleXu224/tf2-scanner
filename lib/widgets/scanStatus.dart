import 'package:flutter/material.dart';
import 'package:bpscanner/globals.dart';

class ScanStatus extends StatefulWidget {
  const ScanStatus({Key? key}) : super(key: key);

  @override
  _ScanStatusState createState() => _ScanStatusState();
}

class _ScanStatusState extends State<ScanStatus> {
  @override
  void initState() {
    super.initState();
    Controllers.scanStatus.redraw = () => setState(() {});
  }

  @override
  void dispose() {
    super.dispose();
    Controllers.scanStatus.redraw = () {};
  }

  @override
  Widget build(BuildContext context) {
    return Material(
      elevation: 8,
      color: Colors.transparent,
      child: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(8),
          color: ThemeColors.s,
        ),
        padding: const EdgeInsets.all(8),
        child: Text(
          Controllers.scanStatus.status,
          style: TextStyle(
            color: ThemeColors.p,
            fontSize: 14,
          ),
        ),
      ),
    );
  }
}
