#include "Player.hpp"
#include "Inventory.hpp"

Player::Player(JsonPlayer::Player &player) : inventory(this) {
    steamid = player.steamid;
    name = player.personaname;
    avatarUrl = player.avatarmedium;
    visibility = player.communityvisibilitystate;
}

void Player::ToConsole() {
    using namespace ImGui;
    if (TreeNode((void*)(intptr_t)this, "Name %s", name.c_str())) {
        Text("SteamID: %s", steamid.c_str());
        Text("Name: %s", name.c_str());
        Text("Avatar: %s", avatarUrl.c_str());
        Text("Visibility: %d", visibility);
        Text("Level: %d", level);
        Text("Histories: %d", histories);
        Text("Hours: %d", hours);
        inventory.ToConsole();
        TreePop();
    }
}
