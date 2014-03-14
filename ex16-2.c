#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
  char *name;
  int age;
  int height;
  int width;
};

struct Person Person_create(char *name, int age, int height, int width) {
  struct Person who;

  // play with this, remove the strdup
  who.name = name;
  who.age = age;
  who.height = height;
  who.width = width;

  return who;
}

void Person_print(struct Person who) {
  printf("Name: %s\n", who.name);
  printf("\tAge: %d\n", who.age);
  printf("\tHeight: %d\n", who.height);
  printf("\tWidth: %d\n", who.width);
}

int main(int argc, char *argv[]) {
  // make two people structures
  struct Person joe = Person_create("Joe Smith", 32, 64, 140);
  struct Person frank = Person_create("Frank Blank", 20, 72, 180);

  // print them out and where they are in memory
  printf("Joe is at the memory location: %p\n", &joe);
  Person_print(joe);

  printf("Frank is at the memory location: %p\n", &frank);
  Person_print(frank);

  joe.age += 20;
  joe.height -= 2;
  joe.width += 40;

  frank.age += 20;
  frank.width += 20;
  Person_print(frank);

  return 0;
}