#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = argc-1;
  while(i > 0) {
    printf("arg %d: %s\n", i, argv[i]);
    i--;
  }

  char *states[] = {
    "California",
    "Oregon",
    "Washington",
    "Texas"
  };

  int num_of_states = 4;
  i = 0;

  while(i < num_of_states) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  return 0;
}