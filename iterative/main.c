#include <stdio.h>
#include "stack.h"

typedef struct TowersOfHanoi {
  stack *A;
  stack *B;
  stack *C;
} towers;

stack **getStackAddress(towers *t, char column) {
  switch (column) {
    case 'A':
      return &(t->A);
    case 'B':
      return &(t->B);
    default: // 'C
      return &(t->C);
  }
}

// return position of the smallest argument
int minOf3 (int x, int y, int z) {
  int xIsZero = x == 0;
  int yIsZero = y == 0;
  int zIsZero = z == 0;

  if (!xIsZero && !yIsZero && !zIsZero) {
    if ( (x < y) && (x < z) ) return 1;
    if ( (y < x) && (y < z) ) return 2;
    return 3;
  }

  if (xIsZero && !yIsZero && !zIsZero) {
    if ( y < z ) return 2;
    else return 3;
  }

  if (!xIsZero && yIsZero && !zIsZero) {
    if ( x < z ) return 1;
    else return 3;
  }

  if (!xIsZero && !yIsZero && zIsZero) {
    if ( x < y ) return 1;
    else return 2;
  }

  if (!xIsZero && yIsZero && zIsZero) {
    return 1;
  }

  if (xIsZero && !yIsZero && zIsZero) {
    return 2;
  }

  // if (xIsZero && yIsZero && !zIsZero) 
  return 3;
}

void createTowers(towers **t, int piecesQuant) {
  // *t = calloc(3, sizeof(stack));
  *t = malloc(sizeof(towers));

  init(&(*t)->A);
  init(&(*t)->B);
  init(&(*t)->C);

  // initialize column A
  for (int i = piecesQuant - 1; i >= 0; i--) {
    push(( &(*t)->A ), i + 1);
  }
}

// // if column isn't full, empty space will be at the bottom
// char *printElement(stack *s, int i, int size) {
//   // print this when element is empty
//   if (s->size == 0 || i >= s->size) return "ˣ";
  
//   // convert int to string
//   char *n = malloc(sizeof(char) * 10);
//   sprintf(n, "%d", s->stack[i]);
//   return n;
// }

// if column isn't full, empty space will be at the top
char *printElement(stack *s, int i, int piecesQuant) {
  int x = piecesQuant - s->size;

  if (i >= x) {
    char *n = malloc(sizeof(char) * 10);
    sprintf(n, "%d", s->stack[i - x]);
    return n;    
  }

  return "x"; // print this when element is empty
}

void printTowers(towers *t, int piecesQuant) {
  for (int i = 0; i < piecesQuant; i++) {    
    printf("%s  %s  %s\n",
      printElement(t->A, i, piecesQuant),
      printElement(t->B, i, piecesQuant),
      printElement(t->C, i, piecesQuant)
    );
  }
  
  printf("\n");
}

// 2ⁿ - 1 = minimal number of moves required to solve a Tower of Hanoi formula
// every result possible is a Mersenne number without primality requirements
// also, every Mersenne number is odd; e.g. (pieces quant vs minimal moves):
// 2 = 3, 3 = 7, 4 = 15, 5 = 31, 6 = 63, 7 = 127, 8 = 255
int twoToThePowerOf(int p) {
  if (p == 1) return 2;
  return 2 * twoToThePowerOf(p - 1);
}
//
int calcMinNumberOfMoves(int piecesQuant) {
  // return pow(2, piecesQuant) - 1; // pow is not portable enough

  return twoToThePowerOf(piecesQuant) - 1;
}

// ITERATIVE SOLUTION with minimal number of moves
// alternate moves between the smallest piece and a non-smallest piece
// move smallest piece to the right if number total of pieces is even
// otherwise, move to the left
// if no position in the chosen direction, move the piece to the opposite end
// but then continue to move in the correct direction
// when the turn is to move the non-smallest piece, there is only one legal move
void iterativeHanoi(towers **t, int piecesQuant, int numberOfMovesLeft) {

  stack **A = getStackAddress(*t, 'A');
  stack **B = getStackAddress(*t, 'B');
  stack **C = getStackAddress(*t, 'C');
  
  if (numberOfMovesLeft % 2 == 1) { // if odd, move smallest
    
    char column; // column with smallest top
    
    // switch (m) {
    switch (minOf3((*t)->A->stack[0], (*t)->B->stack[0], (*t)->C->stack[0])) {
      case 1:
        column = 'A';
        break;
      case 2:
        column ='B';
        break;
      case 3:
        column = 'C';
        break;
    }

    // pop element from stack with smallest top
    stack **s = getStackAddress(*t, column);
    int v = pop(s);

    if (piecesQuant % 2 == 0) { // if size is even, move to the right

      if (column == 'C') {  // if already in the rightmost column
        push(A, v);
      } else {
        if (column == 'A') {
          push(B, v);
        } else { // if column is B
          push(C, v);
        }
      }

    } else { // if size is odd, move to the left

      if (column == 'A') { // if already in the leftmost column
        push(C, v);
      } else {
        if (column == 'C') {
          push(B, v);
        } else { // if column is B
          push(A, v);
        }
      }

    }

  } else { // if even, move the second smallest atop of the largest
    
    int topA = peek((*t)->A);
    int topB = peek((*t)->B);
    int topC = peek((*t)->C);

    char smallestTopCol;
    
    switch (minOf3(topA, topB, topC)) {
      case 1:
        smallestTopCol = 'A';
        break;
      case 2:
        smallestTopCol ='B';
        break;
      case 3:
        smallestTopCol = 'C';
        break;
    }

    // inside each case, find column with the mid number
    // and move its top piece to the column with highest top (or empty)
    switch (smallestTopCol) {

      case 'A':
        if (topC == 0 || topB < topC) { // move topB to C
          int v = pop(B);
          push(C, v);
        } else { // move topC to topB
          int v = pop(C);
          push(B, v);
        }
        break;
      
      case 'B':
        if (topC == 0 || topA < topC) { // move A to C
          int v = pop(A);
          push(C, v);
        } else { // move C to A
          int v = pop(C);
          push(A, v);
        }
        break;
      
      default: // 'C'
        if (topA == 0 || (topB != 0 && topB < topA) ) { // move B to A
          int v = pop(B);
          push(A, v);
        } else { // move A to B
          int v = pop(A);
          push(B, v);
        }

    }
  }

  printf("iterativeHanoi(**t, size = %d, numberOfMovesLeft = %d);\n", piecesQuant, numberOfMovesLeft);
  printTowers(*t, piecesQuant);

  if (numberOfMovesLeft - 1 == 0) return;
  iterativeHanoi(t, piecesQuant, numberOfMovesLeft - 1);
}

int main() {
  int piecesQuant = 5;
  printf("Total number of pieces: "); scanf("%d", &piecesQuant); printf("\n");

  if (piecesQuant < 2) {
    printf("ERROR: 2 is the minimal size allowed.\n");
    return 1;
  }

  towers *t;
  
  createTowers(&t, piecesQuant);

  printf("main();\n"); printTowers(t, piecesQuant); // initial towers print

  iterativeHanoi(&t, piecesQuant, calcMinNumberOfMoves(piecesQuant));
}
