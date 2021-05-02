#include "folderlist.h"

bool isFolder(char *path)
{
  // figure out the file path
  bool hasFolder = false;
  //char nameBuff[80] = WEBROOT;
  //strcat(nameBuff,path); // add a . to the request path so it know's where we are  

  // figure out if the path being called already contrains webroot information
  char folderPath[80];
  strcpy(folderPath,WEBROOT);
  int stp = strstr(path,folderPath); // use strstr to see if we have the string in question
  if(stp)
  {
    strcpy(folderPath,path); // if we have it than just copy path into folder with
  }
  else // if we don't than just tack the address onto the end webroot
  {
    strcat(folderPath,path);
  }
  

  DIR* dir = opendir(folderPath);
  if(dir)
  {
    hasFolder = true;
    closedir(dir);
  }
  //else if(dir == errno)
  //{

  //}

  return hasFolder; // stub
}

size_t countFolders(char *path)
{
  size_t subFolderCount = 0; // counter for sub folders in the file
  // build up the path
  //char namePath[80] = WEBROOT;
  //strcat(namePath,path);
  // open an instance of the folder
  DIR *dir = opendir(path);
  if(dir != NULL) // make sure it's actually open
  {
    struct dirent *de;  // Pointer for directory entry
    while ((de = readdir(dir)) != NULL) // while the current folder isn't NULL...
    {
      subFolderCount++; // increment the counter
    }
    closedir(dir);
  }    


  return subFolderCount;
}

int listFolders(char *path,char **fileNames,size_t folderCount)
{
  struct dirent *de; 
  DIR *dr = opendir(path);
  
  if(dr == NULL)
  {
    return -1; 
  }
  
  size_t index = 0;
  while ((de = readdir(dr)) != NULL)
  {
    if(index < folderCount)
    {
      strcpy(fileNames[index],de->d_name);
      index++;
    }
  }

  return 0;
}
