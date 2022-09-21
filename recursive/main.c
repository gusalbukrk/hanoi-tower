#include <stdio.h>
#include "stack.h"

// utility to clear input buffer
void flush() {
  char c;
  while((c = getchar()) != '\n' && c != EOF) {}
}

typedef struct TowersOfHanoi {
  stack *A;
  stack *B;
  stack *C;
} towers;

int twoToThePowerOf(int p) {
  if (p == 1) return 2;
  return 2 * twoToThePowerOf(p - 1);
}
//
int calcMinNumberOfMoves(int piecesQuant) {
  // return pow(2, piecesQuant) - 1; // pow is not portable enough

  return twoToThePowerOf(piecesQuant) - 1;
}

stack **getTowerAddress(towers *t, char column) {
  switch (column) {
    case 'A':
      return &t->A;
    case 'B':
      return &t->B;
    case 'C':
      return &t->C;
    default:
      printf("ERROR: invalid input.\n");
      exit(1);
  }
}

// e.g. narrowDownAux(A, C) = 'B'
char narrowDownAux(char o, char d) {
  int sum = o + d;

  if (sum == 'A' + 'B') return 'C';
  if (sum == 'B' + 'C') return 'A';
  return 'B';
}

towers *createTowers(int piecesQuant) {
  towers *t = malloc(sizeof(towers));

  t->A = init();
  t->B = init();
  t->C = init();

  return t;
}

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
    printf("    %-2s  %-2s  %-2s\n",
      printElement(t->A, i, piecesQuant),
      printElement(t->B, i, piecesQuant),
      printElement(t->C, i, piecesQuant)
    );
  }
  
  printf("\n");
}

char getTowerName(stack *s, towers *t) {
  if (s == t->A) return 'A';
  if (s == t->B) return 'B';
  return 'C';
}

void move(towers *t, stack **src, stack **dest) {
  printf("      MOVE %c, %c\n", getTowerName(*src, t), getTowerName(*dest, t));
  push(dest, pop(src));
}

int counter = 0;

void recursiveHanoi(towers *t, int n, stack **src, stack **dest, stack **aux) {

  printf("    %d. %c, %c, %c\n", ++counter, getTowerName(*src, t), getTowerName(*dest, t), getTowerName(*aux, t));

  if (n == 1) {
    move(t, src, dest);
    return;
  }

  recursiveHanoi(t, n - 1, src, aux, dest);
  move(t, src, dest);
  recursiveHanoi(t, --n, aux, dest, src);
}

int main() {
  int piecesQuant = 25;
  printf("Total number of pieces: "); scanf("%d", &piecesQuant); flush();
  //
  if (piecesQuant < 2) {
    printf("ERROR: 2 is the minimal size allowed.\n");
    return 1;
  }

  towers *t = createTowers(piecesQuant);

  // source
  char o; printf("Source (A, B or C): "); scanf("%c", &o); flush();
  stack **src = getTowerAddress(t, o);
  for (int i = piecesQuant; i > 0; i--) push(src, i); // populate source column

  // destination
  char d; printf("Destination (A, B or C): "); scanf("%c", &d); flush();
  stack **dest = getTowerAddress(t, d);

  // auxilary
  char a = narrowDownAux(o, d);
  stack **aux = getTowerAddress(t, a);

  // printf("%c, %c, %c\n", o, d, a);

  printf("\n"); printf("INPUT:\n"); printTowers(t, piecesQuant);
  printf("CALLS (%d):\n", calcMinNumberOfMoves(piecesQuant)); recursiveHanoi(t, piecesQuant, src, dest, aux);
  printf("\n"); printf("OUTPUT:\n"); printTowers(t, piecesQuant);
}
