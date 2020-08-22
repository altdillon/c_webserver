#include "endpoint.h"


addEndPoint(void (*endpoint)(const char *,char *,char **,int),const char *path,HTTPCODE_t httptype)
{
  int res = -1;
  
  if((countEndPoints+1) < LISTSIZE)
  {
    endpointList[countEndPoints].path = path;
    endpointList[countEndPoints].endpoint = endpoint;
    endpointList[countEndPoints].codetype = httptype;
    res = 0;
    countEndPoints++;
  }
  
  return res;
}

bool hasEndPoint(char *path)
{
  // get the name of the endpoint out of the path
  size_t epAlloc = 80;
  char endpoint_str[epAlloc];
  memset(endpoint_str,'\0',epAlloc);
  char *ch = strchr(path,'?');  
  
  if(ch != NULL)
  {
    //char *arglist = strtok(ch,"&");
    //int numargs = countArgs((arglist+1));
    size_t endpoint_len = strlen(path) - strlen(ch);
    strncpy(endpoint_str,path,endpoint_len);  
  }
  else
  {
    strcpy(endpoint_str,path);
  }

  //return false;
  bool hasPoint = false;
/*
  size_t i;
  for(i=0;i<countEndPoints;i++)
  {
    if(strcmp(endpointList[i].path,endpoint_str) == 0)
    {
      hasPoint = true;
      break;
    }
  }
*/
  if(findEndPoint(endpoint_str) >= 0)
  {
    hasPoint = true;
  }
  
  return hasPoint;
}

int countArgs(char *args)
{
  int count = 0;
  size_t i;
  for(i=0;i<strlen(args);i++)
  {
    if(args[i] == '&')
    {
      count++;
    }
  }

  return count;
}

// count the number of arguments
int getArgs(char *argstr,char **args)
{
  int argc = 0; // count of tokens  
  char *argtok = strtok(argstr,"&");
  while(argtok != NULL)
  {
    strcpy(args[argc],argtok);
    argtok = strtok(NULL,"&");
    argc++;
  }
  
  return argc;
}

int runEndPoint(char *request,char *responce)
{
  int s = 0; // value for sucess
  // define the name of the endpoint
  size_t epAlloc = 80;
  char endpoint_str[epAlloc]; // string for the path
  char arg_str[epAlloc]; // string for the arguments
  char responceBuffer[MAXRESSIZE]; // buffer for the responce
  // make sure the memmory
  memset(endpoint_str,'\0',epAlloc);
  memset(arg_str,'\0',epAlloc);
  // copy the args out of the path
  char *args = strchr(request,'?'); 
  if(args != NULL)
  {
    size_t endpoint_len = strlen(request) - strlen(args);
    strncpy(endpoint_str,request,endpoint_len);
    strncpy(arg_str,(args+1),(strlen(args)-1));  
  }
  else
  {
    strcpy(endpoint_str,request);
  }

  // get a list of args
  // allocate a list doing things in the crazyist way possible
  // not a huge fan of doing this this way, but I think it makes sence in this context
  char *argList[MAXARGS];
  size_t i;
  for(i=0;i<MAXARGS;i++)
  {
    argList[i] = (char*)malloc(epAlloc);
  }  
  
  // dump the arg list into an array of endpoints
  int argCount = getArgs(arg_str,argList);

  // just assume that the path exists, but check the names to be safe
  // this whole thing is gonzo 
  int endPointIndex = findEndPoint(endpoint_str);
  if(endPointIndex >= 0) // just to be safe, rember that zero is also a valid answer
  {
    (*endpointList[endPointIndex].endpoint)(request,responceBuffer,argList,argCount); 
    s = 0;
  }

  // clean up after everything is done
  size_t j;
  for(j=0;j<MAXARGS;j++)
  {
    free(argList[j]);
  } 
  
  strcpy(responce,responceBuffer); // copy the final value for the output over
  return s; 
}

int findEndPoint(char *endpoint)
{
  int epIndex = -1;

  size_t i;
  for(i=0;i<countEndPoints;i++)
  {
    if(strcmp(endpointList[i].path,endpoint) == 0)
    {
      epIndex = i;
      break;
    }
  }

  return epIndex;
}
