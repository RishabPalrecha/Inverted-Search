#include "types.h"
#include "inv_search.h"


int getindex(char ch)
{
    int index;

    if(ch >= 'a' && ch <= 'z')
        index = ch - 'a';
    else if(ch >= 'A' && ch <= 'Z')
        index = ch - 'A';
    else 
        index = 26;

    return index;
}


STATUS create_main_node(mainnode_t** new_main, char word[])
{
    *new_main = malloc(sizeof(mainnode_t));

    // Memory allocation validation
    if(*new_main == NULL)
        return e_failure;

    // Initializing the new_main node
    strcpy((*new_main)->word, word);
    (*new_main)->file_count = 1;
    (*new_main)->link = NULL;
    (*new_main)->sublink = NULL;
    
    return e_success;
}

STATUS create_sub_node(subnode_t** new_sub, char filename[])
{
    *new_sub = malloc(sizeof(subnode_t));

    if(*new_sub == NULL)
        return e_failure;
    
    // Initializing the new_sub node
    strcpy((*new_sub)->filename,filename);
    (*new_sub)->word_count = 1;
    (*new_sub)->link = NULL;

    return e_success;
}

STATUS create_database(filename_t* f_head, mainnode_t* arr[])
{
    filename_t* f_temp = f_head;        // temporary pointer for LL

    while(f_temp != NULL)
    {
        // File open
        FILE* fptr = fopen(f_temp->filename,"r");
        if(fptr == NULL){
            printf("Error: %s file not opened\n",f_temp->filename);
            return e_failure;
        }


        char word[WORD_SIZE];

        // Reading the file word by word
        while(fscanf(fptr,"%29s",word) == 1)
        {
            // Calculating the index for Hash table
            int index = getindex(word[0]);

            /* LL is empty */
            if(arr[index] == NULL){
                mainnode_t* new_main = NULL;
                subnode_t* new_sub = NULL;

                if(create_main_node(&new_main,word) == e_failure){
                    printf("Error: new main node not created\n");
                    fclose(fptr);
                    return e_failure;
                }

                if(create_sub_node(&new_sub,f_temp->filename) == e_failure){
                    printf("Error: new sub node not created\n");
                    fclose(fptr);
                    return e_failure;
                }

                new_main->sublink = new_sub;
                arr[index] = new_main;
            }

            // LL is not empty 
            else
            {
                mainnode_t* m_temp = arr[index];

                // Traversing in main node and search for word
                while(m_temp != NULL){
                    if(strcmp(m_temp->word,word) == 0)
                        break;
                    
                    m_temp = m_temp->link;
                }

                // word exist in LL
                if(m_temp)
                {
                    subnode_t* s_temp = m_temp->sublink;

                    while(s_temp){
                        if(strcmp(s_temp->filename,f_temp->filename) == 0)
                            break;

                        s_temp = s_temp->link;
                    }

                    //word and file are same
                    if(s_temp)
                        s_temp->word_count++;

                    // word same but file different
                    else
                    {
                        // Create new_sub node
                        subnode_t* new_sub = NULL;

                        if(create_sub_node(&new_sub,f_temp->filename) == e_failure){
                            printf("Error: new sub node not created\n");
                            fclose(fptr);
                            return e_failure;
                        }

                        // Link new_sub node
                        s_temp = m_temp->sublink;

                        while(s_temp->link != NULL)
                            s_temp = s_temp->link;

                        s_temp->link = new_sub;

                        m_temp->file_count++;
                    }
                }

                // word does not matches in LL
                else
                {
                    // Create new_main and new_sub nodes
                    mainnode_t* new_main = NULL;
                    subnode_t* new_sub = NULL;

                    if(create_main_node(&new_main,word) == e_failure){
                        printf("Error: new main node not created\n");
                        fclose(fptr);
                        return e_failure;
                    }

                    if(create_sub_node(&new_sub,f_temp->filename) == e_failure){
                        printf("Error: new sub node not created\n");
                        fclose(fptr);
                        return e_failure;
                    }

                    new_main->sublink = new_sub;            // main -> sub linking


                    // Linking new_main node
                    m_temp = arr[index];

                    while(m_temp->link != NULL)
                        m_temp = m_temp->link;

                    m_temp->link = new_main;
                }
            }
        }

        fclose(fptr);
        f_temp = f_temp->link;
    }

    printf("Database created Successfully\n");
    return e_success;    
}


STATUS display_database(mainnode_t* arr[])
{
    printf("\nIndex\tWord\t\tFile Count\tFile Name(Word Count)\n");
    printf("-------------------------------------------------------------\n");

    for(int i=0;i<27;i++)
    {
        if(arr[i] == NULL)                                      // skip if (index = NULL)
            continue;

        mainnode_t* m_temp = arr[i];

        while(m_temp){                                          // printing main node contents
            printf("%d\t%s\t\t%d\t\t",i,m_temp->word,m_temp->file_count);

            subnode_t* s_temp = m_temp->sublink;

            while(s_temp){                                      // printing sub node contents
                printf("%s (%d)\t",s_temp->filename,s_temp->word_count);

                s_temp = s_temp->link;
            }

            m_temp = m_temp->link;

            printf("\n");
        }
    }

    return e_success;
}

STATUS search_database(mainnode_t* arr[])
{
    char word[WORD_SIZE];

    // Input word from user
    printf("\nEnter the word to search: ");
    scanf("%s",word);


    // Calculating the index
    char first_char = word[0];
    int index;

    if(first_char >= 'a' && first_char <= 'z')
        index = first_char - 'a';
    else if(first_char >= 'A' && first_char <= 'Z')
        index = first_char - 'A';


    /* Searching index in hash table */

    if(arr[index] == NULL){                              // index is NULL
        printf("\nWord Not Found\n");
        return e_failure;
    }
    
    // Index is not NULL, searching
    mainnode_t* m_temp = arr[index];

    while(m_temp)
    {
        if(strcmp(m_temp->word,word) == 0)
            break;

        m_temp = m_temp->link;
    }

    if(m_temp == NULL){
        printf("\nWord Not Found\n");
        return e_failure;
    }

    printf("\nWord %s is present in %d files\n",word,m_temp->file_count);


    subnode_t* s_temp = m_temp->sublink;

    while(s_temp){
        printf("In %s -> %d times\n",s_temp->filename,s_temp->word_count);
        
        s_temp = s_temp->link;
    }

    return e_success;
}


STATUS save_database(mainnode_t* arr[])
{
    char db_filename[FNAME_SIZE];

    printf("\nEnter the Database File name: ");
    scanf("%s",db_filename);

    // Check .txt extension
    char* extn = strchr(db_filename,'.');

    if(extn == NULL || strcmp(extn,".txt") != 0){
        printf("Error: Database file must be .txt file\n");
        return e_failure;
    }

    // open the file in read mode
    FILE* fptr = fopen(db_filename, "w");

    if(fptr == NULL){
        printf("Error: Unable to open Database file\n");
        return e_failure;
    }


    for(int i=0;i<27;i++)
    {
        mainnode_t* m_temp = arr[i];

        while(m_temp)
        {
            fprintf(fptr, "#%d;%s;%d;" , i, m_temp->word,  m_temp->file_count);


            subnode_t* s_temp = m_temp->sublink;

            while(s_temp)
            {
                fprintf(fptr, "%s;%d;" ,s_temp->filename, s_temp->word_count);

                s_temp = s_temp->link;
            }

            fprintf(fptr, "#\n");

            m_temp = m_temp->link;
        }
    }

    fclose(fptr);

    printf("Database saved Successfully\n");

    return e_success;
}


STATUS update_database(filename_t** f_head, mainnode_t* arr[])
{
    char db_filename[FNAME_SIZE];
    
    printf("\nEnter the Database filename: ");
    scanf("%s",db_filename);

    // Extension must be .txt
    char* extn = strchr(db_filename,'.');

    if(extn == NULL || strcmp(extn,".txt") != 0){
        printf("Error: Database file must be .txt file\n");
        return e_failure;
    }


    // File exist and empty or not validation
    FILE* fptr = fopen(db_filename,"r");

    if(fptr == NULL){
        printf("Error: Database file doesnot exist\n");
        return e_failure;
    }

    if(fgetc(fptr) == EOF){
        printf("Error: Database file is empty\n");
        fclose(fptr);
        return e_failure;
    }

    fseek(fptr,-1,SEEK_CUR);

    // Check First character
    char ch;
    fscanf(fptr, "%c", &ch);

    if(ch != '#'){
        printf("Error: Invalid database file due to starting character\n");
        fclose(fptr);
        return e_failure;
    }

    // Check last character
    fseek(fptr,-2,SEEK_END);
    fscanf(fptr,"%c", &ch);

    if(ch != '#'){
        printf("Error: Invalid database file due to last character\n");
        fclose(fptr);
        return e_failure;
    }


    rewind(fptr);                               // offset back to beginning


    // Extract data from database
    int index, file_count;
    char word[WORD_SIZE];

    while(!feof(fptr))
    {
        // main node info
        if(fscanf(fptr, " #%d;%[^;];%d;" ,&index,word,&file_count) != 3)
            break;

        // Create main node
        mainnode_t* new_main = NULL;

        if(create_main_node(&new_main,word) == e_failure){
            printf("Error: Unable to create main node\n");
            fclose(fptr);
            return e_failure;
        }

        new_main->file_count = file_count;


        // Subnode info
        subnode_t* s_temp = NULL;
        
        for(int i=0;i<file_count;i++){
            char filename[FNAME_SIZE];
            int word_count;

            if(fscanf(fptr, "%[^;];%d;" ,filename,&word_count) != 2)
                return e_failure;



            // Remove Duplicate file_node from file LL
            filename_t* f_temp = *f_head;
            filename_t* f_prev = NULL;

            while(f_temp){
                if(strcmp(f_temp->filename,filename) == 0){
                    // If first node
                    if(f_prev == NULL)
                        *f_head = (*f_head)->link;
                    
                    // If mid or last node
                    else
                        f_prev->link = f_temp->link;

                    free(f_temp);
                    break;
                }

                f_prev = f_temp;
                f_temp = f_temp->link;
            }



            // Create sub node
            subnode_t* new_sub = NULL;

            if(create_sub_node(&new_sub,filename) == e_failure){
                printf("Error: Unable to create sub node\n");
                fclose(fptr);
                return e_failure;
            }

            new_sub->word_count = word_count;


            // Link subnode
            if(new_main->sublink == NULL){
                new_main->sublink = new_sub;
                s_temp = new_sub;
            }
            else{
                s_temp->link = new_sub;
                s_temp = s_temp->link;
            }
        }

        // consuming # and \n at last in line
        fscanf(fptr, "#\n");


        if(arr[index] == NULL){
            arr[index] = new_main;
        }
        else{
            mainnode_t* m_temp = arr[index];

            while(m_temp->link != NULL)
                m_temp = m_temp->link;

            m_temp->link = new_main;
        }
    }

    fclose(fptr);

    if(*f_head != NULL){
        if(create_database(*f_head,arr) == e_failure)
            return e_failure;
    }

    printf("Database updated Successfully\n");
    return e_success;
}