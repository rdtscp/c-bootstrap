
void swap(char * &t1, char * &t2) {
  char *temp = t1;
  t1 = t2;
  t2 = temp;
}

void swap(unsigned int &t1, unsigned int &t2) {
  unsigned int temp = t1;
  t1 = t2;
  t2 = temp;
}
