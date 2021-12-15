#pragma once
char source_code[] = "#include<stdio.h>\n#include<string.h>\n#include<stdarg.h>\n#include<stdint.h>\n#include<stdlib.h>\n#define OFFSET %d\n#define NDEBUG\ntypedef enum { false, true } bool;\n\nFILE *filein;                                       \n\nchar* multiStrcat(char* destination, int8_t num_of_strings, ...) {\n    va_list cat_str;\n    va_start(cat_str, num_of_strings);\n    for (int8_t i = 0; i < num_of_strings; i++)\n        strcat(destination, va_arg(cat_str,char*));\n    return destination;\n}\n\nvoid createFile(char *filename, int32_t len_of_file) {\n    FILE *tempfile;                                 \n    if (! (tempfile = fopen(filename, \"wb+\")) ) {\n        printf(\"Error creating %%s file\", filename);\n        exit(1);\n    }\n    char temp_char;                                 \n    printf(\"len_of_file = %d\\n\", len_of_file);\n    while(len_of_file--) {\n        fread(&temp_char, 1, 1, filein);\n        fwrite(&temp_char, 1, 1, tempfile);\n    }\n    fclose(tempfile);\n}\n\nint main(int argc, char **argv) {\n    char exec_name[50];                             \n    char exit_command[200] = \"\";                    \n    char * pointer;                                 \n    bool save = false;                              \n    int8_t pos_of_command = 0;                      \n#ifdef WIN32    \n    if(pointer = strrchr(argv[0], '\\\\'))\n#else           \n    if(pointer = strrchr(argv[0], '/'))\n#endif\n        strcpy(exec_name, ++pointer);\n    else\n        strcpy(exec_name, argv[0]);\n#ifdef WIN32    \n    if(!strrchr(exec_name, '.'))\n        strcat(exec_name, \".exe\");\n#endif\n\n    pointer = NULL;\n\n    if (argc == 2) {\n        if (!strcmp(argv[1], \"-s\")) {\n            save = true;\n        } else if (!(strcmp(argv[1], \"-h\")) || !(strcmp(argv[1], \"--help\"))) {\n            printf(multiStrcat(exit_command, 3, \"Usage: \", exec_name, \" [-s] [command after closing]\"));\n            return 1;\n        }\n        else {\n            pointer = argv[1];\n        }\n    } else if (argc == 3) {\n        \n        if (!strcmp(argv[2],\"-s\")) {\n            pointer = argv[1];\n            save = true;\n        } else if (!strcmp(argv[1],\"-s\")) {\n            pointer = argv[2];\n            save = true;\n        } else {\n            printf(multiStrcat(exit_command,3,\"Wrong arguments Usage: \", exec_name, \" [-s] [command after closing]\"));\n            return 1;\n        } \n\n    } else if (argc > 3) {\n        printf(multiStrcat(exit_command,3,\"Wrong arguments number Usage: \", exec_name, \" [-s] [command after closing]\"));\n        return 1;\n    }\n\n    #ifdef WIN32 \n        multiStrcat(exit_command, 2, \"taskkill /f /im \", exec_name);\n    #else\n        multiStrcat(exit_command, 2, \"pkill -KILL \", exec_name);\n    #endif\n\n    if (!save)\n    #ifdef WIN32                        \n        multiStrcat(exit_command, 2, \" && del /q \", exec_name);\n    #else                              \n        multiStrcat(exit_command, 2, \"&& rm \", exec_name);\n    #endif\n\n    if(pointer) \n        multiStrcat(exit_command, 2, \" && \", pointer);\n\n    filein = fopen(exec_name, \"rb\");\n\n    if (!filein) {\n        printf(\"Error opening %%s file\", exec_name);\n        return 1;\n    }\n\n    const int32_t start_point = OFFSET;             \n    fseek(filein, start_point, SEEK_SET);\n\n    int8_t num_of_files;                            \n    fread(&num_of_files, 1, 1, filein);\n\n    for (int8_t i = 0; i < num_of_files; i++) {\n        int8_t len_of_filename;                     \n        fread(&len_of_filename, 1, 1, filein);\n        char filename[len_of_filename + 1];         \n        fread(filename, 1, len_of_filename, filein);\n        filename[len_of_filename] = '\\0';           \n        int32_t len_of_file;                        \n        fread(&len_of_file, 4, 1, filein);\n        createFile(filename, len_of_file);          \n    }\n\n    fclose(filein);\n\n    if(pos_of_command) \n        multiStrcat(exit_command,2,\" && \", argv[pos_of_command]);\n        \n    system(exit_command);\n\n    return 0;\n}";