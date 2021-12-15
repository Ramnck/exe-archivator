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
    char exit_command[200] = "";                    
    char * pointer;                                 
    bool save = false;                              
    int8_t pos_of_command = 0;                      
    if(pointer = strrchr(argv[0], '\\'))
        strcpy(exec_name, ++pointer);
    else
        strcpy(exec_name, argv[0]);
    if(!strrchr(exec_name, '.'))
        strcat(exec_name, ".exe");

    pointer = NULL;

    if (argc == 2) {
        if (!strcmp(argv[1], "-s")) {
            save = true;
        } else if (!(strcmp(argv[1], "-h")) || !(strcmp(argv[1], "--help"))) {
            printf(multiStrcat(exit_command, 3, "Usage: ", exec_name, " [-s] [command after closing]"));
            return 1;
        }
        else {
            pointer = argv[1];
        }
    } else if (argc == 3) {
        
        if (!strcmp(argv[2],"-s")) {
            pointer = argv[1];
            save = true;
        } else if (!strcmp(argv[1],"-s")) {
            pointer = argv[2];
            save = true;
        } else {
            printf(multiStrcat(exit_command,3,"Wrong arguments Usage: ", exec_name, " [-s] [command after closing]"));
            return 1;
        } 

    } else if (argc > 3) {
        printf(multiStrcat(exit_command,3,"Wrong arguments number Usage: ", exec_name, " [-s] [command after closing]"));
        return 1;
    }

        multiStrcat(exit_command, 2, "taskkill /f /im ", exec_name);

    if (!save)
        multiStrcat(exit_command, 2, " && del /q ", exec_name);

    if(pointer) 
        multiStrcat(exit_command, 2, " && ", pointer);

    filein = fopen(exec_name, "rb");

    if (!filein) {
        printf("Error opening %s file", exec_name);
        return 1;
    }

    const int64_t start_point = OFFSET;             
    fseek(filein, start_point, SEEK_SET);

    int8_t num_of_files;                            
    fread(&num_of_files, 1, 1, filein);

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

    if(pos_of_command) 
        multiStrcat(exit_command,2," && ", argv[pos_of_command]);
        
    system(exit_command);

    return 0;
}