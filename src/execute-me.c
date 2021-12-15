#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdint.h>
#include<stdlib.h>
#define OFFSET 777
#define NDEBUG
typedef enum { false, true } bool;

FILE *filein;                                       

char* multiStrcat(char* destination, int8_t num_of_strings, ...) {
    va_list cat_str;
    va_start(cat_str, num_of_strings);
    for (int8_t i = 0; i < num_of_strings; i++)
        strcat(destination, va_arg(cat_str,char*));
    return destination;
}

void createFile(char *filename, int64_t len_of_file) {
    FILE *tempfile;                                 
    if (! (tempfile = fopen(filename, "wb+")) ) {
        printf("Error creating %s file", filename);
        exit(1);
    }
    char temp_char;
    while(len_of_file--) {
        fread(&temp_char, 1, 1, filein);
        fwrite(&temp_char, 1, 1, tempfile);
    }
    fclose(tempfile);
}

int main(int argc, char **argv) {
    char exec_name[50] = "";
    char cmd_command[50] = "\r";                             
    char exit_command[300] = "";                    
    char * pointer;                                 
    bool save = false;

    if(pointer = strrchr(argv[0], '\\'))
        strcpy(exec_name, ++pointer);
    else
        strcpy(exec_name, argv[0]);
    if(!strrchr(exec_name, '.'))
        strcat(exec_name, ".exe");

    pointer = NULL;

    filein = fopen(exec_name, "rb");

    const int64_t start_point = OFFSET;             
    fseek(filein, start_point, SEEK_SET);
    int8_t num_of_files;                            
    fread(&num_of_files, 1, 1, filein);
    
    if (num_of_files < 0) {
        fread(cmd_command, 1, -num_of_files, filein);
        cmd_command[num_of_files] = '\0';
        fread(&num_of_files, 1, 1, filein);
    }

    if (argc == 2) {
        if (!strcmp(argv[1], "-s")) {
            save = true;
        } else if (!strcmp(argv[1], "-c")) {
            strcpy(cmd_command, "\r");
        } else if (!(strcmp(argv[1], "-h")) || !(strcmp(argv[1], "--help"))) {
            printf(multiStrcat(exit_command, 3, "Usage: ", exec_name, " [-s] [-c <command> task to do after unpacking] (also use -c to block builtin command)"));
            return 1;
        } else {
            printf(multiStrcat(exit_command,3,"Wrong arguments Usage: ", exec_name, "-c [-s] [-c <command> task to do after unpacking] (also use -c to block builtin command)\n"));
        }
    
    } else if (argc == 3) {
        
        if ((!strcmp(argv[1],"-s") && !strcmp(argv[2],"-c")) || (!strcmp(argv[2],"-s") && !strcmp(argv[1],"-c"))) {
            strcpy(cmd_command, "\r");
            save = true;
        } else if (!strcmp(argv[1], "-c")) {
            strcpy(cmd_command, argv[2]);
        } else {
            printf(multiStrcat(exit_command,3,"Wrong arguments Usage: ", exec_name, "-c [-s] [-c <command> task to do after unpacking] (also use -c to block builtin command)\n"));
            return 1;
        } 

    } else if (argc == 4) {
        if (!strcmp(argv[1],"-s") && !strcmp(argv[2],"-c")) {
            strcpy(cmd_command, argv[3]);
            save = true;
        } else {
            printf(multiStrcat(exit_command,3,"Wrong arguments Usage: ", exec_name, "-c [-s] [-c <command> task to do after unpacking] (also use -c to block builtin command)\n"));    
        }
    } else if (argc > 4) {
        printf(multiStrcat(exit_command,3,"Wrong arguments number Usage: ", exec_name, "-c [-s] [-c <command> task to do after unpacking] (also use -c to block builtin command)\n"));
        return 1;
    }

        multiStrcat(exit_command, 2, "taskkill /f /im ", exec_name);

    if (!save)
        multiStrcat(exit_command, 2, " && del /q ", exec_name);

    if(strcmp(cmd_command, "\r")) 
        multiStrcat(exit_command, 2, " && ", cmd_command);

    if (!filein) {
        printf("Error opening %s file", exec_name);
        return 1;
    }

    printf("%s\n",exit_command);

    for (int8_t i = 0; i < num_of_files; i++) {
        int8_t len_of_filename;                     
        fread(&len_of_filename, 1, 1, filein);
        char filename[len_of_filename + 1];         
        fread(filename, 1, len_of_filename, filein);
        filename[len_of_filename] = '\0';
        int64_t len_of_file;                        
        fread(&len_of_file, 8, 1, filein);
        createFile(filename, len_of_file);          
    }

    fclose(filein);

    system(exit_command);

    return 0;
}