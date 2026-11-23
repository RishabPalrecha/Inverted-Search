#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FNAME_SIZE 30
#define WORD_SIZE 30
#define HASH_SIZE 27

#define WORD_NOT_FOUND -1


typedef enum{
    e_success,
    e_failure
}STATUS;


/* Sub Node */
typedef struct sub{
    char filename[FNAME_SIZE];
    int word_count;
    struct sub* link;
}subnode_t;


/* Main Node */
typedef struct main{
    char word[WORD_SIZE];
    int file_count;
    struct main* link;
    subnode_t* sublink;
}mainnode_t;


// File Linked List
typedef struct file{
    char filename[FNAME_SIZE];
    struct file* link;
}filename_t;



#endif