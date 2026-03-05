#include "libFS.h"

// global variables
FileEntry file_table[MAX_FILES];
int file_count = 0;

// create a new file
int fileCreate(const char *filename){
    // check if the file already exists
    for (int i = 0; i < file_count; ++i){
        if (!strcmp(file_table[i].filename, filename)){
            printf("Error: file %s already exists.\n", filename);
            return -1;
        }
    }

    // create the file
    FILE *fptr = fopen(filename, "w");
    if (!fptr){
        printf("Error: Unable to create file %s.\n", filename);
        return -1;
    }
    fclose(fptr);

    // add file to file table
    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].size = 0;
    file_table[file_count].is_open = 0;
    file_count++;

    printf("File %s created successfully.\n", filename);
    return 0;
}

// Open a file
int fileOpen(const char *filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if (file_table[i].is_open) {
                printf("Error: File '%s' is already open.\n", filename);
                return -1;
            }
            file_table[i].is_open = 1;  // Mark file as open
            printf("File '%s' opened successfully.\n", filename);
            return i;  // Return file index
        }
    }

    printf("Error: File '%s' not found.\n", filename);
    return -1;
}

// Write data to a file
int fileWrite(int file_index, const char *data) {

    if (!file_table[file_index].is_open) {
        printf("Error: File '%s' is not open.\n", file_table[file_index].filename);
        return -1;
    }

    int data_size = strlen(data);

    // Write data to the local file
    FILE *file = fopen(file_table[file_index].filename, "w");
    if (!file) {
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }

    fwrite(data, 1, data_size, file);
    fclose(file);

    file_table[file_index].size = data_size;
    printf("Data written to file '%s' successfully.\n", file_table[file_index].filename);
    return 0;
}

int fileClose(int file_index){
    if (!file_table[file_index].is_open) {
        printf("Error: No files currently open.\n");
        return -1;
    }
    file_table[file_index].is_open = 0;  // Mark file as closed
    printf("File closed successfully.\n");
    return 0;
}

int fileDelete(const char *filename){
    for (int i = 0; i < file_count; i++) {
        if (!strcmp(file_table[i].filename, filename)) {
            if (remove(filename)){
                printf("Error: Unable to delete file.\n");
                return -1;
            }
            memcpy(&file_table[i], &file_table[i+1], (file_count - i - 1)*sizeof(FileEntry));
            printf("File '%s' deleted successfully.\n", filename);
            return 0;
        }
    }

    printf("Error: File '%s' not found.\n", filename);
    return -1;
}

int fileRead(int file_index, char *buffer, int buffer_size){
    FILE *fp = fopen(file_table[file_index].filename, "r");
    if (!fp){
        printf("Error: Unable to open file '%s' for writing.\n", file_table[file_index].filename);
        return -1;
    }
    char c;
    int i;
    for (i = 0; i < buffer_size - 1 && (c = fgetc(fp)) != EOF; ++i){
        buffer[i] = c;
    }
    buffer[i] = 0;
    printf("%s\n", buffer);
    return 0;
}
