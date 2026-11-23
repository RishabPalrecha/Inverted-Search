#include "types.h"
#include "inv_search.h"


int main(int argc, char* argv[])
{
    // LL of files
    filename_t* f_head = NULL;

    // Hashtable of database
    mainnode_t* arr[HASH_SIZE] = {NULL};

    if(start_program(argc,argv, &f_head, arr) == e_failure){
        printf("Error: Start program failed\n");
        return e_failure;
    }


    return 0;
}