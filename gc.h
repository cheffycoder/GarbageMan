#include<assert.h> // To access to the assert function, which allows us to stop the program if anything uncertain happens.

//Defining macros
#define MAX_STRUCTNAMESIZE 128
#define MAX_FIELDNAMESIZE 128

typedef struct structure_db_rec_ structure_db_rec_t;

//Enumeration for data types
typedef enum{
UINT8,
UINT32,
INT32,
CHAR, 
OBJ_PTR, //this represent any pointer field within a C structure which point to another object of some structure type.
FLOAT,
DOUBLE,
OBJ_STRUCT
}data_type;

//Macros 
#define OFFSETOF(struct_name,fld_name)		(unsigned int)&(((struct_name *)0)->fld_name)
#define FIELD_SIZE(struct_name,fld_name)		sizeof(((struct_name *)0)->fld_name)


//Defining the structure that will store all the information about a field.
typedef struct field_info_{
	char field_name [MAX_FIELDNAMESIZE];
	unsigned int field_size;
	data_type field_type;
	unsigned int offset;
	char nested_structure_name[MAX_STRUCTNAMESIZE];
}field_info_t;


//Defining every single record in the structure database having linked lists.
struct structure_db_rec_{
	structure_db_rec_t *next;
	char structure_name_ [MAX_STRUCTNAMESIZE]; // Made a character array
	unsigned int structure_size;
	unsigned int number_of_fields;
	field_info_t *fields; // Pointer to the first element of the fields informations array
};


//Defining structure database which is the head of the whole structure database linked lists.
typedef struct structure_db_{
	structure_db_rec_t *head; // Pointer to the start of the structure database rec linked list.
	unsigned int count; // Saves the total number of records in the structure linked lists.
}structure_db_t;


/*	Strucutre database definition ends here.	*/

/*	Printing Functions	> 			*/

void print_structure_record(structure_db_rec_t *structure_record);
void print_structure_database(structure_db_t *structure_db);

/*	Function to add new structure record to structure database	*/
int /*return 0 on success, -1 on failure for some reason*/
add_structure_to_structure_db(structure_db_t *struct_db, structure_db_rec_t *new_structure_record);











