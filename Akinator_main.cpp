#include "Akinator.h"


int main ()
{
    const char* file_name = "Data_base.txt";
    size_t file_size  = Size_File_Counter (file_name);
    char*  point_buff = Buff_Maker (file_size);

    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    AkinatorTree* tree = Tree_Construct ();
    tree->root         = Tree_Filler (point_buff, tree->root);


    txSpeak ("\v\aHello! You can play 4 modes and end game with \"Exit\"\n");
    Mode_Processing (tree);

    return 0;
}


void Mode_Processing (AkinatorTree* tree)
{
    if (tree == NULL)
        Logs_Print (NULL_POINTER, NULL);

    char temp[MAX_COMMAND_NAME] = {};
    int num_command = IMPOSSIBLE_COMMAND;

    while (num_command != EXIT)
    {
        gets (temp);

        if      (!strcmpi (temp,  "GAME"))
                    num_command =  GAME;

        else if (!strcmpi (temp,  "TREE"))
                    num_command =  TREE;

        else if (!strcmpi (temp,  "DEFINITION"))
                    num_command =  DEFINITION;

        else if (!strcmpi (temp,  "COMPARE"))
                    num_command =  COMPARE;

        else if (!strcmpi (temp,  "EXIT"))
                    num_command =  EXIT;

        else Logs_Print (IMPOSSIBLE_COMMAND, tree->root);

        switch (num_command)
        {
            case GAME:       Akinator_Game (tree, tree->root);
                             break;

            case TREE:       Tree_Print (tree->root);
                             break;

            case DEFINITION: Song_Definition (tree->root);
                             break;

            case COMPARE:    Song_Compare (tree->root);
                             break;

            case EXIT:       FILE* data_base = fopen ("Data_base.txt", "w");
                             Save_Changes (tree->root, data_base);
                             fclose (data_base);
                             break;
        }
    }

}



void Akinator_Game (AkinatorTree* tree, Node* node)
{
    if (!txWindow())
        txCreateWindow (900, 600);
    txBitBlt (txDC(), 0, 0, 900, 600, txLoadImage("Music.bmp"));

    if (tree == NULL || node == NULL)
        Logs_Print (NULL_POINTER, node);

    txSpeak ("\v\n\nWhat's your favorite song? Let me guess!\n\n");

    char answer[MAX_COMMAND_NAME] = {};

    while (node->positive_child != NULL && node->negative_child != NULL)
    {
        txSpeak ("\v\a\nIs it %s?\n", node->name);

        gets (answer);

        if      (!strcmpi ("YES", answer))
                node = node->positive_child;

        else if (!strcmpi ("NO", answer))
                node = node->negative_child;

        else    Logs_Print (IMPOSSIBLE_COMMAND, tree->root);
    }

    txSpeak ("\v\nSo.. I guess, your favorite song is %s\n", node->name);
    txSpeak ("\v\a\nIsn't it?\n");

    gets (answer);

    if  (!strcmpi ("YES", answer))
    {
        txSpeak ("\v\a\nHow great! I like this song too. Would you like to hear it?\n");
        gets (answer);

        if      (!strcmpi ("YES", answer))
        {
            Play_Song (node);
        }
        else if (!strcmpi ("NO", answer))
        {
            txSpeak ("\v\aSo, i hope you will listen it soon, cause it's really awesome\n");
        }
        else
        {
            Logs_Print (IMPOSSIBLE_COMMAND, tree->root);
        }
    }

    else if (!strcmpi ("NO", answer))
    {
        txSpeak ("\v\a\nOh, but what did you mean then? Tell me your favorite song!\n");

        char* new_song = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (new_song);

        txSpeak ("\v\nWhat's the difference with %s?\n", node->name);
        txSpeak ("\v\a\n%s is not ", new_song);

        char* difference = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (difference);

        txSpeak ("\v\a\nOk! Got you! Can you, please, share a link to this song on YouTube?\n");

        char* link = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (link);

        txSpeak ("\v\nThank you! I'll wait for the next game with you!\n\n");

        Add_Song (new_song, difference, link, node);
    }

    else Logs_Print (IMPOSSIBLE_COMMAND, tree->root);
}



Node* Tree_Filler (char* point_buff, Node* node)
{
    char* string_temp = NULL;

    if (point_buff != NULL)
        string_temp = strtok (point_buff, "\"\t\n");

    else
        string_temp = strtok (NULL,       "\"\t\n");

    node       = Node_Construct (node);

    node->name = string_temp;

    string_temp = strtok (NULL, "\"\t\n");

    if (string_temp == NULL)
        Logs_Print (INCORRECT_FORMAT, node);

    if (*string_temp == 'h')
    {
        node->link = string_temp;
        string_temp = strtok (NULL,       "\"\t\n");
    }

    else if (*string_temp == '{')
    {
        node->positive_child         = Tree_Filler (NULL, node->positive_child);
        node->positive_child->parent = node;

        string_temp = strtok (NULL, "\"\t\n");

        if (string_temp && *string_temp == '{')
        {
            node->negative_child         = Tree_Filler (NULL, node->negative_child);
            node->negative_child->parent = node;
        }

        string_temp = strtok (NULL, "\"\t\n");
    }

    return node;
}



AkinatorTree* Tree_Construct (void)
{
    AkinatorTree* tree = (AkinatorTree*) calloc (1, sizeof (AkinatorTree));
    tree->root         = NULL;

    if (tree == NULL)
        Logs_Print (MEM_ERROR, NULL);

    return tree;
}



void Add_Song (char* new_song, char* difference, char* link, Node* node)
{
    if (new_song == NULL || difference == NULL || node == NULL)
        Logs_Print (NULL_POINTER, node);

    node->positive_child = Node_Construct (node->positive_child);
    node->negative_child = Node_Construct (node->negative_child);
    node->positive_child->parent = node;
    node->negative_child->parent = node;

    node->positive_child->name = node->name;
    node->positive_child->link = node->link;

    node->negative_child->name = new_song;
    node->negative_child->link = link;

    node->name = difference;
    node->link = NULL;
}



void Play_Song (Node* node)
{
    if (node == NULL)
        Logs_Print (NULL_POINTER, node);

    char*    system_command = (char*) calloc (2 * MAX_COMMAND_NAME, sizeof (char));
    sprintf (system_command, "start chrome %s", node->link);
    system  (system_command);

    free (system_command);
}


void Save_Changes (Node* tree, FILE* data_base)
{
    if (tree == NULL || data_base == NULL)
        Logs_Print (NULL_POINTER, tree);

    fprintf (data_base, "\"%s\"\n", tree->name);
    if (!tree->positive_child && !tree->negative_child)
        fprintf (data_base, "%s\n", tree->link);

    if (tree->positive_child)
    {
        fprintf (data_base, "{\n");
        Save_Changes (tree->positive_child, data_base);
        fprintf (data_base, "}\n");
    }

    if (tree->negative_child)
    {
        fprintf (data_base, "{\n");
        Save_Changes (tree->negative_child, data_base);
        fprintf (data_base, "}\n");
    }
}


void Song_Definition (Node* node)
{
    txSpeak ("\v\a\n\nEnter the song you want to be described\n");
    char name[MAX_COMMAND_NAME] = {};
    gets (name);

    Stack_t* my_stack = StackConstruct (DEFAULT_SIZE);
    NonRecursiveDFS (my_stack, node, name);
    StackReverse (my_stack);

    elem_t temp_elem = POISON;
    while (my_stack->size_ != 0)
    {
        temp_elem = StackPop (my_stack);
        printf ("It %s %s\n", (temp_elem == temp_elem->parent->positive_child) ? "is" : "is not", temp_elem->parent->name);
    }
}


Node* Find_Node (Node* node, char* name)
{
/*
    if (strcmpi (name, node->name) == 0)
        return node;

    Node* temp = NULL;
    if (node->negative_child != NULL)
    {
        temp = Find_Node (node->negative_child, name);
        if (temp != NULL)
            return temp;
    }

    if (node->positive_child != NULL)
    {
        temp = Find_Node (node->positive_child, name);
        if (temp != NULL)
            return temp;
    }
 */
    return NULL;
}

void Stack_Filler (Stack_t* my_stack, Node* node)
{
/*
    while (node->parent != NULL)
    {
        StackPush (my_stack, (node->parent->positive_child == node) ? POSITIVE : NEGATIVE);
        node = node->parent;
    }
*/
}


void NonRecursiveDFS (Stack_t* my_stack, Node* tree, char* name)
{
    Node* node = tree->negative_child;

    while (node)
    {
        StackPush (my_stack, node);
        if (strcmpi (name, node->name) == 0)
            break;

        if (node->negative_child)
        {
            node = node->negative_child;
            continue;
        }

        if (node == node->parent->negative_child)
        {
            StackPop (my_stack);
            node = node->parent;
        }
        else
        {
            while (node->parent && node == node->parent->positive_child)
            {
                StackPop (my_stack);
                node = node->parent;
            }
            if (my_stack->size_ > 0)
                StackPop (my_stack);
            node = node->parent;
        }

        if (node && node->positive_child)
        {
            node = node->positive_child;
        }

    }

    if (node == NULL)
        Logs_Print (IMPOSSIBLE_COMMAND, tree);
}
//

void Song_Compare (Node* tree)
{
    txSpeak ("\v\a\n\nEnter the songs you want to be compared\n");
    char first_name[MAX_COMMAND_NAME]  = {};
    char second_name[MAX_COMMAND_NAME] = {};

    gets (first_name);
    gets (second_name);

    Stack_t* first_stack  = StackConstruct (DEFAULT_SIZE);
    Stack_t* second_stack = StackConstruct (DEFAULT_SIZE);

    /*
    Node* first_node  = Find_Node (tree,  first_name);
    Node* second_node = Find_Node (tree, second_name);

    Stack_Filler (first_stack,   first_node);
    Stack_Filler (second_stack, second_node);
    */

    NonRecursiveDFS  (first_stack, tree,  first_name);
    NonRecursiveDFS (second_stack, tree, second_name);

    /*
    double first_elem  = StackPop  (first_stack);
    double second_elem = StackPop (second_stack);
    */
    Node* first_elem  = StackPop  (first_stack);
    Node* second_elem = StackPop (second_stack);
    StackReverse  (first_stack);
    StackReverse (second_stack);

    Node* node = tree;

    if (first_elem == second_elem)
        printf ("\nBoth are:\n");

    while (first_elem == second_elem && first_stack->size_ > 0 && second_stack->size_ > 0)
    {
        /*
        if (first_elem == NEGATIVE)
            printf ("Not ");
        printf ("%s\n", node->name);
        node = (first_elem == POSITIVE) ? node->positive_child : node->negative_child;

        first_elem  = StackPop  (first_stack);
        second_elem = StackPop (second_stack);
        */

        if (first_elem == first_elem->parent->negative_child)
            printf ("Not ");
        printf ("%s\n", first_elem->parent->name);

        first_elem  = StackPop  (first_stack);
        second_elem = StackPop (second_stack);
    }
    /*
    Node* saved_node = node;
    if (node == tree)
        printf ("\nThe first is:\n");
    else
        printf ("\nBut the first is:\n");

    if (first_elem == NEGATIVE)
            printf ("Not ");
        printf ("%s\n", node->name);
    while (first_stack->size_ > 0)
    {
        node = (first_elem == POSITIVE) ? node->positive_child : node->negative_child;
        first_elem = StackPop (first_stack);

        if (first_elem == NEGATIVE)
            printf ("Not ");
        printf ("%s\n", node->name);
    }

    printf ("\nAnd the second is:\n");


    if (second_elem == NEGATIVE)
            printf ("Not ");
    printf ("%s\n", saved_node->name);
    while (second_stack->size_ > 0)
    {
        saved_node = (second_elem == POSITIVE) ? saved_node->positive_child : saved_node->negative_child;
        second_elem = StackPop (second_stack);

        if (second_elem == NEGATIVE)
            printf ("Not ");
        printf ("%s\n", saved_node->name);
    }*/

}




Node* Node_Construct (Node* node)
{
    node = (Node*) calloc (1, sizeof (Node));
    *node = {};

    if (node == NULL)
        Logs_Print (MEM_ERROR, node);

    return node;
}
