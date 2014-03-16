#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_MAX_DATA 512
#define DEFAULT_MAX_ROWS 100

struct Address {
  int id;
  int set;
  char *name;
  char *email;
};

struct Database {
  int max_rows;
  int max_data;
  struct Address *rows;
};

struct Connection {
  FILE *file;
  struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message) {
  if(errno) {
    perror(message);
  }
  else {
    printf("ERROR: %s\n", message);;
  }
  exit(1);
}

void die_and_close(const char *message, struct Connection *conn) {
  Database_close(conn);
  die(message);
}

void Address_print(struct Address *addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
  if(fread(conn->db, sizeof(struct Database), 1, conn->file) != 1) {
    die("Failed to load database.");
  }
}

struct Connection *Database_open(const char *filename, char mode, const int max_rows, const int max_data) {
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(!conn) die_and_close("Memory error", conn);

  conn->db = malloc(sizeof(struct Database));
  if(!conn->db) die_and_close("Memory Error", conn);

  conn->db->max_rows = max_rows;
  conn->db->max_data = max_data;

  if(mode == 'c') {
    conn->file = fopen(filename, "w");
  }
  else {
    conn->file = fopen(filename, "r+");
    if(conn->file) {
      Database_load(conn);
    }
  }

  if(!conn->file) die_and_close("Failed to open the file", conn);

  return conn;
}

void Database_close(struct Connection *conn) {
  if(conn) {
    if(conn->file) fclose(conn->file);
    if(conn->db) {
      free(conn->db->rows);
      free(conn->db);
    }
    free(conn);
  }
}

void Database_write(struct Connection *conn) {
  rewind(conn->file);
  int rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
  if(rc != 1) die_and_close("Failed to write database.", conn);

  /* if(fwrite(&conn->db->max_data, sizeof(char) * conn->db->max_data, 1, conn->file) != 1) {
    die("Failed to write database.");
  }
  if(fwrite(&conn->db->max_rows, sizeof(int) * conn->db->max_rows, 1, conn->file) != 1) {
    die("Failed to write database.");
  } */

  rc = fwrite(conn->db, sizeof(conn->db), 1, conn->file);
  if(rc != 1) die("Failed to write database.");

  rc = fflush(conn->file);
  if(rc == -1) die_and_close("Cannot flush database", conn);
}

void Database_create(struct Connection *conn) {
  conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));

  int field_size = conn->db->max_data * sizeof(char);

  for(int i = 0; i < conn->db->max_rows; i++) {
    struct Address addr;
    addr.id = i;
    addr.set = 0;

    addr.name = malloc(field_size);
    if (!addr.name) die("Memory error: new address malloc name.");
    addr.name[0] = '\0';

    addr.email = malloc(field_size);
    if (!addr.email) die("Memory error: new address malloc email.");
    addr.email[0] = '\0';
    memcpy(conn->db->rows +i, &addr, sizeof(addr) );

    free(addr.name);
    free(addr.email);
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
  struct Address *addr = &conn->db->rows[id];
  if(addr->set) {
    printf("ID: %d", id);
    die_and_close("Already set, delete it first.", conn);
  }
  addr->set = 1;
  char *res = strncpy(addr->name, name, conn->db->max_data);

  if(!res) die_and_close("Name copy failed", conn);

  int l = strlen(addr->name);
  addr->name[l] = '\0';

  res = strncpy(addr->email, email, conn->db->max_data);
  if(!res) die_and_close("Email copy failed", conn);

  l = strlen(addr->email);
  addr->email[l] = '\0';
}

void Database_get(struct Connection *conn, int id) {
  struct Address *addr = &conn->db->rows[id];
  if(addr->set) {
    Address_print(addr);
  }
  else {
    die_and_close("ID is not set", conn);
  }
}

void Database_delete(struct Connection *conn, int id) {
  struct Address addr = { .id = id, .set = 0};
  conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
  struct Database *db = conn->db;

  for(int i = 0; i < conn->db->max_rows; i++) {
    struct Address *cur = &db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[]) {
  if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];

  int max_rows = DEFAULT_MAX_ROWS;
  int max_data = DEFAULT_MAX_DATA;
  int id = 0;
  if(argc > 3 && action != 'c') {
    id = atoi(argv[3]);
  }
  else if(argc > 3 && action == 'c') {
    max_rows = atoi(argv[3]);
    max_data = atoi(argv[4]);
  }

  struct Connection *conn = Database_open(filename, action, max_rows, max_data);


  if(id >= conn->db->max_rows) die("There's not that many records");

  switch(action) {
    case 'c':
      Database_create(conn);

      Database_write(conn);
      break;
    case 'g':
      if(argc != 4) die("Need an id to get");

      Database_get(conn, id);
      break;
    case 's':
      if(argc != 6) die("Need id, name, email to set");

      Database_set(conn, id, argv[4], argv[5]);
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
      die("Invalid action. Only: c=create, g=get, s=set, d=del, l=list");
  }

  Database_close(conn);

  return 0;
}