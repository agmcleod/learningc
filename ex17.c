#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
  int id;
  int set;
  char *name;
  char *email;
};

struct Database {
  int MAX_DATA;
  int MAX_ROWS;
  struct Address **rows; // USE ARRAY OF POINTERS
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void die(const char *message) {
  if(errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
  size_t i=0;

  // Each database will have two `int` values. read
  // those first.
  assert(conn->db && conn->file);
  if (!(conn->db && conn->file))
      die("Database load : Invalid Connection info");
  if (fread(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file) != 1)
      die("Database load : Couldn't read MAX_DATA");
  if (fread(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file) != 1)
      die("Database load : Couldn't read MAX_ROWS");
  conn->db->rows = (struct Address**)malloc(sizeof(struct Address *) * conn->db->MAX_ROWS);

  assert(conn->db->rows);
  if (!(conn->db->rows)) {
    die("Database_load : Could not MAX_ROWS Address  structures");
  }

  // Now load each address structure
  for (i=0; i<conn->db->MAX_ROWS; i++) {
    // read two int values first.

    conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
    struct Address *row = conn->db->rows[i];
    if (fread(&row->id, sizeof(row->id), 1, conn->file) !=1)
      die("Database load : Could not read Address::id");
    if (fread(&row->set, sizeof(row->set), 1, conn->file) != 1)
      die("Database load : Couldn't read  Address::set");
    // allocate two buffers large enough for our data
    row->name = malloc(conn->db->MAX_DATA * sizeof(*row->name));
    row->email = malloc(conn->db->MAX_DATA * sizeof(*row->email));
    assert(row->email && row->name);
    if (!(row->name && row->email))
      die("Database load : Failed to Allocate Address strings");
    // now just read both strings
    if (fread(row->name, conn->db->MAX_DATA * sizeof(*row->name), 1, conn->file) != 1)
      die("Database load : Failed to read Address::name");
    if (fread(row->email, conn->db->MAX_DATA * sizeof(*row->email), 1, conn->file) != 1)
      die("Database load : Failed to read Address::email");
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(!conn) die("Memory error");

  conn->db = malloc(sizeof(struct Database));
  if(!conn->db) die("Memory error");

  if(mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if(conn->file) {
      Database_load(conn);
    }
  }

  if(!conn->file) die("Failed to open the file");

  return conn;
}

void Database_close(struct Connection *conn) {
  size_t i;
  if(conn) {
    if (conn->db && conn->db->rows) {
      for (i = 0; i < conn->db->MAX_ROWS; i++) {
        struct Address *cur = conn->db->rows[i];
        free(cur);
      }
    }

    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(conn);
  }
}


void Database_write(struct Connection *conn) {
    size_t i = 0;
    rewind(conn->file);
    // Writing MAX_DATA and MAX_ROWS to the file first
    if(fwrite(&conn->db->MAX_DATA, sizeof(conn->db->MAX_DATA), 1, conn->file) != 1)
      die("Database write : Failed to write MAX_DATA");
    if(fwrite(&conn->db->MAX_ROWS, sizeof(conn->db->MAX_ROWS), 1, conn->file) != 1)
      die("Database write : Failed to write MAX_ROWS");
    // Writing Address's list now
    for (i=0; i<conn->db->MAX_ROWS; i++) {
      struct Address *row = conn->db->rows[i];
      if (fwrite(&row->id, sizeof(row->id), 1, conn->file) != 1)
          die("Database write : Failed to write Address(id)");
      if (fwrite(&row->set, sizeof(row->set), 1, conn->file) != 1)
          die("Database write : Failed to write Address(set)");
      if (fwrite(row->name, sizeof(char)*conn->db->MAX_DATA, 1, conn->file) != 1)
          die("Database write : Failed to write Address(name)");
      if (fwrite(row->email, sizeof(char)*conn->db->MAX_DATA, 1, conn->file) != 1)
          die("Database write : Failed to write Address(email)");
    }
    if (fflush(conn->file) == -1)
        die("Database_write : Cannot flush database.");
}

void Database_create(struct Connection *conn, int MAX_DATA, int MAX_ROWS) {
  int i = 0;
  conn->db->MAX_DATA = MAX_DATA;
  conn->db->MAX_ROWS = MAX_ROWS;
  conn->db->rows = (struct Address**)malloc(sizeof(struct Address*) * MAX_ROWS);

  for(i = 0; i < MAX_ROWS; i++) {
    conn->db->rows[i] = (struct Address*)malloc(sizeof(struct Address));
    conn->db->rows[i]->id = i;
    conn->db->rows[i]->set = 0;
    conn->db->rows[i]->name = (char *)malloc(conn->db->MAX_DATA);
    conn->db->rows[i]->name = (char *)memset(conn->db->rows[i]->name, ' ', conn->db->MAX_DATA);
    conn->db->rows[i]->email= (char *)malloc(conn->db->MAX_DATA);
    conn->db->rows[i]->email= (char *)memset(conn->db->rows[i]->email, ' ', conn->db->MAX_DATA);
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
  if (!(conn && conn->db && conn->db->rows && conn->db->rows[id])) return;

  struct Address *addr = conn->db->rows[id];
  int MAX_DATA = conn->db->MAX_DATA;
  if(addr->set == 1) die("Already set, delete it first");

  addr->set = 1;
  addr->name = malloc(sizeof(char) * MAX_DATA);
  addr->email = malloc(sizeof(char) * MAX_DATA);

  char *res = strncpy(addr->name, name, MAX_DATA);
  if(!res) die("Name copy failed");

  res = strncpy(addr->email, email, MAX_DATA);
  if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
  struct Address *addr = conn->db->rows[id];

  if(addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

void Database_delete(struct Connection *conn, int id) {
  struct Address addr = {.id = id, .set = 0};
  conn->db->rows[id] = &addr;
}

void Database_list(struct Connection *conn) {
  int i = 0;
  struct Database *db = conn->db;
  int MAX_ROWS = conn->db->MAX_ROWS;
  for(i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[]) {
  if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  struct Connection *conn = Database_open(filename, action);
  int id = 0;

  switch(action) {
    case 'c':
        if(argc < 5) die("Required: MAX_DATA, MAX_ROWS");
      Database_create(conn, atoi(argv[3]), atoi(argv[4]));
      Database_write(conn);
      break;

    case 'g':
      if(argc != 4) die("Need an id to get");

      Database_get(conn, atoi(argv[3]));
      break;

    case 's':
      if(argc != 6) die("Need id, name, email to set");
      Database_set(conn, atoi(argv[3]), argv[4], argv[5]);
      Database_write(conn);
      break;

    case 'd':
      if(argc != 4) die("Need id to delete");

      Database_delete(conn, id);
      Database_write(conn);
      break;

    case 'l':
      Database_list(conn);
      break;
    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
  }

  Database_close(conn);

  return 0;
}

