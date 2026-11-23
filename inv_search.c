#include "types.h"
#include "inv_search.h"

int db_flag = 0;

STATUS start_program(int argc,char * argv[], filename_t** f_head, mainnode_t* arr[])
{
    if(argc < 2){
        printf("Error: Please provide input file names\n");
        return e_failure;
    }

    if(validate_files(argc, argv, f_head) == e_failure){
        printf("Error: File Validation failed\n");
        return e_failure;
    }

    printf("File validation success\n");

    menu(f_head, arr);

    return e_success;
}


STATUS validate_files(int argc, char* argv[], filename_t** f_head)
{

    for(int i=1;i<argc;i++){
        int duplicate = 0;

        // Check .txt extension
        char* extn = strchr(argv[i],'.');

        if(extn == NULL || strcmp(extn,".txt") != 0){
            printf("Error: File must be .txt file\n");
            return e_failure;
        }


        // Check file existence
        FILE* fptr = fopen(argv[i],"r");

        if(fptr == NULL){
            printf("Error: %s does not exist\n",argv[i]);
            return e_failure;
        }


        /* Check for duplicate file */
        filename_t* f_temp = *f_head;

        while(f_temp != NULL){
            if(strcmp(f_temp->filename,argv[i]) == 0){
                printf("Error: %s is Duplicate file\n",argv[i]);
                duplicate = 1;
                break;
            }

            f_temp = f_temp->link;
        }

        if(duplicate == 1){
            fclose(fptr);
            continue;
        }


        /* Check wheather file is empty */
        if(fgetc(fptr) == EOF){
            printf("Error: %s file is empty\n",argv[i]);
            fclose(fptr);
            continue;                       // skip: if file is empty
        }

        fclose(fptr);                       // closing file if it is non empty

        /* Create new_node of valid files */
        filename_t* new_node = malloc(sizeof(filename_t));

        // Memory allocation Validation 
        if(new_node == NULL){
            printf("Error: Memory not allocated for file node\n");
            return e_failure;
        }

        strcpy(new_node->filename,argv[i]);
        new_node->link = NULL;

        // if LL is empty
        if(*f_head == NULL){
            *f_head = new_node;
        }

        else
        {
            f_temp = *f_head;

            while(f_temp->link != NULL)            // reach the last node
                f_temp = f_temp->link;
            
            f_temp->link = new_node;
        }
    }

    return e_success;
}


STATUS menu(filename_t** f_head, mainnode_t* arr[])
{
    int choice;

    while(1)
    {
        printf("\nInverted Search Menu\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Update Database\n");
        printf("5. Save Database\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1: if(db_flag == 1){
                        printf("Error: Database already created/updated\n");
                        break;
                    }
                    if(create_database(*f_head, arr) == e_success)
                        db_flag = 1;
                    break;

            case 2: display_database(arr);
                    break;

            case 3: search_database(arr);
                    break;

            case 4: if(db_flag == 1){
                        printf("Error: Database already created/updated\n");
                        break;
                    }
                    if(update_database(f_head, arr) == e_success)
                        db_flag = 1;
                    break;

            case 5: save_database(arr);
                    break;

            case 6: printf("Exiting...\n");
                    return e_success;

            default: printf("Invalid Choice, Please try again\n");
        }
    }

    return e_success;
}