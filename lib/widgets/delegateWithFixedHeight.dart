import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'dart:math' as math;

class SliverGridDelegateWithMaxCrossAxisExtentFixedHeight
    extends SliverGridDelegate {
  /// Creates a delegate that makes grid layouts with tiles that have a maximum
  /// cross-axis extent.
  ///
  /// All of the arguments except [mainAxisExtent] must not be null.
  /// The [maxCrossAxisExtent], [mainAxisExtent], [mainAxisSpacing],
  /// and [crossAxisSpacing] arguments must not be negative.
  /// The [childAspectRatio] argument must be greater than zero.
  const SliverGridDelegateWithMaxCrossAxisExtentFixedHeight({
    required this.maxCrossAxisExtent,
    this.mainAxisSpacing = 0.0,
    this.crossAxisSpacing = 0.0,
    this.childAspectRatio = 1.0,
    this.height = 64,
    this.mainAxisExtent,
  });

  /// The maximum extent of tiles in the cross axis.
  ///
  /// This delegate will select a cross-axis extent for the tiles that is as
  /// large as possible subject to the following conditions:
  ///
  ///  - The extent evenly divides the cross-axis extent of the grid.
  ///  - The extent is at most [maxCrossAxisExtent].
  ///
  /// For example, if the grid is vertical, the grid is 500.0 pixels wide, and
  /// [maxCrossAxisExtent] is 150.0, this delegate will create a grid with 4
  /// columns that are 125.0 pixels wide.
  final double maxCrossAxisExtent;

  /// The number of logical pixels between each child along the main axis.
  final double mainAxisSpacing;

  /// The number of logical pixels between each child along the cross axis.
  final double crossAxisSpacing;

  /// The ratio of the cross-axis to the main-axis extent of each child.
  final double childAspectRatio;

  final double height;

  /// The extent of each tile in the main axis. If provided it would define the
  /// logical pixels taken by each tile in the main-axis.
  ///
  /// If null, [childAspectRatio] is used instead.
  final double? mainAxisExtent;

  bool _debugAssertIsValid(double crossAxisExtent) {
    assert(crossAxisExtent > 0.0);
    assert(maxCrossAxisExtent > 0.0);
    assert(mainAxisSpacing >= 0.0);
    assert(crossAxisSpacing >= 0.0);
    assert(childAspectRatio > 0.0);
    assert(height > 0.0);
    return true;
  }

  @override
  SliverGridLayout getLayout(SliverConstraints constraints) {
    assert(_debugAssertIsValid(constraints.crossAxisExtent));
    final int crossAxisCount =
        (constraints.crossAxisExtent / (maxCrossAxisExtent + crossAxisSpacing))
            .ceil();
    final double usableCrossAxisExtent = math.max(0.0,
        constraints.crossAxisExtent - crossAxisSpacing * (crossAxisCount - 1));
    final double childCrossAxisExtent = usableCrossAxisExtent / crossAxisCount;
    final double childMainAxisExtent = height;
    return SliverGridRegularTileLayout(
      crossAxisCount: crossAxisCount,
      mainAxisStride: childMainAxisExtent + mainAxisSpacing,
      crossAxisStride: childCrossAxisExtent + crossAxisSpacing,
      childMainAxisExtent: childMainAxisExtent,
      childCrossAxisExtent: childCrossAxisExtent,
      reverseCrossAxis: axisDirectionIsReversed(constraints.crossAxisDirection),
    );
  }

  @override
  bool shouldRelayout(
      SliverGridDelegateWithMaxCrossAxisExtentFixedHeight oldDelegate) {
    return oldDelegate.maxCrossAxisExtent != maxCrossAxisExtent ||
        oldDelegate.mainAxisSpacing != mainAxisSpacing ||
        oldDelegate.crossAxisSpacing != crossAxisSpacing ||
        oldDelegate.childAspectRatio != childAspectRatio ||
        oldDelegate.mainAxisExtent != mainAxisExtent;
  }
}
