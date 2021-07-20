class Effects {
  Map<String, int> effect = new Map();
  Effects(Map<String, dynamic> data) {
    data.forEach((key, value) {
      this.effect[key] = int.parse(value.toString());
    });
  }
}
