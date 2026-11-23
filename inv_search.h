#ifndef INV_SEARCH_H
#define INV_SEARCH_H

#include "types.h"
 
/* Program Starter */
STATUS start_program(int argc,char * argv[], filename_t** f_head, mainnode_t* arr[]);

/* Initial Validation of CLA */
STATUS validate_files(int argc, char* argv[], filename_t** f_head);

/* Menu */
STATUS menu(filename_t** f_head, mainnode_t* arr[]);

/* Create the Database */
STATUS create_database(filename_t* f_head, mainnode_t* arr[]);

/* To find index in Hash table */
int getindex(char ch);

STATUS create_main_node(mainnode_t** new_main, char word[]);

STATUS create_sub_node(subnode_t** new_sub, char filename[]);

STATUS display_database(mainnode_t* arr[]);

STATUS search_database(mainnode_t* arr[]);

STATUS save_database(mainnode_t* arr[]);

STATUS update_database(filename_t** f_head, mainnode_t* arr[]);




#endif 