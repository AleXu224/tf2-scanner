class User {
  // steamid:                  string;
  late String steamid;
  // communityvisibilitystate: number;
  late int communityvisibilitystate;
  // profilestate:             number;
  late int profilestate;
  // personaname:              string;
  late String personaname;
  // profileurl:               string;
  late String profileurl;
  // avatarmedium:             string;
  late String avatarmedium;
  // personastate:             number;
  late int personastate;
  User(Map<String, dynamic> data) {
    steamid = data["steamid"];
    communityvisibilitystate = data["communityvisibilitystate"];
    profilestate = data["profilestate"];
    personaname = data["personaname"];
    profileurl = data["profileurl"];
    avatarmedium = data["avatarmedium"];
    personastate = data["personastate"];
  }
}
