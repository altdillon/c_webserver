#ifndef DATALOGGING_H
#define DATALOGGING_H
#define MAXLOGSIZE 80
#define SAVEDENTRIES 100
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "severconfig.h"

void logmsg(char *msg);

#ifdef SAVELOGFILE
size_t enteryCount;                           // count for entries
char *logdata[SAVEDENTRIES];                  // log for saved files
int saveLogFile(const char *path,char *msg);  // append a a log file string to the end of a log file
bool fileExists(const char *path);            // return true if the file exists
void initLogArray();                          // allocate memory for the log data array
void clearArray();                            // clear all the data
void makeHeaderString(char *logheadr);        // generate a header string
int initFile(const char *path);              // make an empty header file and delete the old one if it exsists
void initLogging();                           // make an empty log file

#endif

#endif
