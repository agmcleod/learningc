#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("ERROR: You need one argument\n");
    // abort the program
    return 1;
  }

  for(int i = 0; i < argv[1][i] != '\0'; i++) {
    char letter = tolower(argv[1][i]);

    switch(letter) {
      case 'a':
        printf("%d: 'A'\n", i);
        break;
      case 'e':
        printf("%d: 'E'\n", i);
        break;
      case 'i':
        printf("%d: 'I'\n", i);
        break;
      case 'o':
        printf("%d: 'O'\n", i);
        break;
      case 'u':
        printf("%d: 'U'\n", i);
        break;
      case 'y':
        if(i > 2) {
          // it's only sometimes Y
          printf("%d: 'Y'\n", i);
        }
        break;
      default:
        printf("%d: %c is not a vowel\n", i, letter);
    }
  }

  return 0;
}