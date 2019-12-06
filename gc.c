#include <stdio.h>
#include <stdlib.h>
#include "gc.h"


char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT"};

/* Dumping Function */

void
print_structure_record(structure_db_rec_t *structure_record){
    if(!structure_record) return; // Taking care if no structure is initialized.
    int j = 0;
    field_info_t *fld = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, structure_record->structure_name_, structure_record->structure_size, structure_record->number_of_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|-------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for(j = 0; j < structure_record->number_of_fields; j++){
        fld = &structure_record->fields[j];
        printf("  %-20s |", "");
        printf("%-3d %-20s | FieldType = %-15s | FieldSize = %-5d | FieldOffset = %-6d|  NestedStructName = %-20s  |\n",
                j, fld->field_name, DATA_TYPE[fld->field_type], fld->field_size, fld->offset, fld->nested_structure_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "---------------------------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}


void
print_structure_database(structure_db_t *structure_db){
    
    if(!structure_db) return; //Taking care if no structure is there, then no db will exist.
    printf("Printing structure Database\n");
    int i = 0;
    structure_db_rec_t *structure_record = NULL;
    structure_record = structure_db->head;
    printf("No. of Structures Registered = %d\n", structure_db->count);
    while(structure_record){
        printf("Structure No : %d (%p)\n", i++, structure_record); // %p will print the hexadecimal value of the address to which pointer struct_rec is pointing rightnow.
        print_structure_record(structure_record);
        structure_record = structure_record->next;
    }
}


int add_structure_to_structure_db(structure_db_t *struct_db, structure_db_rec_t *new_structure_record){
	structure_db_rec_t *head = struct_db->head; //Saving the value of the current head of the structure database.
	
	if(!head){ //Seeing if its the first structure being added to the structure database. If it is first then this if block will execute
		struct_db->head = new_structure_record;
		new_structure_record->next=NULL;
		struct_db->count++;
		return 0;
		}
	// If the new record added is not the first one.
	new_structure_record->next=head; // making the new record as the head, saving the head of the linked list in the next of new record.
	struct_db->head=new_structure_record;
	struct_db->count++;
	return 0;
};
