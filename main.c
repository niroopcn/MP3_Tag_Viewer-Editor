#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

int formaterrorflag = 0;

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
	int ret = check_operation(argv[1]);
	switch (ret)
	{
	    case e_view:
		{
		    if (argc > 2)
		    {
			MP3Data view;
			if (read_and_validate_args(&view, argv, e_view) == e_success)
			{
			    if (open_mp3_file(&view, "r") == e_success)
			    {
				if (check_ID3_header(&view) == e_success)
				{
				    printf("%s detected\n", view.ID3_ver);
				    view_mp3_file_contents(&view);
				    fclose(view.fptr);
				}
			    }
			    else
				formaterrorflag = 1;
			}
			else
			    formaterrorflag = 1;
		    }
		    else
			formaterrorflag = 1;
		}
		break;
	    case e_edit:
		{
		    MP3Data edit;
		    if (argc > 4)
		    {
			if (read_and_validate_args(&edit, argv, e_edit) == e_success)
			{
			    if (open_mp3_file(&edit, "r+") == e_success)
			    {
				if (check_ID3_header(&edit) == e_success)
				{
				    printf("%s detected\n", edit.ID3_ver);
				    copy_mp3(&edit);
				    int pos = search_tag(&edit);

				    if (pos != -1)
					copy_and_replace(&edit, pos, argv[3]);
				    else
					fprintf(stderr, "[ERROR] Unable to modify, %s tag is not present inside this mp3 file\n", argv[2]);

				    char option;
				    printf("Do you want to view the modified contents (Y/y): ");
				    scanf("%c", &option);

				    if (option == 'Y' || option == 'y')
				    {
					fseek(edit.fptr, 10, SEEK_SET);
					view_mp3_file_contents(&edit);
				    }

				    system("rm temp");
				    fclose(edit.fptr);
				    fclose(edit.copy_fptr);
				}
			    }
			    else
				formaterrorflag = 1;
			}
			else
			    formaterrorflag = 1;
		    }
		    else
			formaterrorflag = 1;
		}
		break;
	    case e_help:
		{
		    printf("Usage:\n1. To View mp3 file contents: %s -v mp3_file_name\n", argv[0]);
		    printf("2. To Edit mp3 file contents: %s -e [OPTIONS] mp3_file_name\n   Available edit options are -t/-a/-A/-m/-y/-c\n", argv[0]);
		    printf("\t-t -> Modifies Title tag\n\t-a -> Modifies Artist tag\n\t-A -> Modifies Album tag\n\t-m -> Modifies Genre(Content) tag\n\t-y -> Modifies Year tag\n\t-c -> Modifies Comment tag\n");
		}
		break;
	    default:
		{
			fprintf(stderr, "[ERROR] %s is an unsupported format\n", argv[1]);
			formaterrorflag = 1;
		}
		break;
	}
    }
    else
    {
	fprintf(stderr, "[ERROR] %s: Invalid Arguments\n", argv[0]);
	formaterrorflag = 1;
    }

    if (formaterrorflag)
    {
	fprintf(stderr, "Usage:\nTo View, please pass: %s -v <.mp3_file>\n", argv[0]);
	fprintf(stderr, "To Edit, please pass: %s -e -t/-a/-A/-m/-y/-c changing_text <.mp3_file>\n", argv[0]);
	fprintf(stderr, "To get help, please pass: %s --help\n", argv[0]);
    }
}

TagType check(char *buffer)
{
    if (!strcmp(buffer, "TIT2"))
	return e_title;
    else if (!strcmp(buffer, "TPE1"))
	return e_artist;
    else if (!strcmp(buffer, "TALB"))
	return e_Album;
    else if (!strcmp(buffer, "TYER"))
	return e_year;
    else if (!strcmp(buffer, "TDRC"))
	return e_year;
    else if (!strcmp(buffer, "TCON"))
	return e_genre;
    else if (!strcmp(buffer, "COMM"))
	return e_comments;
	else
	return e_no_tag;
}

OperationType check_operation(char *str)
{
    if (!strcmp(str, "-v"))
	return e_view;
    else if (!strcmp(str, "-e"))
	return e_edit;
    else if (!strcmp(str, "--help"))
	return e_help;
    else
	return e_unsupported;
}

TagType check_tag_operation(char *str)
{
    if (!strcmp(str, "-t"))
	return e_title;
    else if (!strcmp(str, "-a"))
	return e_artist;
    else if (!strcmp(str, "-A"))
	return e_Album;
    else if (!strcmp(str, "-m"))
	return e_genre;
    else if (!strcmp(str, "-y"))
	return e_year;
    else if (!strcmp(str, "-c"))
	return e_comments;
    else
	return e_no_tag;
}

Status read_and_validate_args(MP3Data *structure, char *argv[], OperationType operation)
{
    switch (operation)
    {
	case e_view:
	    {
		char *ptr;
		if ((ptr = strstr(argv[2], ".mp3")) == NULL)// || strcmp(ptr, ".mp3"))
		    return e_failure;
		else
		{
		    structure->fname = argv[2];
		    return e_success;
		}
	    }
	    break;
	case e_edit:
	    {
		if ((structure->edit_tag = check_tag_operation(argv[2])) != e_no_tag)
		{
		    char *ptr;
		    if ((ptr = strstr(argv[4], ".")) == NULL || strcmp(ptr, ".mp3"))
		    {
			fprintf(stderr, "[ERROR] Please enter .mp3 files only\n");
			return e_failure;
		    }
		    else
		    {
			structure->fname = argv[4];
			return e_success;
		    }
		}
		else
		{
		    fprintf(stderr, "%s is an invalid option\n", argv[2]);
		    formaterrorflag = 1;
		}
	    }
	    break;
		default: break;
    }
	return e_failure;
}

Status open_mp3_file(MP3Data *structure, char *mode)
{
    structure->fptr = fopen(structure->fname, mode);

    // Do error handling
    if (structure->fptr == NULL)
    {
	perror("fopen");
	fprintf(stderr, "[ERROR] Please re-run the program and enter valid mp3 file\n");
	return e_failure;
    }

    return e_success;
}

Status check_ID3_header(MP3Data *structure)
{
    char ID3[3];
    fread(ID3, sizeof(char), 3, structure->fptr);
    if (strcmp(ID3, "ID3"))
    {
	fprintf(stderr, "[ERROR] File is not mp3, as it doesn't contain ID3 tag!\n");
	return e_failure;
    }

    short version;
    void *ptr = &version;
    fread((char *)ptr + 1, sizeof(char), 1, structure->fptr);
    fread((char *)ptr, sizeof(char), 1, structure->fptr);

    if (version == 0x0300)
    {
	structure->ID3_ver = "ID3v2.3";
	fseek(structure->fptr, 5, SEEK_CUR);
	return e_success;
    }
    else if (version == 0x0400)
    {
	structure->ID3_ver = "ID3v2.4";
	fseek(structure->fptr, 5, SEEK_CUR);
	return e_success;
    }
    else
    {
	fprintf(stderr, "[ERROR] ID3 Metadata Container is not at v2.3 or v2.4, failed to retrieve data.\n");
	return e_failure;
    }
}
