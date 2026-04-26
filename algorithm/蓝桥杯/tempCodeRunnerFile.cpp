int main() {
  int y = 2000, m = 1, d = 1;
  int days = 0;
  while (y != 2020 && m != 10 && d != 1) {
    nextday(y, m, d);
    days++;
  }
  return 0;
}