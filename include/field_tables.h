#ifndef FIELD_TABLES_H
#define FIELD_TABLES_H

#include <stdlib.h>
#include <stdio.h>
typedef enum {
	DT_FLOAT,
	DT_DOUBLE,
	DT_INT,
	DT_LONG,
	DT_SSIZE_T,
	DT_CSTRING,
} DATATYPE;

typedef struct Field_t {
	char *title; 			// what is displayed in the title row <-- must fit within width
	bool left_justify;
	void *rows;			// contiguous array of size = width*rowcount bytes
	ssize_t row_count;
	ssize_t width;			// internal size (B) of the values
	char *fmt;
	ssize_t max_print_width;
	DATATYPE FIELD_TYPE;
	bool warned;
} Field_t;

typedef struct FieldTable_t {
	Field_t *fields;
	char name[256];
	ssize_t column_count;
} FieldTable_t;

FieldTable_t new_FieldTable(char* table_name);
Field_t *new_Field(const char* field_title, DATATYPE FIELD_TYPE, const char* fmt, bool left_justify,  ssize_t max_print_width);
void ADD_FIELD_ENTRY(Field_t* field, void* row_data);
void assign_Field(FieldTable_t* table_ptr, Field_t *field_ptr);
void print_FieldTable(FieldTable_t table);

/* use compound literal to grab the void* of an rvalue */
#define add_Field_entry(field_ptr, val) ADD_FIELD_ENTRY(field_ptr, (&(typeof(val)) { val }))

#endif //FIELD_TABLES_H
