#ifndef LIBFS_H
#define LIBFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants
#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_FILE_SIZE 1024

// file system structures
typedef struct {
    char filename[MAX_FILENAME];
    int size;
    int is_open;
}FileEntry;

// function prototypes
int fileCreate(const char *filename);
int fileOpen(const char *filename);
int fileRead(int file_index, char *buffer, int buffer_size);
int fileWrite(int file_index, const char *data);
int fileClose(int file_index);
int fileDelete(const char *filename);

#endif