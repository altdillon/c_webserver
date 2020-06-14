#include "datalogging.h"

void logmsg(char *msg)
{
  #ifndef SAVELOGFILE
  printf("%s",msg);
  #else
  saveLogFile(LOGPATH,msg);
  #endif
}

#ifdef SAVELOGFILE
int saveLogFile(const char *path,char *msg)
{
  int fd;
  // open the file for logging
  fd = open(path,O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
  if(fd == -1)
  {
    return -1; // fail silently
  }  

  // write the log string to the log file
  size_t logsize = strlen(msg);
  if(logsize > MAXLOGSIZE)
  {
    return -1;
  }

  int s = write(fd,msg,logsize);
  if(s < 0)
  {
    return -1;
  }
  
  close(fd);

  return 0;
}

bool fileExists(const char *path)
{
  bool isthere = true;
  int fd;

  fd = open(path,O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
  if(fd == -1)
    isthere = false;
  else
    close(fd);

  return isthere;
}

void makeHeaderString(char *logheadr)
{
  int hours,minutes,seconds,day,month,year;
  const char *welcome = "wsheaderfile";
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);
  
}

#endif
