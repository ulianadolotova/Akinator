#ifndef TX_USE_SPEAK
    #define TX_USE_SPEAK
#endif

#include "TXlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <sys\stat.h>

struct Node
{
    char* name;
    char* link;
    Node* positive_child;
    Node* negative_child;
    Node* parent;
};

#include "C:\\Users\\Ульяна\\Desktop\\Projects\\Akinator\\Stack\\Stack.h"

struct AkinatorTree
{
    Node* root;
};


enum errors_akinator
{
    MEM_ERROR          = 0,
    INCORRECT_FORMAT   = 1,
    IMPOSSIBLE_COMMAND = -1,
    NULL_POINTER       = 2,

};

enum modes
{
    GAME              = 0,
    TREE              = 1,
    DEFINITION        = 2,
    COMPARE           = 3,
    EXIT              = 4
};

const size_t MAX_COMMAND_NAME = 100;
const size_t DEFAULT_SIZE     = 100;
const double POSITIVE         = 1;
const double NEGATIVE         = -1;



size_t Size_File_Counter (const char* file_name);
size_t Buff_Filler       (char* point_buff, const char* file_name, size_t file_size);
char*  Buff_Maker        (size_t file_size);


AkinatorTree* Tree_Construct  (void);
Node*         Tree_Filler     (char* point_buff, Node* node);
void          Save_Changes    (Node* tree, FILE* data_base);
Node*         Node_Construct  (Node* node);
Node*         Find_Node       (Node* node, char* name);
void          Stack_Filler    (Stack_t* my_stack, Node* node);
void          NonRecursiveDFS (Stack_t* my_stack, Node* tree, char* name);


void Node_Print (Node* tree, FILE* Tree_image);
void Tree_Print (Node* tree);
void Logs_Print (int code_error, Node* tree);


void Mode_Processing (AkinatorTree* tree);
void Akinator_Game   (AkinatorTree* tree, Node* node);
void Add_Song        (char* new_song, char* difference, char* link, Node* node);
void Play_Song       (Node* node);
void Song_Definition (Node* node);
void Song_Compare    (Node* tree);


