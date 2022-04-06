import 'package:flutter/rendering.dart';
import 'package:flutter/widgets.dart';

class Unfocuser extends StatelessWidget {
  const Unfocuser({Key? key, required this.child}) : super(key: key);

  final Widget child;

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: (){
        FocusScopeNode currentFocus = FocusScope.of(context);
        if (!currentFocus.hasPrimaryFocus) {
          currentFocus.unfocus();
        }
      },
      child: child,
    );
  }
}

class IgnoreUnfocuser extends SingleChildRenderObjectWidget {
  IgnoreUnfocuser({required this.child}) : super(child: child);

  final Widget child;

  @override
  IgnoreUnfocuserRenderBox createRenderObject(BuildContext context) =>
      IgnoreUnfocuserRenderBox();
}

class IgnoreUnfocuserRenderBox extends RenderPointerListener {}
