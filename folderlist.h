#ifndef FOLDERLIST_H
#define FOLDERLIST_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include "severconfig.h"

// macros for the HTML for the folder list page 


bool isFolder(char *path); // determine if the path is a folder 
bool hasFolder(char *path); // determine if the folder exists 
int listFolders(char *path,char **fileNames,size_t folderCount); // get a list of folders 
size_t countFolders(char *path); // figure how how many folders are in the sub folders


#endif
