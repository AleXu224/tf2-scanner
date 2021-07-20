class Skins {
  Map<String, String> skin = new Map();
  Skins(Map<String, dynamic> data) {
    data.forEach((key, value) {
      this.skin[key] = value.toString();
    });
  }
}
