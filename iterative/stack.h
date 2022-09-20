#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
  int size;
  int stack[]; // flexible array member
} stack; 

void printStack(stack *s) {
  printf("{ ");

  for (int i = 0; i < s->size; i++) {
    printf("%d, ", s->stack[i]);
  }

  printf("}\n");
}

int peek(stack *s) {
  return s->size == 0 ? 0 : s->stack[0];
}

void push (stack **s, int v) {
  int newSize = *s == NULL ? 1 : (*s)->size + 1;
  
  *s = realloc(*s, sizeof(stack) + ( sizeof(int) * newSize ));

  // move every element one position to the right
  for (int i = newSize - 1; i > 0; i--) {
    (*s)->stack[i] = (*s)->stack[i - 1];
  }

  (*s)->stack[0] = v;  

  (*s)->size = newSize;
}

int pop(stack **s) {
  if (*s == NULL || (*s)->size == 0) return -1;

  int r = (*s)->stack[0];

  // move every element one position to the left
  for (int i = 0; i < (*s)->size - 1; i++) {
    (*s)->stack[i] = (*s)->stack[i + 1];
  }

  *s = realloc(*s, sizeof(stack) + ( sizeof(int) * --(*s)->size ));

  return r;
}

void init(stack **s) {
  *s = malloc(sizeof(stack)); // `calloc` instead of `malloc` to initialize `s->size` to 0
  (*s)->size = 0;
}
