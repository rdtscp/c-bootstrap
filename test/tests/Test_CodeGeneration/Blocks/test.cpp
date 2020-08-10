int main(int argc, char **argv) {

  const int x = 5;
  const int y = 10;

  const bool correct = (x == 5) && (y == 10);
  if (correct) {
    return 0;
  }

  return 1;
}
