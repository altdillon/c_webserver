#ifndef ENDPOINT_H
#define ENDPOINT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define POST 1
#define GET 2
#define LISTSIZE 20
#define MAXARGS 40 
#define MAXRESSIZE 120
// post or get arguments can do up to MAXARGS arguments


size_t countEndPoints; // count of allocated endpoints

typedef struct 
{
  char *path;
  void (*endpoint)(const char *,char *,char **,int);  
}endpoint_t;

endpoint_t endpointList[LISTSIZE];

int addEndPoint(void (*fun_ptr)(const char *,char *,char **,int),const char *path);
int runEndPoint(char *request,char *responce); // run and endpoit
int getArgs(char *argstr,char **args);
int countArgs(char *args); // returns the number of args
int findEndPoint(char *endpoint); // do a linear search and return the index of an endpoint, -1 if not found
bool hasEndPoint(char *path); // find a path


#endif
