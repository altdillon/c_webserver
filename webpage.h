#ifndef WEBPAGE_H
#define WEBPAGE_H
//#define BUFFERSIZE 1000000 // moved to server config
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include "severconfig.h"

int readFile(char *filename,char **textBuffer); // read a file to a text buffer, return number of lines or -1 if can't open file
int sendFile(int sockfd,char *resource);        // just read the file out to the out buffer
int get_file_size(int fd);                      // return the size in bytes of the file
bool hasFile(char *filename);
bool fileTooLarge(char *filename);

#endif
