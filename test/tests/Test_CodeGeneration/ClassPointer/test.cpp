void printf(const char *, int);

class Point {
public:
  int x;
  int y;

  ~Point() {}
};

int main(int argc, char **argv) {
  Point point;

  Point *point_ptr = &point;
  point_ptr->x = 5;
  point_ptr->y = 10;

  printf("x, expecting 5:  %d\n", point.x);
  printf("y, expecting 10: %d\n", point.y);

  const bool correct = (point.x == 5) && (point.y == 10);
  if (!correct) {
    return 1;
  }

  return 0;
}
