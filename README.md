# Field Tables
Small C library to record, and later print formatted values in a 'table' of 'fields'.
## Features
- Each 'field' has its own format specifier. For example, field "time elapsed" could have the format spec "%0.2lf".
- Fields can have a max/min width, which defaults to the strlen of the title. They can also be left or right justified (right default).
- If you add an entry (or a title) to a field which is truncated by the `max_print_width`, you will get a warning (which can be suppressed by defining `_SUPPRESS_TRUNCATION_WARNINGS`).


## Using the library

### 0. Creating a field table
Field Tables are created via:
```c
FieldTable_t new_FieldTable(char* table_name);
//destructor:
void destroy_FieldTable(FieldTable_t* table);
```

### 1. Defining fields 
After (or before) this, Fields themselves can be created/defined: 
```c
Field_t *new_Field(const char* field_title,
                   DATATYPE FIELD_TYPE, 
                   const char* fmt, 
                   bool left_justify,  
                   ssize_t max_print_width);
```

### 2. Adding entries to a table
Once a field is created, you may add 'entries' (rows) to them, by calling the following:
```c
void ADD_FIELD_ENTRY(FieldTable_t* field_ptr, void* val_ptr);
```
For most use cases, the macro `add_Field_entry(field_ptr, val)` is probably prefered, as it will create a compound literal and dereference the 'val' field for you, allowing you to directly write in literals or other r-values.


After fields are populated/have entries, you must assign them to a table. Note that this is a copy assignment; tables dont store pointers, they store values (for the most part). 

### 3. assignment to a table
```c
void assign_Field(FieldTable_t* table_ptr, Field_t *field_ptr);
```
To this end, it makes sense to assign the fields right before printing a table. 

*i do wish to change this interface in the future, as to me it makes more sense to 'assign' a field to a table at the same time as creating it*

#### Lastly, printing:
```c
void print_FieldTable(FieldTable_t table);

/* should print something like:
|FieldTitle|FieldTitle2  |FieldTitle3   |
|Field1row1|         1234|1234          |
|Field1row2|        12345|12345         |
|Field1row3|      88.25ms|88.25ms       |
*/
```
