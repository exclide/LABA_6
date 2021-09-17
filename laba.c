#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc <= 3)
    {
        printf("Correct usage: \n--file <name.arc> --extract\n--file <name.arc> --list \n--file <name.arc> --create <file1> <file2> ... <filen>\n");
        return 1;
    }
    char** FILES = NULL;
    char* FILE_NAME = NULL;
    int extract = 0, list = 0, create = 0, k = 0;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--file"))
        {
            if (i+1<argc) 
            {
                FILE_NAME = (char*)malloc(strlen(argv[i+1])+1);
                strcpy(FILE_NAME, argv[i+1]);
            }
            else 
            {
                printf("Wrong format");
                return 1;
            }
        }
        else if (!strcmp(argv[i], "--extract"))
        {
            extract = 1;
        }
        else if (!strcmp(argv[i], "--list"))
        {
            list = 1;
        }
        else if (!strcmp(argv[i], "--create"))
        {
            create = 1;
        }
        else if(!strstr(argv[i], ".arc"))
        {
            if (FILES == NULL) FILES = (char**)malloc((argc-4)*sizeof(char*));
            int len = strlen(argv[i]);
            FILES[k] = (char*)malloc(len);
            strcpy(FILES[k], argv[i]);
            printf("file %s\n", FILES[k]);
            k++;
        }
    }
    /*
    char* files[3];
    files[0] = "01.World In My Eyes.mp3";
    files[1] = "photo_2021-02-13_12-53-27.jpg";
    files[2] = "problems1.pdf";
    */

    
    //.arc 3 files
    //FILE* arc = fopen("test.arc", "wb");
    //total metadata size in bytes
    //size of file #1   //long long
    //strlen of file #1 //int
    //name of file #1

    //size of file #2
    //strlen of file #2
    //name of file #2
    //...
    FILE* t;
    FILE* arc;
    if (create)
    {
    arc = fopen(FILE_NAME, "wb");
    if (!arc)
    {
        printf("Failed to open file, check name");
        return 1;
    }
    int file_count = argc-4;
    long long* files_size = (long long*)malloc(file_count*sizeof(long long));
    long long total_strlen = 0;
    for (int i = 0; i < file_count; i++)
    {
        t = fopen(FILES[i], "rb");
        if (!t)
        {
            printf("Can't open file");
            return 1;
        }
        fseek(t, 0L, SEEK_END);
        files_size[i] = ftell(t);
        total_strlen += strlen(FILES[i]);
        fclose(t);
    }
    long long total_meta_size = 12 + total_strlen + 12 * file_count; //METASIZE, FILECOUNT
    //METADATA_SIZE SIZEOFFILE1 STRLENOFFILE1 STR SIZEOFFILE2 STRLENOFFILE2 STR
    fwrite(&total_meta_size, sizeof(long long), 1, arc); //write full meta size
    fwrite(&file_count, sizeof(int), 1, arc);
    for (int i = 0; i < file_count; i++)
    {
        fwrite(&files_size[i], sizeof(long long), 1, arc); //write file #i size
        int file_len = strlen(FILES[i]);
        fwrite(&file_len, sizeof(int), 1, arc), //write filename #i length
        fwrite(FILES[i], 1, file_len, arc); //write filename #i name string
    }
    //WRITE FILE CONTENTS RIGHT AFTER
    for (int i = 0; i < file_count; i++)
    {
        t = fopen(FILES[i], "rb");
        if (!t)
        {
            printf("Can't open file");
            return 1;
        }
        char* buf = (char*)malloc(files_size[i]);
        fread(buf, 1, files_size[i], t);
        fwrite(buf, 1, files_size[i], arc);
        free(buf);
    }
    fclose(arc);
    }
    //NOW READ ARC AND EXTRACT FILES
    //READ FULL META SIZE
    //READ FILE COUNT
   //arc = fopen("test.arc", "rb");
    if (list || extract)
    {
    arc = fopen(FILE_NAME, "rb");
    if (!arc)
    {
        printf("Can't open file");
        return 1;
    }
    long long FULL_META_SIZE;
    int FILE_COUNT;
    fread(&FULL_META_SIZE, 1, sizeof(long long), arc);
    fread(&FILE_COUNT, 1, sizeof(int), arc);
    long long* FILE_SIZES = (long long*)malloc(FILE_COUNT*sizeof(long long));
    int* FILE_LENS = (int*)malloc(FILE_COUNT*sizeof(int));
    char** FILE_STRS = (char**)malloc(FILE_COUNT*sizeof(char*));
    for (int i = 0; i < FILE_COUNT; i++)
    {
        //READ SIZE OF FILE 1
        //READ LEN OF STR 1
        //READ STR
        fread(&FILE_SIZES[i], sizeof(long long), 1, arc);
        fread(&FILE_LENS[i], sizeof(int), 1, arc);
        FILE_STRS[i] = (char*)malloc(FILE_LENS[i]+1);
        fread(FILE_STRS[i], 1, FILE_LENS[i], arc);
        FILE_STRS[i][FILE_LENS[i]] = '\0';
        printf("File #%d size: %d strlen: %d str: %s\n", i, FILE_SIZES[i], FILE_LENS[i], FILE_STRS[i]);

    }
    //SKIP META
    fseek(arc, FULL_META_SIZE, 0);
    //READ FILES AND WRITE ONE BY ONE
    if (extract)
    {
    for (int i = 0; i < FILE_COUNT; i++)
    {
        t = fopen(FILE_STRS[i], "wb");
        if (!t)
        {
            printf("Can't open file");
            return 1;
        }
        char* buf = (char*)malloc(FILE_SIZES[i]);
        fread(buf, 1, FILE_SIZES[i], arc);
        fwrite(buf, 1, FILE_SIZES[i], t);
        fclose(t);
    }
    }
    }

    return 0;
}