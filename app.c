#include "gc.h"
#include <memory.h>
#include <stdlib.h>

//OBJ-PTR IS TAKING REAL SPACE OF 12. `Will tackle this later.`
/*Application Structures*/

typedef struct emp_{

    char emp_name[30];
    unsigned int emp_id;
    unsigned int age;
    struct emp_ *mgr;
    float salary;
} emp_t;

typedef struct student_{

    char stud_name[32];
    unsigned int rollno;
    unsigned int age;
    float aggregate;
    struct student_ *best_colleage;
} student_t;

typedef struct vehicle_{
    char vehicle_name[32];
    unsigned int vehicle_number;
    char driver_name[32];
    struct vehicle_ *super_cars;
} vehicle_t;

int
main(int argc, char **argv){

    /*Step 1 : Initialize a new structure database */
    structure_db_t *struct_db = calloc(1, sizeof(structure_db_t));
    
    /*Create structure record for structure emp_t*/
    static field_info_t emp_fields[] = {
        FIELD_INFO(emp_t, emp_name, CHAR,    0),
        FIELD_INFO(emp_t, emp_id,   UINT32,  0),
        FIELD_INFO(emp_t, age,      UINT32,  0),
        FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
        FIELD_INFO(emp_t, salary,   FLOAT, 0)
    };
    REG_STRUCT(struct_db, emp_t, emp_fields);

    static field_info_t stud_fields[] = {
        FIELD_INFO(student_t, stud_name, CHAR, 0),
        FIELD_INFO(student_t, rollno,    UINT32, 0),
        FIELD_INFO(student_t, age,       UINT32, 0),
        FIELD_INFO(student_t, aggregate, FLOAT, 0),
        FIELD_INFO(student_t, best_colleage, OBJ_PTR, student_t)
    };
    REG_STRUCT(struct_db, student_t, stud_fields);

    static field_info_t vehicle_fields[] = {
    FIELD_INFO(vehicle_t, vehicle_name, CHAR, 0),
    FIELD_INFO(vehicle_t, vehicle_number, UINT32, 0),    
    FIELD_INFO(vehicle_t, driver_name, CHAR, 0),
    FIELD_INFO(vehicle_t, super_cars, OBJ_PTR, vehicle_t),    
    };
    REG_STRUCT(struct_db, vehicle_t, vehicle_fields);

    print_structure_database(struct_db);
    return 0;
}
