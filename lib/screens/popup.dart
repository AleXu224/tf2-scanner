import 'package:bpscanner/widgets/textInput.dart';
import 'package:bpscanner/widgets/buttons.dart';
import 'package:bpscanner/widgets/loadingCard.dart';
import 'package:bpscanner/utilities.dart';
import 'package:flutter/material.dart';

import '../globals.dart';

class PopupScreen extends StatefulWidget {
  const PopupScreen({Key? key}) : super(key: key);

  @override
  _PopupScreenState createState() => _PopupScreenState();
}

class _PopupScreenState extends State<PopupScreen> {
  @override
  Widget build(BuildContext context) {
    return Visibility(
      visible: Scanner.showPopup,
      child: Scanner.popupScreen == 0 ? ApiKeyInput() : LoadingScreen(),
    );
  }
}

class LoadingScreen extends StatefulWidget {
  const LoadingScreen({Key? key}) : super(key: key);

  @override
  State<LoadingScreen> createState() => _LoadingScreenState();
}

class _LoadingScreenState extends State<LoadingScreen> {
  @override
  Widget build(BuildContext context) {
    return Container(
      color: ThemeColors.pD.withOpacity(0.75),
      child: Column(
        children: [
          Expanded(child: Container()),
          Container(
            child: CircularProgressIndicator(
              color: ThemeColors.s,
              value: Scanner.loadingData.length / 4,
            ),
          ),
          Expanded(
            child: ConstrainedBox(
              constraints: BoxConstraints(maxWidth: 304),
              child: ListView.builder(
                itemCount: Scanner.loadingData.length,
                itemBuilder: (context, i) {
                  return Scanner.loadingData[i];
                },
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class ApiKeyInput extends StatelessWidget {
  const ApiKeyInput({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      color: ThemeColors.pD.withOpacity(0.75),
      child: Center(
        child: ConstrainedBox(
          constraints: BoxConstraints(maxWidth: 304),
          child: Container(
            height: 135,
            padding: EdgeInsets.only(bottom: 16),
            decoration: BoxDecoration(
              color: ThemeColors.p,
              borderRadius: BorderRadius.circular(4),
            ),
            child: Column(
              children: [
                TextInput(
                  controller: Controllers.apiKey,
                  labelText: "Api Key",
                ),
                OptionsButton(
                  name: "Save",
                  action: () {
                    // Check if api key looks valid
                    Controllers.apiKey.controller.text = Controllers.apiKey.controller.text.replaceAll(" ", "");
                    if (Controllers.apiKey.controller.text.length != 32) {
                      showSnack(context, "Api key is invalid.");
                      return;
                    }

                    Scanner.config.saveApiKey();
                  },
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
