#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "types.h"

typedef struct
{
    int char_size;
    FILE *fptr;
    FILE *copy_fptr;
    char *fname;
    char *ID3_ver;
    char *str_buffer;
    int edit_tag;
} MP3Data;

OperationType check_operation(char *str);

Status read_and_validate_args(MP3Data *structure, char *argv[], OperationType operation);

Status open_mp3_file(MP3Data *structure, char *mode);

void view_mp3_file_contents(MP3Data *view);

Status check_ID3_header(MP3Data *structure);

Status read_tag_and_size(MP3Data *view);

TagType check(char *buffer);

void custom_print(char *str, int char_size);

TagType check_tag_operation(char *str);

void copy_mp3(MP3Data *edit);

int search_tag(MP3Data *edit);

void copy_and_replace(MP3Data *edit, int pos, char *str);

#endif