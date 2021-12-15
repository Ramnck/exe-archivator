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
    // printf("%s's length is %ld\n", filename, len_of_file);
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
        printf("Usage: exe-archivator [files to pack]");
        exit(1);
    }
        
    long long offset = 0;
    // Определяем позицию байта с которой начинается информация (offset)
    FILE * source = fopen("temp_a.c", "w+"), *compiled;

    if (!source) {
        printf("Error occured creating source temp.c file");
        return 1;
    }
    // printf(source_code, offset);
    fprintf(source, source_code, offset);
    fclose(source);
    // printf("Now its gonna be gcc temp_a.exe\n");
    system("gcc -s -g0 temp_a.c -o temp_a.exe");
    while(!(compiled = fopen("temp_a.exe", "r"))) {

        // printf("wait\n");
        Sleep(10);
    }
    offset = filesize(compiled);
    fclose(compiled);
    // cout << offset;
    // Определили offset, теперь компилируем exe файл с макросом OFFSET равным найденному offset
    source = fopen("temp_a.c", "w+");
    fprintf(source, source_code, offset);
    fclose(source);
    system("gcc -s -g0 temp_a.c -o exec-me.exe");
    while(!(compiled = fopen("exec-me.exe", "rb+")))
        Sleep(10);
    system("del /q temp_a.c");
    system("del /q temp_a.exe");

    // Скомпилировали конечный исполняемый файл
    char num_of_files = argc - 1;
    fseek(compiled , 0 , SEEK_END);
    fwrite(&num_of_files,1,1,compiled);
    for (int i = 1; i <= num_of_files; i++) 
        writeFile(argv[i], compiled);
    fclose(compiled);
    return 0;
}