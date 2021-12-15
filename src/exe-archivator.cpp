#include<cstring>
#include<fstream>
#include<windows.h>
#include"source_code.h"
#define MB 32*1024*1024
;
using namespace std;

long long filesize(FILE * &file) {
    fseek(file , 0 , SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

void writeFile(char * filename, FILE * &dest_file) {
    FILE * temp_file = fopen(filename, "rb");
    if (!temp_file) {
        printf("Error opening %s\n", filename);
        exit(1);
    }
    char len_of_filename = strlen(filename);
    long long len_of_file = filesize(temp_file);
    fwrite(&len_of_filename, 1, 1, dest_file);
    fwrite(filename, 1, len_of_filename, dest_file);
    fwrite(&len_of_file, 8, 1, dest_file);
    char temp_char;
    long long r_bytes;
    char* temp_chars = new char[MB]; 
    while (r_bytes = fread(temp_chars, 1, MB, temp_file)) 
        fwrite(temp_chars, 1, r_bytes, dest_file);
    delete[] temp_chars;
    fclose(temp_file);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: exe-archivator [-c <command> task to do after unpacking] [files to pack]\n");
        exit(1);
    }
    if (!(strcmp(argv[1], "-h")) || !(strcmp(argv[1], "--help"))) {
        printf("Usage: exe-archivator [-c <command> task to do after unpacking] [files to pack]\n");
        exit(1);
    }

    char num_of_files = argc - 1;

    char builtin_command[50] = "\r\n";

    char** files = argv + 1;

    if (!strcmp(argv[1], "-c")) {
        if (argc < 4) {
            printf("No Files to pack, [-h | --help] to see info\n");
            exit(1);
        }
        strcpy(builtin_command, argv[2]);
        files = argv + 3;
        num_of_files -= 2;
    }
    long long offset = 0;
    FILE * source = fopen("temp_a.c", "w+"), *compiled;

    if (!source) {
        printf("Error occured creating source temp.c file");
        return 1;
    }
    fprintf(source, source_code, offset);
    fclose(source);
    system("gcc -s -g0 temp_a.c -o temp_a.exe");
    while(!(compiled = fopen("temp_a.exe", "r"))) {

        Sleep(10);
    }
    offset = filesize(compiled);
    fclose(compiled);
    source = fopen("temp_a.c", "w+");
    fprintf(source, source_code, offset);
    fclose(source);
    system("gcc -s -g0 temp_a.c -o exec-me.exe");
    while(!(compiled = fopen("exec-me.exe", "rb+")))
        Sleep(10);
    system("del /q temp_a.c");
    system("del /q temp_a.exe");

    fseek(compiled , 0 , SEEK_END);
    if(strcmp(builtin_command, "\r\n")) {
        char size_of_command = - strlen(builtin_command);
        fwrite(&size_of_command, 1, 1, compiled);
        fwrite(builtin_command, 1, strlen(builtin_command), compiled);
    }
    fwrite(&num_of_files,1,1,compiled);
    for (int i = 0; i < num_of_files; i++) 
        writeFile(files[i], compiled);
    fclose(compiled);
    return 0;
}