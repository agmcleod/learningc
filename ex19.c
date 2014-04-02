#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ex19.h"

int Monster_attack(void *self, int damage) {
  Monster *monster = self;
  printf("You attack %s!\n", monster->_(description));

  monster->hit_points -= damage;

  if(monster->hit_points > 0) {
    printf("It is still alive.\n");
    return 0;
  }
  else {
    printf("It is dead!\n");
    return 1;
  }
}

int Monster_init(void *self) {
  Monster *monster = self;
  monster->hit_points = 10;
  return 1;
}

Object MonsterProto = {
  .init = Monster_init,
  .attack = Monster_attack
};

void *Room_move(void *self, Direction direction) {
  Room *room = self;
  Room *next = NULL;

  if(direction == NORTH && room->north) {
    printf("You go north, into:\n");
    next = room->north;
  }
  else if(direction == SOUTH && room->south) {
    printf("You go south, into:\n");;
    next = room->south;
  }
  else if(direction == EAST && room->east) {
    printf("You go east, into:\n");
    next = room->east;
  }
  else if(direction == WEST && room->west) {
    printf("You go west, into:\n");
    next = room->west;
  }
  else {
    printf("You can't go that direction.\n");
    next = NULL;
  }

  if(next) {
    next->_(describe)(next);
  }

  return next;
}

int Room_attack(void *self, int damage) {
  Room *room = self;
  Monster *monster = room->bad_guy;

  if(monster) {
    monster->_(attack)(monster, damage);
    return 1;
  }
  else {
    printf("You fail in the air at nothing.\n");
    return 0;
  }
}

Object RoomProto = {
  .move = Room_move,
  .attack = Room_attack
}