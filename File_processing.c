#include "Akinator.h"


size_t Size_File_Counter (const char* file_name)
{
    assert (file_name != NULL);

    size_t file_size = 0;
    struct stat buff;

    stat (file_name, &buff);

    file_size = buff.st_size;

    return file_size;
}


size_t Buff_Filler (char* point_buff, const char* file_name, size_t file_size)
{
    assert (point_buff != NULL);
    assert (file_name  != NULL);

    FILE* file_pointer = fopen(file_name, "r");

    size_t num_symbols = fread (point_buff, sizeof(char), file_size, file_pointer);;

    fclose (file_pointer);

    return num_symbols;
}


char* Buff_Maker (size_t file_size)
{
    char*  point_buff  = (char*) calloc (file_size, sizeof(char));
    assert (point_buff);
    return point_buff;
}
