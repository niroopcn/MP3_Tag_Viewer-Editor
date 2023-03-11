#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

void copy_mp3(MP3Data *edit)
{
    rewind(edit->fptr);
    edit->copy_fptr = fopen("temp", "w+");
    int ch;
    while ((ch = fgetc(edit->fptr)) != EOF)
        fputc(ch, edit->copy_fptr);
    rewind(edit->copy_fptr);
    fseek(edit->fptr, 10, SEEK_SET);
}

int search_tag(MP3Data *edit)
{
    char buffer[5] = {0};
    int i = 25;
    while (i--)
    {
        fread(buffer, sizeof(char), 4, edit->fptr);
        fread((char *)(&edit->char_size) + 3, sizeof(char), 1, edit->fptr);
        fread((char *)(&edit->char_size) + 2, sizeof(char), 1, edit->fptr);
        fread((char *)(&edit->char_size) + 1, sizeof(char), 1, edit->fptr);
        fread((char *)(&edit->char_size), sizeof(char), 1, edit->fptr);
        fseek(edit->fptr, 2, SEEK_CUR);

        if (edit->edit_tag == check(buffer))
        {
            printf("Modifying %s tag contents\n", buffer);
            return (int)ftell(edit->fptr);
        }
        else
            fseek(edit->fptr, edit->char_size, SEEK_CUR);
    }
    return -1;
}

void copy_and_replace(MP3Data *edit, int pos, char *str)
{
    fseek(edit->fptr, -6, SEEK_CUR);
    
    int len = strlen(str) + 1;
    fwrite((char *)(&len) + 3, sizeof(char), 1, edit->fptr);
    fwrite((char *)(&len) + 2, sizeof(char), 1, edit->fptr);
    fwrite((char *)(&len) + 1, sizeof(char), 1, edit->fptr);
    fwrite((char *)(&len), sizeof(char), 1, edit->fptr);
    
    fseek(edit->fptr, 2, SEEK_CUR);
    fseek(edit->copy_fptr, pos + edit->char_size, SEEK_SET);

    fputc(0, edit->fptr);

    for (int i = 0; i < len - 1; i++) //-1 to remove '\0' from being inserted
        fputc(str[i], edit->fptr);

    int ch;
    while ((ch = fgetc(edit->copy_fptr)) != EOF)
    {
        fputc(ch, edit->fptr);
    }
}
