import 'package:flutter/material.dart';
import 'package:open_url/open_url.dart';
import 'globals.dart' as g;
import 'widgets.dart' as w;
import 'package:transparent_image/transparent_image.dart' as k;
import 'scanning.dart';

class Inventory extends StatefulWidget {
  final Player player;
  final List<Item> items;
  Inventory({@required this.player, @required this.items});
  @override
  _InventoryState createState() => _InventoryState();
}

class _InventoryState extends State<Inventory> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: g.primary,
      body: Row(
        children: [
          w.settingsSideBar(true),
          Expanded(
            child: Column(
              children: [
                Container(
                  height: 75,
                  color: g.primaryDark,
                  child: Row(
                    children: [
                      w.CustomTooltip(
                        message: widget.player.name,
                        child: GestureDetector(
                          onTap: () {
                            openUrl(
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
                      w.UserButton(
                        message: "Add friend",
                        icon: Icons.add,
                        action: () {
                          openUrl(
                              "steam://friends/add/${widget.player.steamid}");
                        },
                      ),
                      w.UserButton(
                        message: "Backpack.tf page",
                        icon: Icons.account_circle,
                        action: () {
                          openUrl(
                              "https://backpack.tf/profiles/${widget.player.steamid}");
                        },
                      ),
                    ],
                  ),
                ),
                Container(
                  height: 50,
                  color: g.primaryDark,
                  child: Row(
                    children: [
                      w.InfoCard(
                        message:
                            "${(widget.player.inventory.scrap / 9 * 100).floorToDouble() / 100} Ref",
                        tooltipMessage: "Refined",
                      ),
                      w.InfoCard(
                        message: "${widget.player.inventory.keys} Keys",
                        tooltipMessage: "Keys",
                      ),
                      w.InfoCard(
                        message:
                            "${widget.player.hours == null ? "Private" : widget.player.hours.toString() + " Hrs"}",
                        tooltipMessage: "Hours",
                      ),
                      w.InfoCard(
                        message:
                            "${widget.player.level == null ? "Private" : "Level " + widget.player.level.toString()}",
                        tooltipMessage: "Level",
                      ),
                      w.InfoCard(
                        message:
                            "${widget.player.histories == null ? "Unknown" : widget.player.histories.toString() + " States"}",
                        tooltipMessage: "Saved history states on backpack.tf",
                      ),
                    ],
                  ),
                ),
                Expanded(
                  child: Scrollbar(
                    child: ListView.builder(
                      itemCount: widget.items.length,
                      itemBuilder: (context, i) {
                        return w.InventoryItemWide(widget.items[i]);
                      },
                    ),
                  ),
                ),
              ],
            ),
          )
        ],
      ),
    );
  }
}
