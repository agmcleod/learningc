#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

struct Person {
  int id;
  char *name;
  struct Person *gaveTo;
};

struct Person *findById(struct Person people[], int id) {
  struct Person *person;
  int len = sizeof(people) / sizeof(people[0]);
  for(int i = 0; i < len; i++) {
    if(people[i].id == id) {
      person = &people[i];
    }
  }

  return person;
}

int main() {
  srand(time(NULL));
  struct Person oddList[] = {
    { 1, "Cersei Lannister", NULL },
    { 3, "Bran Stark", NULL },
    { 5, "Tyrion Lannister", NULL }
  };

  struct Person evenList[] = {
    { 2, "Jon Snow", NULL },
    { 4, "Daenarys Targaryan", NULL }
  };

  int oddListLength = sizeof(oddList) / sizeof(oddList[0]);
  int evenListLength = sizeof(evenList) / sizeof(evenList[0]);

  for(int i = 0 ; i < evenListLength; i++) {
    struct Person *p = &evenList[i];

  }

  return 0;
}