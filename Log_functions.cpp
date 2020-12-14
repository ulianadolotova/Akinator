#include "Akinator.h"


void Tree_Print (Node* tree)
{
    FILE* Tree_image = fopen ("Tree_image.txt", "w");

    fprintf (Tree_image, "digraph G {\n");
    fprintf (Tree_image, "node [shape = \"record\", style = \"filled\", fillcolor = \" #E7ECFF\"]\n");

    fprintf (Tree_image, "\"%p\" [fillcolor = \"#E7ECFF\", label = \"{%s|{%s}}\"]\n", tree, tree->name, tree->link);

    if (tree->negative_child != NULL)
    {
        Node_Print (tree->negative_child, Tree_image);
        fprintf (Tree_image, "\"%p\" -> \"%p\"\n", tree, tree->negative_child);
    }

    if (tree->positive_child != NULL)
    {
        Node_Print (tree->positive_child, Tree_image);
        fprintf (Tree_image, "\"%p\" -> \"%p\"\n", tree, tree->positive_child);
    }

    fprintf (Tree_image, "}");

    fclose (Tree_image);

    system ("dot -Tjpeg Tree_image.txt > Tree_image.jpeg");
    system ("start Tree_image.jpeg");
}

void Node_Print (Node* tree, FILE* Tree_image)
{
    fprintf (Tree_image, "\"%p\" [color =\"#000800\", label = \"{%s|{%s}}\"]\n", tree, tree->name, tree->link);
    fprintf (Tree_image, "\"%p\" -> \"%p\"\n", tree, tree->parent);

    if (tree->negative_child != NULL)
    {
        Node_Print (tree->negative_child, Tree_image);
        fprintf (Tree_image, "\"%p\" -> \"%p\"\n", tree, tree->negative_child);
    }

    if (tree->positive_child != NULL)
    {
        Node_Print (tree->positive_child, Tree_image);
        fprintf (Tree_image, "\"%p\" -> \"%p\"\n", tree, tree->positive_child);
    }
}


void Logs_Print (int code_error, Node* tree)
{
    if (tree != NULL)
        Tree_Print (tree);

    //FILE* Akinator_logs = fopen ("Akinator_logs.txt", "w");
    FILE* Akinator_logs = stderr;

    switch (code_error)
    {
        case MEM_ERROR:          fprintf (Akinator_logs, "\nError: memory limit.\n");
                                 break;

        case INCORRECT_FORMAT:   fprintf (Akinator_logs, "\nError: incorrect format of data_base.\n");
                                 break;

        case IMPOSSIBLE_COMMAND: fprintf (Akinator_logs, "\nError: incorrect format of entered command.\n");
                                 break;

        case NULL_POINTER:       fprintf (Akinator_logs, "\nError: trying to go after null pointer.\n");
                                 break;
    }

    assert (!"OK");

}



