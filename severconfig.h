#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
#define VERSION 1.21
#define PORT 8080
#ifndef WEBROOT
#define WEBROOT "./webroot"
#define INDEX "index.html"
#endif
#define DEFULT_MAIN

// macros for handling signals
#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */
#define SIGKILL 9   /* Kill the process */
// macros for controlling options durring debug
#define DEBUG
// enable multiprocess
//#define MULTIPROC
// define a path for the log file as well as the buffer size for sending files (size defined in bytes)
#define LOGPATH "/tmp/webserver.log" 
#define BUFFERSIZE 1000000
#define SAVELOGFILE

#endif
