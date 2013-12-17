// I skipped a few exercises in between because 

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
    unsigned int max_data;
    unsigned int max_rows;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

// Fnc prototype for Database_close
void Database_close(struct Connection *conn);

unsigned int 
Database_size(unsigned int max_rows, unsigned int max_data)
{
    unsigned int ret;
    
    // Contained address
    ret = sizeof(int) * 2;
    ret += sizeof(char) * max_data * 2;
    
    // Database
    ret *= max_rows;
    ret += sizeof(unsigned int) * 2;

    return ret;
}

void 
die(const char *message, struct Connection *conn)
{
	if(errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

    if(conn)
        Database_close(conn);

	exit(1);
}

struct Database *
Database_alloc(unsigned int max_rows, unsigned int max_data)
{
    int i;
    struct Database *db;

    db = malloc(sizeof(struct Database));
    if(!db) die("Memory error", NULL);
    memset(db, 0, sizeof(struct Database));
    
    db->rows = malloc(sizeof(struct Address) * max_rows);
    if(!(db->rows)) die("Memory error", NULL);
    memset(db->rows, 0, sizeof(struct Address) * max_rows);
    
    for(i = 0; i < max_rows; i++)
    {
        db->rows[i].name = (char *) malloc(sizeof(char) * max_data);
        if(!(db->rows[i].name)) die("Memory error", NULL);
        memset(db->rows[i].name, 0, sizeof(char) * max_data);
        
        db->rows[i].email = malloc(sizeof(char) * max_data);
        if(!(db->rows[i].email)) die("Memory error", NULL);
        memset(db->rows[i].email, 0, sizeof(char) * max_data);
    }

    return db;
}

void
Database_dealloc(struct Database *db)
{
    int i;
    
    if(db)
    {
        if(db->rows)
        {
            for(i = 0; i < db->max_rows; i++)
            {
                if(db->rows[i].name)
                    free(db->rows[i].name);
                if(db->rows[i].email)
                    free(db->rows[i].email);
            }
            free(db->rows);
        }
        free(db);
    } 
}

void 
Address_print(struct Address *addr)
{
	printf("%d %s %s\n", 
		    addr->id, addr->name, addr->email);
}

void 
Database_load(struct Connection *conn)
{
    int i;    

    struct Sizes {
        unsigned int max_data;
        unsigned int max_rows;
    } sizes;
    
    int rc = fread(&sizes, sizeof(struct Sizes), 1, conn->file);
    
    if(rc != 1) die("Failed to get database size.", conn);

    rewind(conn->file);
    
    conn->db = Database_alloc(sizes.max_rows, sizes.max_data);
    
    rc = fread(conn->db, 2 * sizeof(unsigned int), 1, conn->file);
    
    for(i = 0; i < sizes.max_rows; i++)
    {
        rc &= fread(&(conn->db->rows[i]), 2 * sizeof(int), 1, conn->file);
        rc &= fread(conn->db->rows[i].name, sizeof(char) * sizes.max_data, 1, conn->file);
        rc &= fread(conn->db->rows[i].email, sizeof(char) * sizes.max_data, 1, conn->file);
    }
    
	if(rc != 1) die("Failed to load database.", conn);
}

struct Connection *
Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);

    if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		
		if(conn->file) {
			Database_load(conn);
		}
	}

	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

void 
Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file) fclose(conn->file);
        Database_dealloc(conn->db); 	
		free(conn);
	}
}

void 
Database_write(struct Connection *conn)
{
    int i;

    rewind(conn->file);

    int rc = fwrite(conn->db, 2 * sizeof(unsigned int), 1, conn->file);
    
    for(i = 0; i < conn->db->max_rows; i++)
    {
        rc &= fwrite(&(conn->db->rows[i]), 2 * sizeof(int), 1, conn->file);
        memset(conn->db->rows[i].name, 0, sizeof(char) * conn->db->max_data);
        rc &= fwrite(conn->db->rows[i].name, sizeof(char) * conn->db->max_data, 1, conn->file);
        rc &= fwrite(conn->db->rows[i].email, sizeof(char) * conn->db->max_data, 1, conn->file);
    }

    if(rc != 1) die("Failed to write database.", conn);

    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void 
Database_create(struct Connection *conn, unsigned int max_rows, unsigned int max_data)
{
	int i = 0;

    conn->db = Database_alloc(max_rows, max_data);

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
	
	for(i = 0; i < max_rows; i++) {
		conn->db->rows[i].id = i;
        conn->db->rows[i].set = 0;
	}
}

void 
Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    
    if(id >= conn->db->max_rows)
        die("There's not that many records.", conn);    
   
	if(conn->db->rows[id].set) die("Already set, delete it first", conn);

    conn->db->rows[id].set = 1;	
	// WARNING: bug, read the "How To Break It" and fix this
	strncpy(conn->db->rows[id].name, name, conn->db->max_data);

    if(conn->db->max_data > 0)
        conn->db->rows[id].name[conn->db->max_data - 1] = '\0';
	// demonstrate the strncpy bug

	strncpy(conn->db->rows[id].email, email, conn->db->max_data);
}

void 
Database_get(struct Connection *conn, int id)
{
    if(id >= conn->db->max_rows)
        die("There's not that many records.", conn);    
    
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void 
Database_delete(struct Connection *conn, int id)
{
    if(id >= conn->db->max_rows)
        die("There's not that many records.", conn);    
    
    conn->db->rows[id].id = id;
    conn->db->rows[id].set = 0;
}

void 
Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    // if(id >= MAX_ROWS) die("There's not that many records.");

    switch(action) {
        case 'c':
            if(argc != 5) die("Need max rows and max data", conn);

            Database_create(conn, atoi(argv[3]), atoi(argv[4]));
            Database_write(conn);
            break;
        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;
        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;
        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);
            
    return 0;
}
