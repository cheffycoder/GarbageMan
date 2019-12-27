#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "css.h"


char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT"};


/*Working with Structure Database*/

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


static structure_db_rec_t *
structure_db_look_up(structure_db_t *struct_db,
                  char *struct_name){
    
    structure_db_rec_t *head = struct_db->head;
    if(!head) return NULL;
    
    for(; head; head = head->next){
        if(strncmp(head->structure_name_, struct_name, MAX_STRUCTNAMESIZE) == 0)
            return head;
    }
    return NULL;
}


/* Dumping Function for Structure Database*/

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



















/*Working with objects*/

static object_db_rec_t *
object_db_look_up(object_db_t *object_db, void *ptr){

    object_db_rec_t *head = object_db->head;
    if(!head) return NULL;
    
    for(; head; head = head->next){
        if(head->object_ptr == ptr)
            return head;
    }
    return NULL;
}


static void
add_object_to_object_db(object_db_t *object_db, 
                     void *ptr, 
                     int units,
                     structure_db_rec_t *struct_rec,
                     boolean_t is_root){
     
    object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
    /*Dont add same object twice*/
    assert(!obj_rec);

    obj_rec = calloc(1, sizeof(object_db_rec_t));

    obj_rec->next = NULL;
    obj_rec->object_ptr = ptr;
    obj_rec->units = units;
    obj_rec->structure_rec = struct_rec;
    obj_rec->is_visited = GC_FALSE;
    obj_rec->is_root = is_root;

    object_db_rec_t *head = object_db->head;
        
    if(!head){
        object_db->head = obj_rec;
        obj_rec->next = NULL;
        object_db->count++;
        return;
    }

    obj_rec->next = head;
    object_db->head = obj_rec;
    object_db->count++;
}




void *
xcalloc(object_db_t *object_db, 
        char *struct_name, 
        int units){

    structure_db_rec_t *struct_rec = structure_db_look_up(object_db->structure_db, struct_name);
    assert(struct_rec);
    void *ptr = calloc(units, struct_rec->structure_size);
    add_object_to_object_db(object_db, ptr, units, struct_rec, GC_FALSE); /*xcalloc by default set the object as non-root*/
    return ptr;
}



/*Dumping Functions for Object database*/
void 
print_object_rec(object_db_rec_t *obj_rec, int i){
    
    if(!obj_rec) return;
    printf(ANSI_COLOR_MAGENTA"|--------------------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "|%-3d ptr = %-10p | next = %-15p | units = %-4d | struct_name = %-10s | is_root = %s |\n"ANSI_COLOR_RESET, 
        i, obj_rec->object_ptr, obj_rec->next, obj_rec->units, obj_rec->structure_rec->structure_name_,  obj_rec->is_root ? "TRUE " : "FALSE"); 
    printf(ANSI_COLOR_MAGENTA "|--------------------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
}

void
print_object_db(object_db_t *object_db){

    object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN "Printing OBJECT DATABASE\n");
    for(; head; head = head->next){
        print_object_rec(head, i++);
    }
}







/*The global object of the application which is not created by xcalloc
  should be registered with MLD using below API*/
void 
register_global_object_as_root(object_db_t *object_db,
                          void *objptr,
                          char *struct_name,
                          unsigned int units){

    structure_db_rec_t *struct_rec = structure_db_look_up(object_db->structure_db, struct_name);
    assert(struct_rec);

   /*Create a new object record and add to object database*/
   add_object_to_object_db(object_db, objptr, units, struct_rec, GC_TRUE);  
}

/* Application might create an object using xcalloc , but at the same time the object
   can be root object. Use this API to override the object flags for the object already
   preent in object db*/
void
set_dynamic_object_as_root(object_db_t *object_db, void *obj_ptr){

    object_db_rec_t *obj_rec = object_db_look_up(object_db, obj_ptr);
    assert(obj_rec);
    
    obj_rec->is_root = GC_TRUE;
}




/*  Initialize the algorithm by setting is_visited flag of all the objects as FALSE.    */
static void
init_algorithm(object_db_t *object_db){

     object_db_rec_t *obj_rec = object_db->head;
     while(obj_rec){
         obj_rec->is_visited = GC_FALSE;
         obj_rec = obj_rec->next;
     }
}


/*  Finds the next root node to start exploring rest of the objects attached to it.  */
static object_db_rec_t *
get_next_root_object(object_db_t *object_db, 
                     object_db_rec_t *starting_from_here){

    object_db_rec_t *first = starting_from_here ? starting_from_here->next : object_db->head;
    while(first){
        if(first->is_root)
            return first;
        first = first->next;
    }
    return NULL;
}




/* Level 2 Pseudocode : This function explore the direct childs of obj_rec and mark
 * them visited. Note that obj_rec must have already visted.*/
static void
explore_objects_recursively(object_db_t *object_db, 
                                object_db_rec_t *parent_obj_rec){

    unsigned int i , n_fields;
    char *parent_obj_ptr = NULL,
         *child_obj_offset = NULL;
    void *child_object_address = NULL;
    field_info_t *field_info = NULL;

    object_db_rec_t *child_object_rec = NULL;
    structure_db_rec_t *parent_struct_rec = parent_obj_rec->structure_rec;

    if(!parent_struct_rec){
        /* Handling void pointers : We cannot explore fields of objects which are of type void * in application.
         * Such objects will not have structure records, hence, nothing to do here*/
        return;
    }

    /*Parent object must have already visited*/
    assert(parent_obj_rec->is_visited);

    if(parent_struct_rec->number_of_fields == 0){
        return;
    }

    for( i = 0; i < parent_obj_rec->units; i++){

        parent_obj_ptr = (char *)(parent_obj_rec->object_ptr) + (i * parent_struct_rec->structure_size);

        for(n_fields = 0; n_fields < parent_struct_rec->number_of_fields; n_fields++){

            field_info = &parent_struct_rec->fields[n_fields];

            /*We are only concerned with fields which are pointer to
             * other objects*/
            switch(field_info->field_type){
                case UINT8:
                case UINT32:
                case INT32:
                case CHAR:
                case FLOAT:
                case DOUBLE:
                case OBJ_STRUCT:
                    break;
                case OBJ_PTR:
                default:
                    ;

                /*child_obj_offset is the memory location inside parent object
                 * where address of next level object is stored*/
                child_obj_offset = parent_obj_ptr + field_info->offset;
                memcpy(&child_object_address, child_obj_offset, sizeof(void *));

                /*child_object_address now stores the address of the next object in the
                 * graph. It could be NULL, Handle that as well*/
                if(!child_object_address) continue;

                child_object_rec = object_db_look_up(object_db, child_object_address);

                assert(child_object_rec);
                /* Since we are able to reach this child object "child_object_rec" 
                 * from parent object "parent_obj_ptr", mark this
                 * child object as visited and explore its children recirsively. 
                 * If this child object is already visited, then do nothing - avoid infinite loops*/
                if(!child_object_rec->is_visited){
                    child_object_rec->is_visited = GC_TRUE;
                    explore_objects_recursively(object_db, child_object_rec);
                }
                else{
                    continue; /*Do nothing, explore next child object*/
                }
            }
        }
    }
}


/*  We will traverse the graph starting from root objects and mark all the reachable nodes as visited.  */
void
run_algorithm(object_db_t *object_db){

    /*Step 1 : Mark all objects in object databse as unvisited*/
    init_algorithm(object_db);

    /* Step 2 : Get the first root object from the object db, it could be 
     * present anywhere in object db. If there are multiple roots in object db
     * return the first one, we can start mld algorithm from any root object*/

    object_db_rec_t *root_obj = get_next_root_object(object_db, NULL);

    while(root_obj){
        if(root_obj->is_visited){
            /* It means, all objects reachable from this root_obj has already been
             * explored, no need to do it again, else you will end up in infinite loop.
             * Remember, Application Data structures are cyclic graphs*/
            root_obj = get_next_root_object(object_db, root_obj);
            continue;
        }
        
        /*root objects are always reachable since application holds the global
         * variable to it*/ 
        root_obj->is_visited = GC_TRUE;
        
        /*Explore all reachable objects from this root_obj recursively*/
        // Define later -> explore_objects_recursively();

        root_obj = get_next_root_object(object_db, root_obj);
    } 
}
