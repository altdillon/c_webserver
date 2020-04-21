#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H
#define VERSION 0.79
#define PORT 8080
#define WEBROOT "./webroot"

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

#endif