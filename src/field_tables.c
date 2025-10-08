#include <string.h>
#define NUM_FIELDS 		128
#define NUM_ROWS		128
#define WARNING_STR_SIZE 	256
#include <stdio.h>
#include <stdlib.h>

#include "field_tables.h"

// *INDENT-OFF*
bool field_row_chk_truncation(char* to_truncate, Field_t field, ssize_t row) {
	char buf[128];
	if (row >= field.row_count) {
		strcpy(buf, "N/A");
	} else {
		switch (field.FIELD_TYPE) {
		case DT_FLOAT: sprintf(buf, field.fmt, *(float*)(field.rows + (row * field.width))); break;
		case DT_DOUBLE: sprintf(buf, field.fmt, *(double*)(field.rows + (row * field.width))); break;
		case DT_INT: sprintf(buf, field.fmt, *(int*)(field.rows + (row * field.width))); break;
		case DT_LONG: sprintf(buf, field.fmt, *(long*)(field.rows + (row * field.width))); break;
		case DT_CSTRING: sprintf(buf, field.fmt, (char*)(field.rows + (row * field.width))); break;
		case DT_SSIZE_T: sprintf(buf, field.fmt, *(ssize_t*)(field.rows + (row * field.width))); break;
		default: perror("Attempted to print unknown field type."); break;
		}
	}
	if (field.left_justify) {
		sprintf(to_truncate, "%-*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	} else {
		sprintf(to_truncate, "%*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	}
	return strlen(to_truncate) < strlen(buf);
}

bool field_title_chk_truncation(Field_t field) {
	char buf[128];
	if (field.left_justify) {
		sprintf(buf, "%-*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	} else {
		sprintf(buf, "%*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	}
	return strlen(buf) < strlen(field.title);
}

Field_t *new_Field(const char* field_title, DATATYPE FIELD_TYPE, const char* fmt, bool left_justify,  ssize_t max_print_width) {
	Field_t *field = malloc(sizeof(Field_t));
	if(!field){
		perror("field, malloc");
		free(field);
		return NULL;
	}
	if (max_print_width == 0) { // default
		max_print_width = strlen(field_title);
	}

	ssize_t width = 0;
	switch (FIELD_TYPE) {
	case DT_FLOAT: 		width = sizeof(float);		break;
	case DT_DOUBLE: 	width = sizeof(double);		break;
	case DT_INT: 		width = sizeof(int);		break;
	case DT_LONG: 		width = sizeof(long);		break;
	case DT_CSTRING:	width = max_print_width+1; 	break;
	case DT_SSIZE_T:	width = sizeof(ssize_t);	break;
	default: 		width = -1; 			break;	
	}


	*field = (Field_t) {
		.title = (char*)field_title,
		.rows = calloc(NUM_ROWS, width),
		.row_count = 0,
		.width = width,
		.fmt = (char*)fmt,
		.max_print_width = max_print_width,
		.FIELD_TYPE = FIELD_TYPE,
		.left_justify = left_justify,
		.warned = false,
	};

	if(!field->rows){
		perror("field->rows, calloc");
		free(field);
		return NULL;
	}
	#ifndef __SUPRESS_TRUNCATION_WARNINGS
		if (field_title_chk_truncation(*field)){
			fprintf(stderr, "Warning! Row %zu of field '%s', will be truncated!", field->row_count-1, field->title);
		if(!field->warned){
			fprintf(stderr, "If this is intended, suppress this error (and others), by defining __SUPPRESS_TRUNCATION_WARNINGS.\n");
			field->warned = true;
		}
		}
	#endif 
	return field;
}
void destroy_Field(Field_t* field_ptr){
	free(field_ptr->rows);
	free(field_ptr);
}
void print_field_title(Field_t field) {
	if (field.left_justify){
		printf("%-*.*s", (int)field.max_print_width, (int)field.max_print_width, field.title);
	} else {
		printf("%*.*s", (int)field.max_print_width, (int)field.max_print_width, field.title);
	}
}


void print_field_row(Field_t field, ssize_t row) {
//	printf("Printing row (%zu). Table info:\n",row);
//	printf("\ttitle: '%s'\n",field.title);
//	printf("\trow_count: %zu\n",field.row_count);
//	printf("\twidth: %zu\n",field.width);
//	printf("\tfmt: '%s'\n",field.fmt);
//	printf("\tmax_print_width: '%zu'\n",field.max_print_width);

	char buf[128];
	if (row>=field.row_count){
		strcpy(buf, "N/A");
	} else {
		switch (field.FIELD_TYPE) {
		case DT_FLOAT: 		sprintf(buf, field.fmt, *(float*)(field.rows + (row*field.width))); 	break;
		case DT_DOUBLE: 	sprintf(buf, field.fmt, *(double*)(field.rows + (row*field.width))); 	break;
		case DT_INT: 		sprintf(buf, field.fmt, *(int*)(field.rows + (row*field.width))); 	break;
		case DT_LONG: 		sprintf(buf, field.fmt, *(long*)(field.rows + (row*field.width))); 	break;
		case DT_CSTRING:	sprintf(buf, field.fmt, (char*)(field.rows + (row*field.width))); 	break;
		case DT_SSIZE_T:	sprintf(buf, field.fmt, *(ssize_t*)(field.rows + (row*field.width))); 	break;
		default: 		perror("Attempted to print unknown field type."); 			break;
		}
	}
	if (field.left_justify){
		printf("%-*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	} else {
		printf("%*.*s", (int)field.max_print_width, (int)field.max_print_width, buf);
	}
}
void ADD_FIELD_ENTRY(Field_t* field, void* entry_data) {
	ssize_t row = field->row_count;
	switch (field->FIELD_TYPE) {
	case DT_FLOAT:		((float*)field->rows)[row] = *(float*)entry_data; 	break;
	case DT_DOUBLE:		((double*)field->rows)[row] = *(double*)entry_data; 	break;
	case DT_INT:		((int*)field->rows)[row] = *(int*)entry_data; 		break;
	case DT_LONG:		((long*)field->rows)[row] = *(long*)entry_data; 		break;
	case DT_CSTRING:	strcpy(&((char*)field->rows)[(row)*field->width],  (char*)entry_data); 		break;
	case DT_SSIZE_T:	((ssize_t*)field->rows)[row] = *(ssize_t*)entry_data; 	break;
	default: 		perror("Attempted to write unknown field type."); 		break;
	}
	field->row_count++;
	#ifndef __SUPRESS_TRUNCATION_WARNINGS
		char field_row_test[256];
		if (field_row_chk_truncation(field_row_test, *field, row)){
			fprintf(stdout, "Warning! Row %zu of field '%s', |", row,field->title);
			print_field_row(*field, row);
			fprintf(stdout, "| will be truncated!\n");
		if (!field->warned){
			fprintf(stdout, "If this is intended, suppress this error (and others), by defining:\n __SUPPRESS_TRUNCATION_WARNINGS.\n");
			field->warned = true;
		}
		}
	#endif 
}

FieldTable_t new_FieldTable(char* table_name){
	FieldTable_t ft = (FieldTable_t){
		.fields = calloc(NUM_FIELDS, sizeof(Field_t)),
		.column_count = 0,
	};
	strcpy(ft.name, table_name);
	return ft;
}

void destroy_FieldTable(FieldTable_t table){
	for (int i = 0; i<table.column_count; i++){
		free(table.fields[i].rows);
	}
	free(table.fields);
}

void assign_Field(FieldTable_t* table_ptr, Field_t *field_ptr){
	table_ptr->fields[table_ptr->column_count++] = *field_ptr;
}

void print_FieldTable(FieldTable_t table){
	ssize_t longest_field_row_count = 0;
	for (int x = 0; x<table.column_count; x++){
		int width = table.fields[x].max_print_width;
		int row_count = table.fields[x].row_count;
		if (x==0) printf("|");
		print_field_title(table.fields[x]);
		printf("|");
		if (row_count>longest_field_row_count){
			longest_field_row_count = row_count;
		}
	}
	printf("\n");
	for (int y = 0; y<longest_field_row_count; y++){
		for (int x = 0; x<table.column_count; x++){
			if (x==0) printf("|");
			print_field_row(table.fields[x], y);
			printf("|");
		}
		printf("\n");
	}

	// table has 'fields', field has rows.
}

// *INDENT-ON*

void example() {
	FieldTable_t table = new_FieldTable("Test Table");

	Field_t *f = new_Field("Total Time (ms)", DT_DOUBLE, "%0.2lf", false,  0);
	double d1 = 22;
	double d2 = 23.0;
	double d3 = 24.31238213;
	double d4 = 25.3323;
	ADD_FIELD_ENTRY(f, &d1);
	ADD_FIELD_ENTRY(f, &d2);
	ADD_FIELD_ENTRY(f, &d3);
	ADD_FIELD_ENTRY(f, &d4);

	assign_Field(&table, f);


	Field_t *s = new_Field("Names", DT_CSTRING, "%s", true, 20);
	char *s1 = "Daniel";
	char *s2 = "Rubin";
	char *s3 = "Fungus";
	char *s4 = "ThisNameIsTooLong";
	ADD_FIELD_ENTRY(s, s1);
	ADD_FIELD_ENTRY(s, s2);
	ADD_FIELD_ENTRY(s, s3);
	ADD_FIELD_ENTRY(s, s4);

	assign_Field(&table, s);

	print_FieldTable(table);

}
