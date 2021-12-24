import 'package:bpscanner/globals.dart';
import 'package:flutter/material.dart';
import 'buttons.dart';
import 'misc.dart';
import 'dart:math';
import 'dart:convert';
import 'package:cached_network_image/cached_network_image.dart';

class UserContainer extends StatelessWidget {
  UserContainer({Key? key}) : super(key: key);

  final String id =
      base64Encode(List<int>.generate(32, (index) => Random().nextInt(255)));

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.only(top: 8),
      height: 136,
      color: ThemeColors.p,
      child: Column(
        children: [
          // Top half
          Container(
            height: 96,
            padding: EdgeInsets.all(16),
            child: Row(
              children: [
                UserAvatar(
                  avatarUrl: "https://via.placeholder.com/150",
                  name: "Florinel",
                ),
                UserButtons(),
                ItemsContainer(),
                Padding(
                  padding: const EdgeInsets.only(
                    left: 16,
                    right: 0,
                    top: 16,
                    bottom: 16,
                  ),
                  child: UserButton(Icons.close, "Remove user", () {
                    App.users = App.users.where((e) => e.id != id).toList();
                    States.setState();
                  }),
                ),
              ],
            ),
          ),
          // Bottom half
          Container(
            height: 40,
            child: Padding(
              padding: const EdgeInsets.only(
                right: 16,
                left: 16,
              ),
              child: Row(
                children: [
                  InfoCard(
                    text: "1 Banana",
                  ),
                  InfoCard(
                    text: "2 Banane",
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}

class InfoCard extends StatelessWidget {
  final String text;
  const InfoCard({Key? key, required this.text}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: BoxDecoration(
        borderRadius: BorderRadius.all(Radius.circular(4)),
        color: ThemeColors.pL,
      ),
      height: 24,
      padding: EdgeInsets.only(
        left: 8,
        right: 8,
      ),
      margin: EdgeInsets.only(
        right: 8,
      ),
      child: Center(
        child: Text(
          text,
          style: TextStyle(
            color: ThemeColors.t,
            fontSize: 12,
          ),
        ),
      ),
    );
  }
}

class UserAvatar extends StatelessWidget {
  final String avatarUrl;
  final String name;
  const UserAvatar({Key? key, required this.avatarUrl, required this.name})
      : super(key: key);

  @override
  Widget build(BuildContext context) {
    return CustomTooltip(
      message: name,
      child: ClipRRect(
        borderRadius: BorderRadius.circular(16),
        child: Container(
          height: 64,
          width: 64,
          color: ThemeColors.s,
          child: CachedNetworkImage(
            imageUrl: avatarUrl,
            progressIndicatorBuilder: (context, url, downloadProgress) {
              return SizedBox(
                child: Center(
                  child: CircularProgressIndicator(
                    value: downloadProgress.progress,
                    color: ThemeColors.p,
                  ),
                ),
                height: 64,
                width: 64,
              );
            },
            errorWidget: (contxt, url, error) {
              return Icon(Icons.error);
            },
          ),
        ),
      ),
    );
  }
}

class UserButtons extends StatelessWidget {
  const UserButtons({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: EdgeInsets.only(top: 16, bottom: 16),
      margin: EdgeInsets.only(left: 16, right: 16),
      height: 64,
      width: 112,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          UserButton(Icons.add, "Add User", () {}),
          UserButton(Icons.person, "Backpack.tf Profile", () {}),
          UserButton(Icons.block, "Idk", () {}),
        ],
      ),
    );
  }
}

class ItemsContainer extends StatelessWidget {
  const ItemsContainer({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Expanded(
      child: Container(
        height: 64,
        child: ListView(
          scrollDirection: Axis.horizontal,
          children: List.generate(50, (index) => TFItem()),
        ),
      ),
    );
  }
}

class TFItem extends StatelessWidget {
  const TFItem({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return CustomTooltip(
      message: "Item name goes here",
      child: Padding(
        padding: const EdgeInsets.only(right: 8),
        child: ClipRRect(
          borderRadius: BorderRadius.circular(8),
          child: Stack(
            children: [
              // Quality 1
              Container(
                color: ThemeColors.s,
                height: 48,
                width: 64,
              ),
              // Secondary quality if there is one
              Container(
                decoration: BoxDecoration(
                  color: Colors.green,
                  borderRadius: BorderRadius.only(
                    topLeft: Radius.circular(8),
                    topRight: Radius.circular(8),
                  ),
                ),
                height: 40,
                width: 64,
                margin: EdgeInsets.only(top: 8),
              ),
              // Item image
              CachedNetworkImage(
                imageUrl:
                    "https://community.akamai.steamstatic.com/economy/image/fWFc82js0fmoRAP-qOIPu5THSWqfSmTELLqcUywGkijVjZULUrsm1j-9xgEIUxMFWiTvuSB8j93oMv6NGucF1dsz4ZEHjWc4x1csMbG2aGdmK1KVAvUHWaJo8grtCiMwvMNhAYLior1IOVK4PATpep0/330x192",
                progressIndicatorBuilder: (context, url, downloadProgress) {
                  return SizedBox(
                    child: Center(
                      child: CircularProgressIndicator(
                        value: downloadProgress.progress,
                        color: ThemeColors.p,
                      ),
                    ),
                    height: 64,
                    width: 64,
                  );
                },
                errorWidget: (contxt, url, error) {
                  return Icon(Icons.error);
                },
              ),
              // Price
              Container(
                height: 16,
                width: 64,
                color: ThemeColors.pD,
                margin: EdgeInsets.only(top: 48),
                child: Center(
                  child: Text(
                    "Price",
                    style: TextStyle(
                      fontSize: 10,
                      color: ThemeColors.t,
                      height: 1.2,
                    ),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
