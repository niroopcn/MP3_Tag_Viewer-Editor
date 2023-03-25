#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

void view_mp3_file_contents(MP3Data *view)
{
    int count = 0;
    for (int i = 0; i < 25; i++)
    {
        if(read_tag_and_size(view) == e_success)
        count++;

        free(view->str_buffer);

        if(count == 6)
        {
            break;
        }
    }
}

Status read_tag_and_size(MP3Data *view)
{
    char buffer[4];
    fread(buffer, sizeof(char), 4, view->fptr);

    fread( (char *)(&view->char_size) + 3, sizeof(char), 1, view->fptr);
    fread( (char *)(&view->char_size) + 2, sizeof(char), 1, view->fptr);
    fread( (char *)(&view->char_size) + 1, sizeof(char), 1, view->fptr);
    fread( (char *)(&view->char_size), sizeof(char), 1, view->fptr);

    fseek(view->fptr, 2, SEEK_CUR);

    view->str_buffer = malloc(view->char_size * sizeof(char));
    fread(view->str_buffer, sizeof(char), view->char_size, view->fptr);

    int success = 0;
    switch(check(buffer))
    {
        case e_title:
            printf("Title:\t  ");
            success = 1;
        break;
        case e_artist:
            printf("Artist:\t  ");
            success = 1;
        break;
        case e_Album:
            printf("Album:\t  ");
            success = 1;
        break;
        case e_year:
            printf("Year:\t  ");
            success = 1;
        break;
        case e_genre:
            printf("Genre:\t  ");
            success = 1;
        break;
        case e_comments:
            printf("Comments: ");
            success = 1;
        break;
        default: break;
    }

    if(success)
    {
        custom_print(view->str_buffer, view->char_size);
        return e_success;
    }
    else
        return e_failure;
}

void custom_print(char *str, int char_size)
{
    for(int i = 0; i < char_size; i++)
    {
        if(str[i] >= 32 && str[i] < 127)
        printf("%c", str[i]);
    }
    printf("\n");
}