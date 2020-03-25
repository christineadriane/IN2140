#ifndef MY_SHARED_HEADER
#define MY_SHARED_HEADER

/* includes */
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

/* declarations */
void read();
void read_connections();
struct router* find_router();
void free_memory();
void set_model();
void add_connection();
void delete();
void print();

#endif
