#include "webpage.h"

bool hasFile(char *filename)
{
  bool fileExists = false;

  int fd;
  fd = open(filename,O_RDONLY,0);
  if(fd > 0)
  {
    fileExists = true;
    close(fd);
  }

  return fileExists;
}

// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html

int sendFile(int sockfd,char *resource)
{
  char *buffer;
  int linesRead = 0;
  int fd;
  
  // attempt to open the file
  fd = open(resource, O_RDONLY, 0);
  if(fd < 0)
  {
    return -1; // failed to open file, return early with an error
  }
  
  buffer = (char *) malloc(BUFFERSIZE); // create a buffer to read webpage into into

  int length = get_file_size(fd);
  // kill the function if we're greater than buffer size
  if(length > BUFFERSIZE)
  {
    free(buffer);
    return -1;
  }
  
  // read the resource to the buffer
  read(fd, buffer, length);  
  // send buffer to the client socket
  send(sockfd, buffer, length, 0);
  
  free(buffer); // finely, free up the buffer
  return linesRead;
}

/* This function accepts an open file descriptor and returns     
 * the size of the associated file.  Returns -1 on failure.
 */
int get_file_size(int fd) 
{
   struct stat stat_struct;

   if(fstat(fd, &stat_struct) == -1)
      return -1;
   return (int) stat_struct.st_size;
}

bool fileTooLarge(char *filename)
{
  bool isToLarge = false;
  int fd;  

  fd = open(filename, O_RDONLY, 0);
  if(fd < 0)
  {
    return true; // failed to open file, return early with an error
  }

  if(get_file_size(fd) > BUFFERSIZE)
  {
    isToLarge = true;
  }
  close(fd); // remeber to close the file descriptor
  
  return isToLarge;
}
