#ifndef ENDPOINT_H
#define ENDPOINT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "httpcodes.h"
#define LISTSIZE 20
#define MAXARGS 40 
#define MAXRESSIZE 120
// post or get arguments can do up to MAXARGS arguments

/*
  NOTE: EXSAMPLE HEADER: void ledstate(const char *rec,char *res,char **args,int len)
  WHERE:
          "rec" is a raw thing that show's the raw text of the request 
          "res" is a 2d array that has a parsed list of the arguments 
          "len" is the number of arguments made to the callback
*/

size_t countEndPoints; // count of allocated endpoints

typedef struct 
{
  HTTPCODE_t codetype;
  char *path;
  void (*endpoint)(const char *,char *,char **,int);  
}endpoint_t;

endpoint_t endpointList[LISTSIZE];

int addEndPoint(void (*fun_ptr)(const char *,char *,char **,int),const char *path,HTTPCODE_t httptype);
int runEndPoint(char *request,char *responce,HTTPCODE_t httpcode); // run and endpoit
int getArgs(char *argstr,char **args);
int countArgs(char *args); // returns the number of args
int findEndPoint(char *endpoint,HTTPCODE_t httpcode); // do a linear search and return the index of an endpoint, -1 if not found
bool hasEndPoint(char *path,HTTPCODE_t httpcode); // see if the endpont exisits.  A wrapper for find end point


#endif
