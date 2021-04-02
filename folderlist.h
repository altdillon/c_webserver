#ifndef FOLDERLIST_H
#define FOLDERLIST_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "severconfig.h"

bool isFolder(char *path); // determine if the path is a folder 
bool hasFolder(char *path); // determine if the folder exists 
int listFolders(char *fileNames); // get a list of folders 



#endif
