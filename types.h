#ifndef TYPES_H
#define TYPES_H

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported
} OperationType;

typedef enum
{
    e_artist,
    e_title,
    e_Album,
    e_year,
    e_genre,
    e_comments,
    e_no_tag
} TagType;

#endif