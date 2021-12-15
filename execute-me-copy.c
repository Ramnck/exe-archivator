#include<stdio.h>
#include<string.h>
#include<stdarg.h>
#include<stdint.h>
#include<stdlib.h>
#define OFFSET 777
#define NDEBUG
typedef enum { false, true } bool;

                                        // Глобальный поток, хранящий собственный исполняемый файл
FILE *filein;                                       

char* multiStrcat(char* destination, int8_t num_of_strings, ...) {
                                        // Множественный strcat()
                                        // К строке destination добавляет в конец num_of_strings строк перечисленных после аргумента num_of_strings
                                        // Возвращает указатель на destination
    va_list cat_str;
    va_start(cat_str, num_of_strings);
    for (int8_t i = 0; i < num_of_strings; i++)
        strcat(destination, va_arg(cat_str,char*));
    return destination;
}

void createFile(char *filename, int32_t len_of_file) {
                                        // Так как поток filein - глобальный, то мы можем просто продолжать считывать из него len_of_file байт
                                        // Создаваемый файл
    FILE *tempfile;                                 
    if (! (tempfile = fopen(filename, "wb+")) ) {
                                        // В случае ошибки создания нового файла пишем сообщение об этом и завершаем выполнение программы
        printf("Error creating %s file", filename);
        exit(1);
    }
                                        // Временный байт
    char temp_char;                                 
                                        // len_of file раз считываем байт из filein и записываем его в tempfile
    printf("len_of_file = %d\n", len_of_file);
    while(len_of_file--) {
        fread(&temp_char, 1, 1, filein);
        fwrite(&temp_char, 1, 1, tempfile);
    }
                                        // Закрываем tempfile
    fclose(tempfile);
}

int main(int argc, char **argv) {
                                        // Строка для хранения собственного имени файла
    char exec_name[50] = "";                             
                                        // Пустая строка для выходной команды
    char exit_command[200] = "";                    
                                        // Указатель, который понадобится для определения собственного имени файла а также для контаксекации выходной команды к exit_command
    char * pointer;                                 
                                        // Булевая переменная, обозначающая, нужно ли удалять файл после завершения работы
    bool save = false;                              
                                        // Индекс строки, содержащей в себе Если один из двух аргументов командной строки - команда, исполняемая после завершения работы, то pos_of_command - индекс массива argv этой команды
    int8_t pos_of_command = 0;                      
                                        // Windows
#ifdef WIN32    
    if(pointer = strrchr(argv[0], '\\'))
                                        // Не Windows
#else           
    if(pointer = strrchr(argv[0], '/'))
#endif
        strcpy(exec_name, ++pointer);
    else
        strcpy(exec_name, argv[0]);
                                        // Windows
#ifdef WIN32    
                                        // Чтобы exec_name являлся названием исполняемого файла в Windows, в конце имени файла должно быть .exe
    if(!strrchr(exec_name, '.'))
        strcat(exec_name, ".exe");
#endif

    pointer = NULL;

    if (argc == 2) {
                                        // Если единственный аргумент командной строки является -s save = true
        if (!strcmp(argv[1], "-s")) {
            save = true;
        } else if (!(strcmp(argv[1], "-h")) || !(strcmp(argv[1], "--help"))) {
            printf(multiStrcat(exit_command, 3, "Usage: ", exec_name, " [-s] [command after closing]"));
            return 1;
        }
        else {
                                        // Иначе если этот аргумент не является параметром -s, записываем его в pointer
            pointer = argv[1];
        }
    } else if (argc == 3) {
                                        // Если аргумента командной строки 2, то узнаем, какой из аргументов - название команды,
                                        // выполняемой после завершения распаковки и записываем индекс аргумента в pos_of_command
        
        if (!strcmp(argv[2],"-s")) {
            pointer = argv[1];
            save = true;
        } else if (!strcmp(argv[1],"-s")) {
            pointer = argv[2];
            save = true;
        } else {
                                        // Если ни один из двух аргументов не -s тогда выводим сообщение об ошибке
            printf(multiStrcat(exit_command,3,"Wrong arguments Usage: ", exec_name, " [-s] [command after closing]"));
            return 1;
        } 

    } else if (argc > 3) {
                                        // Если количество аргументов больше допустимого, то выводим сообщение об ошибке 
        printf(multiStrcat(exit_command,3,"Wrong arguments number Usage: ", exec_name, " [-s] [command after closing]"));
        return 1;
    }

    #ifdef WIN32 
        multiStrcat(exit_command, 2, "taskkill /f /im ", exec_name);
    #else
        multiStrcat(exit_command, 2, "pkill -KILL ", exec_name);
    #endif

    if (!save)
                                        // Если файл не нужно оставлять после завершения работы, то записываем команду самоуничтожения в exit_command в соответствие с ОС
                                        // Windows
    #ifdef WIN32                        
        multiStrcat(exit_command, 2, " && del /q ", exec_name);
                                        // Не Windows
    #else                              
        multiStrcat(exit_command, 2, "&& rm ", exec_name);
    #endif

    if(pointer) 
        multiStrcat(exit_command, 2, " && ", pointer);

                                        // Открываем собственный исполняемый файл для чтения информации
    filein = fopen(exec_name, "rb");

    if (!filein) {
                                        // Завершаем выполнение программы и выводим сообщение об ошибке в случае ошибки открытия файла
        printf("Error opening %s file", exec_name);
        return 1;
    }

                                        // Порядковый номер байта, с которого начинается информация о файлах
    const int32_t start_point = OFFSET;             
    fseek(filein, start_point, SEEK_SET);

                                        // Количество записанных файлов
    int8_t num_of_files;                            
    fread(&num_of_files, 1, 1, filein);

    for (int8_t i = 0; i < num_of_files; i++) {
                                        // Длина имени файла
        int8_t len_of_filename;                     
        fread(&len_of_filename, 1, 1, filein);
                                        // Название файла
        char filename[len_of_filename + 1];         
        fread(filename, 1, len_of_filename, filein);
                                        // Добавляем \0 в конец filename так как fread не делает это автоматически
        filename[len_of_filename] = '\0';           
                                        // Длина файла
        int32_t len_of_file;                        
        fread(&len_of_file, 4, 1, filein);
                                        // Создаем файл, зная его имя и длину 
        createFile(filename, len_of_file);          
    }

    fclose(filein);

                                        // Если pos_of_command ненулевой, то записываем в конец exit_command нужную команду
    if(pos_of_command) 
        multiStrcat(exit_command,2," && ", argv[pos_of_command]);
        
                                        // Завершаем работу программы
    system(exit_command);

    return 0;
}