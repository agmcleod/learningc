#include <stdio.h>

int main(int argc, char *argv[]) {
  int distance = 100;
  float power = 2.345f;
  double super_power = 56789.4532;
  char initial = 'G';
  char first_name[] = "Aaron";
  char last_name[] = "McLeod";

  printf("You are %d miles away.\n", distance);
  printf("You have %f levels of power.\n", power);
  printf("You have %f awesome super powers", super_power);
  printf("I have an initial %c.\n", initial);
  printf("I have a first name %s.\n", first_name);
  printf("I have a last name %s.\n", last_name);
  printf("My whole name is %s %c. %s.\n", first_name, initial, last_name);
  printf("A hex value %a\n", -0xc.90fep-2);

  return 0;
}