void printf(const char *, const bool);
void printf(const char *, const int);
void printf(const char *, const unsigned int);

bool test_assert(const char *log, const bool actual, const bool expect) {
  printf(log, actual);
  if (actual == expect) {
    return true;
  }
  return false;
}

bool test_assert(const char *log, const int actual, const int expect) {
  printf(log, actual);
  if (actual == expect) {
    return true;
  }
  return false;
}

bool test_assert(const char *log, const unsigned int actual, const unsigned int expect) {
  printf(log, actual);
  if (actual == expect) {
    return true;
  }
  return false;
}

int main(int argc, const char **argv) {
  const int failure = 1;
  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;

  const bool true_val = true;
  const bool false_val = !true_val;

  if (false_val == true) {
    return failure;
  }

  if (!test_assert("one < two == %d\n", one < two, true)) {
    return failure;
  }

  if (!test_assert("two < three == %d\n", two < three, true)) {
    return failure;
  }

  if (!test_assert("three < one == %d\n", three < one, false)) {
    return failure;
  }
  if (!test_assert("three > two == %d\n", three > two, true)) {
    return failure;
  }
  if (!test_assert("two > one == %d\n", two > one, true)) {
    return failure;
  }
  if (!test_assert("one > three == %d\n", one > three, false)) {
    return failure;
  }
  if (!test_assert("two >= one == %d\n", two >= one, true)) {
    return failure;
  }
  if (!test_assert("two >= two == %d\n", two >= two, true)) {
    return failure;
  }
  if (!test_assert("two >= three == %d\n", two >= three, false)) {
    return failure;
  }

  if (!test_assert("two <= one == %d\n", two <= one, false)) {
    return failure;
  }
  if (!test_assert("two <= two == %d\n", two <= two, true)) {
    return failure;
  }
  if (!test_assert("two <= three == %d\n", two <= three, true)) {
    return failure;
  }

  if (!test_assert("three mod two == %d\n", three % two, 1u)) {
    return failure;
  }

  if (!test_assert("four mod two == %d\n", four % two, 0u)) {
    return failure;
  }

  if (!test_assert("four - two == %d\n", four - two, 2)) {
    return failure;
  }

  

  return 0;
}
