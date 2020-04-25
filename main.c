#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "main.h"
#include "webpage.h"
#include "endpoint.h"
//#include "debug.h"

// test main for the demo app
// prototypes for the demo app
void demoOnClick(const char *rec,char *res,char **args,int len);
void onMessage(const char *msg,char *res,char **args,int len);
void add(const char *msg,char *res,char **args,int len);
void ledstate(const char *rec,char *res,char **args,int len);

bool ledvalue;

int main()
{
    ledvalue = false;
    addEndPoint(&demoOnClick,"/onClick");
    addEndPoint(&onMessage,"/message");
    addEndPoint(&add,"/add");
    addEndPoint(&ledstate,"/ledstate");

    running = true;
    signal(SIGINT, handle_sigint); 
    // print out some opening information
    printf("crappy mini web server, version %.2f \n",VERSION);   
    printf("starting on port: %d with webroot: \"%s\" \n",PORT,WEBROOT);
    #ifdef MULTIPROC
    printf("MULTIPROCESS ENABLED IN THIS BUILD \n");
    #endif
    runServerLoop(); // start up a server loop
    
    return 0;
}

void ledstate(const char *rec,char *res,char **args,int len)
{
  if(len == 1)
  {
    char *mcd = strstr(args[0],"=")+1;
    if(strcmp(mcd,"on")==0)
    {
      ledvalue = true;
    }
    else if(strcmp(mcd,"off")==0)
    {
      ledvalue = false;
    }
  }
  else if(len == 0)
  {
    if(ledvalue == true)
    {
      strcpy(res,"led on");
    }
    else
    {
      strcpy(res,"led off");
    }
  }
}

void demoOnClick(const char *rec,char *res,char **args,int len)
{
  strcpy(res,"hello there!");
}

void onMessage(const char *msg,char *res,char **args,int len)
{
  char *st_a = strstr(args[0],"=");
  st_a += 1;
  sprintf(res,"hello %s ",st_a);
}

void add(const char *msg,char *res,char **args,int len)
{
  if(len == 2)
  {
    char *st_a = strstr(args[0],"=");
    char *st_b = strstr(args[1],"=");
    int a = atoi(strstr(st_a,"=")+1);
    int b = atoi(strstr(st_b,"=")+1);
    int c = a + b;
    //printf("[TEST ~LINE 57]a+b= %d\n",c);
    sprintf(res,"%d + %d = %d ",a,b,c);
    //sprintf(res,"%d",c);
  }
}

// Handler for SIGINT, caused by 
// Ctrl-C at keyboard 
void handle_sigint(int sig) 
{ 
    printf("[STATUS] SIGINT handled\n");
    serverRunning = 0; // stop the server
    stopServer();
}

// A function to display an error message and then exit
void fatal(char *message) 
{
   char error_message[100];

   strcpy(error_message, "[!!] Fatal Error ");
   strncat(error_message, message, 83);
   perror(error_message);
   exit(-1);
}

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size) 
{
   void *ptr;
   ptr = malloc(size);
   if(ptr == NULL)
      fatal("in ec_malloc() on memory allocation");
   return ptr;
}


