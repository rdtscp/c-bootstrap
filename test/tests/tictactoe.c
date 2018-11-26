// Tic-tac-toe game
// Written by Daniel Hillerström

#include "minic-stdlib.h"

void print_s(char *s) {}

void print_i(int i) {}

void print_c(char c) {}

char read_c() { return 'c'; }

int read_i() { return 0; }

void *mcmalloc(int size) {
  void *out;
  return out;
}

// Board layout
char a11;
char a12;
char a13;
char a21;
char a22;
char a23;
char a31;
char a32;
char a33;
char empty; // Empty cell character

// Resets the board
void reset() {
  a11 = empty;
  a12 = empty;
  a13 = empty;
  a21 = empty;
  a22 = empty;
  a23 = empty;
  a31 = empty;
  a32 = empty;
  a33 = empty;
}

// Predicate indicating whether all board cells are filled.
// 1 => true
// 0 => false
int full() {
  int n;
  n = 0;
  if (a11 != empty)
    n = n + 1;
  if (a21 != empty)
    n = n + 1;
  if (a31 != empty)
    n = n + 1;

  if (a12 != empty)
    n = n + 1;
  if (a22 != empty)
    n = n + 1;
  if (a32 != empty)
    n = n + 1;

  if (a13 != empty)
    n = n + 1;
  if (a23 != empty)
    n = n + 1;
  if (a33 != empty)
    n = n + 1;

  if (n == 9)
    return 1;
  else
    return 0;
}

// Attempts to put mark in cell (row,col)
// Returns 1 on success, otherwise -1 is returned when the cell is already
// occupied, and 0 when the move is not within the board range.
int set(char row, int col, char mark) {
  int r;
  r = 1;
  if (row == 'a') {
    if (col == 1) {
      if (a11 == empty)
        a11 = mark;
      else
        r = -1;
    } else {
      if (col == 2) {
        if (a12 == empty)
          a12 = mark;
        else
          r = -1;
      } else {
        if (col == 3) {
          if (a13 == empty)
            a13 = mark;
          else
            r = -1;
        } else {
          r = 0;
        }
      }
    }
  } else {
    if (row == 'b') {
      if (col == 1) {
        if (a21 == empty)
          a21 = mark;
        else
          r = -1;
      } else {
        if (col == 2) {
          if (a22 == empty)
            a22 = mark;
          else
            r = -1;
        } else {
          if (col == 3) {
            if (a23 == empty)
              a23 = mark;
            else
              r = -1;
          } else {
            r = 0;
          }
        }
      }
    } else {
      if (row == 'c') {
        if (col == 1) {
          if (a31 == empty)
            a31 = mark;
          else
            r = -1;
        } else {
          if (col == 2) {
            if (a32 == empty)
              a32 = mark;
            else
              r = -1;
          } else {
            if (col == 3) {
              if (a33 == empty)
                a33 = mark;
              else
                r = -1;
            } else {
              r = 0;
            }
          }
        }
      } else {
        r = 0;
      }
    }
  }
  return r;
}

// Prints the game board to stdout
void printGame() {
  print_s((char *)"\n");
  print_s((char *)"     1   2   3\n");
  print_s((char *)"   +---+---+---+\n");
  print_s((char *)"a  | ");
  print_c(a11);
  print_s((char *)" | ");
  print_c(a12);
  print_s((char *)" | ");
  print_c(a13);
  print_s((char *)" |\n");
  print_s((char *)"   +---+---+---+\n");
  print_s((char *)"b  | ");
  print_c(a21);
  print_s((char *)" | ");
  print_c(a22);
  print_s((char *)" | ");
  print_c(a23);
  print_s((char *)" |\n");
  print_s((char *)"   +---+---+---+\n");
  print_s((char *)"c  | ");
  print_c(a31);
  print_s((char *)" | ");
  print_c(a32);
  print_s((char *)" | ");
  print_c(a33);
  print_s((char *)" |\n");
  print_s((char *)"   +---+---+---+\n");
  print_s((char *)"\n");
}

void printWinner(int player) {
  print_s((char *)"Player ");
  print_i(player);
  print_s((char *)" has won!\n");
}

int switchPlayer(int currentPlayer) {
  if (currentPlayer == 1)
    return 2;
  else
    return 1;
}

// Gets the mark for player 1 or 2
char get_mark(int player) {
  if (player == 1)
    return 'X';
  else
    return 'O';
}

// Asks the current player to select his/her move.
void selectmove(int player) {
  char row;
  int col;
  int selected;
  int success;
  char mark;
  selected = 1;
  while (selected) {
    print_s((char *)"Player ");
    print_i(player);
    print_s((char *)" select move (e.g. a2)>");
    row = read_c();
    col = read_i();

    mark = get_mark(player);
    success = set(row, col, mark);
    if (success == 0) {
      print_s((char *)"That is not a valid move!\n");
    } else {
      if (success == -1)
        print_s((char *)"That move is not possible!\n");
      else
        selected = 0;
    }
  }
}

// Determines whether anybody has won.
// 0 => nobody has won yet
// 1 => there is a winner
int won(char mark) {
  int r;
  r = 0;
  if (a11 == mark) {
    if (a21 == mark) {
      if (a31 == mark) {
        r = 1;
      }
    } else {
      if (a22 == mark) {
        if (a33 == mark) {
          r = 1;
        }
      } else {
        if (a12 == mark) {
          if (a13 == mark) {
            r = 1;
          }
        }
      }
    }
  }

  if (a12 == mark) {
    if (a22 == mark) {
      if (a32 == mark) {
        r = 1;
      }
    }
  }

  if (a13 == mark) {
    if (a23 == mark) {
      if (a33 == mark) {
        r = 1;
      }
    } else {
      if (a22 == mark) {
        if (a31 == mark) {
          r = 1;
        }
      }
    }
  }

  if (a21 == mark) {
    if (a22 == mark) {
      if (a23 == mark) {
        r = 1;
      }
    }
  }

  if (a31 == mark) {
    if (a32 == mark) {
      if (a33 == mark) {
        r = 1;
      }
    }
  }
  return r;
}

// Main entry point
void main() {
  int playing;
  int player;
  char mark;
  char yesno;

  empty = ' ';
  playing = 1;
  reset();
  printGame();
  player = 1; // Begin with player one
  while (playing) {
    selectmove(player);
    mark = get_mark(player);
    printGame();
    if (won(mark)) { // player won
      printWinner(player);
      playing = 0;
    } else if (full() == 1) { // game board is full, it is a draw
      print_s((char *)"It's a draw!\n");
      playing = 0;
    } else {
      player = switchPlayer(player); // give the turn to the opponent
    }

    if (playing == 0) {
      print_s((char *)"Play again? (y/n)> ");
      yesno = read_c();
      if (yesno == 'y') {
        playing = 1;
        reset();
      } else {
        if (yesno == 'Y') {
          playing = 1;
          reset();
        }
      }
    }
  }
}