#include<cstring>
#include<fstream>
#include<windows.h>
#include"source_code.h"

using namespace std;

long filesize(FILE * &file) {
    fseek(file , 0 , SEEK_END);                          // устанавливаем позицию в конец файла
    long size = ftell(file);
    rewind(file);
    return size;
}

void writeFile(char * filename, FILE * &dest_file) {
    FILE * temp_file = fopen(filename, "rb");
    char len_of_filename = strlen(filename);
    int len_of_file = filesize(temp_file);
    fwrite(&len_of_filename, 1, 1, dest_file);
    fwrite(filename, 1, len_of_filename, dest_file);
    fwrite(&len_of_file, 4, 1, dest_file);
    char temp_char;
    while (len_of_file--) {
        fread(&temp_char, 1, 1, temp_file);
        fwrite(&temp_char, 1, 1, dest_file);
    }
    fclose(temp_file);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: exe-archivator [files to pack]");
        exit(1);
    }
        
    int offset = 0;
    // Определяем позицию байта с которой начинается информация (offset)
    FILE * source = fopen("temp_a.c", "w+"), *compiled;
    fprintf(source, source_code, offset);
    fclose(source);
    system("gcc -s -g0 temp_a.c -o temp_a.exe");
    while(!(compiled = fopen("temp_a.exe", "r")))
        Sleep(10);
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
    system("del temp_a.c");
    system("del temp_a.exe");
    // Скомпилировали конечный исполняемый файл
    char num_of_files = argc - 1;
    fseek(compiled , 0 , SEEK_END);
    fwrite(&num_of_files,1,1,compiled);
    for (int i = 1; i <= num_of_files; i++) 
        writeFile(argv[i], compiled);
    
    // fwrite(data, 1, sizeof(data), ofile);
    // strcat(str, "asdkuj");
    // cout << str;
    // system("echo off");
    // system(argv[1]);
    // fclose(source);
    fclose(compiled);
    // fclose(source);
    return 0;
}