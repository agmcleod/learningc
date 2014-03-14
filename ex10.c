#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int i = 0;

  // go through each string in argv
  // skip 0 to not output the program name (./ex10)
  for(i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  char *states[] = {
    "California",
    "Oregon",
    "Washington",
    "Texas"
  };

  int num_of_states = sizeof(states) / sizeof(states[0]);

  for(i = 0; i < num_of_states; i++) {
    char *state = states[i];
    printf("state: %d: %s\n", i, state);
    int word_len = strlen(state);
    for(int j = 0; j < word_len; j++) {
      printf("Character %c for index %i\n", state[j], j);
    }
  }


  return 0;
}