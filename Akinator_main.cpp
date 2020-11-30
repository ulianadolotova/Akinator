#include "Akinator.h"


int main ()
{
    const char* file_name = "Data_base.txt";
    size_t file_size  = Size_File_Counter (file_name);
    char*  point_buff = Buff_Maker (file_size);

    size_t num_symbols = Buff_Filler (point_buff, file_name, file_size);

    AkinatorTree* tree = Tree_Construct ();
    tree->root         = Tree_Filler (point_buff, tree->root);

    Mode_Processing (tree);
    //Tree_Print (tree->root);

    return 0;
}


void Mode_Processing (AkinatorTree* tree)
{
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

//            case DEFINITION: Song_Definition (tree);
//                             break;

//            case COMPARE:    Song_Compare    (tree);
//                             break;

            case EXIT:       FILE* data_base = fopen ("Data_base.txt", "w");
                             Save_Changes    (tree->root, data_base);
                             fclose (data_base);
                             break;
        }
    }

}


void Akinator_Game (AkinatorTree* tree, Node* node)
{
    printf ("\n\nWhat's your favorite song? Let me guess!\n\n");

    char answer[MAX_COMMAND_NAME] = {};

    while (node->positive_child != NULL && node->negative_child != NULL)
    {
        printf ("\nIs it %s\n", node->name);

        gets (answer);

        if      (!strcmpi ("YES", answer))
                node = node->positive_child;

        else if (!strcmpi ("NO", answer))
                node = node->negative_child;

        else Logs_Print (IMPOSSIBLE_COMMAND, tree->root);
    }

    printf ("\nSo.. I guess, your favorite song is %s\n", node->name);
    printf ("\nIsn't it?\n");

    gets (answer);

    if  (!strcmpi ("YES", answer))
    {
        printf ("\nHow great! I like this song too. Would you like to hear it?\n");
        gets (answer);

        if      (!strcmpi ("YES", answer))
        {
            /*Play_Song (node)*/;
        }
        else if (!strcmpi ("NO", answer))
        {
            printf ("So i hope you will listen it soon, cause it's really awesome\n");
        }
        else
        {
            Logs_Print (IMPOSSIBLE_COMMAND, tree->root);
        }
    }

    else if (!strcmpi ("NO", answer))
    {
        printf ("\nOh, but what did you mean then? Tell me your favorite song!\n");

        char* new_song = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (new_song);

        printf ("\nWhat's the difference with %s?\n", node->name);
        printf ("\n%s is not ", new_song);

        char* difference = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (difference);

        printf ("\nOk! Got you! Can you, please, share a link to this song on YouTube?\n");

        char* link = (char*) calloc (MAX_COMMAND_NAME, sizeof (char));
        gets (link);

        printf ("\nThank you! I'll wait for the next game with you!\n\n");

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

    return tree;
}


void Add_Song (char* new_song, char* difference, char* link, Node* node)
{
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


void Save_Changes (Node* tree, FILE* data_base)
{
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


Node* Node_Construct (Node* node)
{
    node = (Node*) calloc (1, sizeof (Node));
    *node = {};
    if (node == NULL)
        Logs_Print (MEM_ERROR, node);

    return node;
}
