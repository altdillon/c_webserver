#include "server.h"
#include "debug.h"
#include "httpcodes.h"
#include "severconfig.h"
#include "endpoint.h"
#include "datalogging.h"
#include "folderlist.h"


/* This function accepts a socket FD and a ptr to the null terminated
 * string to send.  The function will make sure all the bytes of the
 * string are sent.  Returns 1 on success and 0 on failure.
 */
int send_string(int sockfd, unsigned char *buffer) 
{
   int sent_bytes, bytes_to_send;
   bytes_to_send = strlen(buffer);
   while(bytes_to_send > 0) {
      sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
      if(sent_bytes == -1)
         return 0; // return 0 on send error
      bytes_to_send -= sent_bytes;
      buffer += sent_bytes;
   }
   return 1; // return 1 on success
}

/* This function accepts a socket FD and a ptr to a destination
 * buffer.  It will receive from the socket until the EOL byte
 * sequence in seen.  The EOL bytes are read from the socket, but
 * the destination buffer is terminated before these bytes.
 * Returns the size of the read line (without EOL bytes).
 */
int recv_line(int sockfd, unsigned char *dest_buffer) 
{
    #define EOL "\r\n" // End-Of-Line byte sequence
    #define EOL_SIZE 2
   unsigned char *ptr;
   int eol_matched = 0;

   ptr = dest_buffer;
   while(recv(sockfd, ptr, 1, 0) == 1) { // read a single byte
      if(*ptr == EOL[eol_matched]) { // does this byte match terminator
         eol_matched++;
         printf("debug: %02x\n",ptr);
         if(eol_matched == EOL_SIZE) { // if all bytes match terminator,
            *(ptr+1-EOL_SIZE) = '\0'; // terminate the string
            return strlen(dest_buffer); // return bytes recevied
         }
      } else {
         eol_matched = 0;
      }   
      ptr++; // increment the pointer to the next byter;
   }
   return 0; // didn't find the end of line characters
}


/*
*
*   Run the server in a loop
*   ...But in a server
*
*/

void runDeamonServerLoop()
{
  printf("%s\n","starting webserver as a background process"); // you know, just let the user know...
  //running = true; // running
  serverRunning = 1;
  int sockfd, new_sockfd, yes=1;
  struct sockaddr_in host_addr, client_addr; // local address information
  socklen_t sin_size;
  char messages[80]; // string for messages  

  // start the webserver as a deamon
  pid_t pid,sid;
  // start a new process and make sure it works
  pid = fork();
  if(pid < 0)
  {
    logmsg("[ERROR] failed to start deamon process\n");
    exit(1);
  }
  else
  {
    printf("deamon process started with PID: %d \n",pid);
  }

  // kill the parrent process and disconnect from the terminal
  if(pid > 0)
  {
    exit(0);
  }

  umask(0); // set umask to zero
  // get a new session from the os for the child pid
  sid = setsid();
  if(sid < 0)
  {
    logmsg("[ERROR] failed to set child pid\n");
    exit(3);
  }
  // change to current directory
  //if(chdir("/") < 0){
  //  logmsg("[ERROR] failed to change directory\n");
  //  exit(4);
  //}
  
  // close the standeard descripters 
  close(STDIN_FILENO); // No more listening.
  close(STDOUT_FILENO); // No more talking.
  close(STDERR_FILENO); // No more failing (or so...)   

  // log that we've started the web server...
  sprintf(messages,"[STATUS] server deamon started with pid status: %d and process status id: %d\n",pid,sid);
  logmsg(messages);

  // setup the web server
  sprintf(messages,"[INFO] starting socket on port: %d \n",PORT);
  logmsg(messages);

 // OK! Now we do something kind of fishy... Now that the server if effectivly running as a deamon we can just start start the server as we normally would
  logmsg("[INFO] now starting webserver like normal... \n");
  runServerLoop();

}


/*
*   Run the server in a loop 
*/

void runServerLoop()
{
  //running = true; // running
  serverRunning = 1;
  int sockfd, new_sockfd, yes=1;
  struct sockaddr_in host_addr, client_addr; // local address information
  socklen_t sin_size;
  char messages[80]; // string for messages  

    // init the log file
  #ifdef SAVELOGFILE
  // make sure to init the log file
  if(initFile(LOGPATH) == -1)
  {
    printf("%s\n","failed to start log file"); // no point in using the error system
  }
  #endif

  // do some stuff to init the web server
  //printf("[STATUS] starting socket on port: %d \n",PORT);
  sprintf(messages,"[STATUS] starting socket on port: %d \n",PORT);
  logmsg(messages);

  // attempt to setup a new socket and see if it failed
  sockfd = socket(PF_INET,SOCK_STREAM,0);
  if(sockfd == -1)
  {
    //printf("[ERROR] failed to creat socket errorcode: %d\n",sockfd);
    sprintf(messages,"[ERROR] failed to creat socket errorcode: %d\n",sockfd);
    logmsg(messages);
  }
  
  // set the socket options, tell the user if that failed
  if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
  {
    //printf("[ERROR] failed to set socket option SO_REUSEADDR\n");
    sprintf(messages,"[ERROR] failed to set socket option SO_REUSEADDR\n");
    logmsg(messages);
  }
  
  // set some more options
  host_addr.sin_family = AF_INET; // host byte order
  host_addr.sin_port = htons(PORT); // short, network byte order
  host_addr.sin_addr.s_addr = INADDR_ANY; // automaticly fill with local ip
  memset(&(host_addr.sin_zero),'\0',8); // zero out the rest of the struct
  
  // bind to the host ip address and start listening
  if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
  {
    //printf("[ERROR] failed to bind socket to port \n");
    sprintf(messages,"[ERROR] failed to bind socket to port \n");
    logmsg(messages);
  }

  if(listen(sockfd,20) == -1)
  {
    //printf("[ERROR] failed to start listening on socket");
    logmsg("[ERROR] failed to start listening on socket");
  }
  else
  {
    //printf("[STATUS] now waiting for connections...\n");
    logmsg("[INFO] getting ready to the start the server...\n");
  }
  
  // main loop of the server
  //while(running)
  while(serverRunning)
  {
    sin_size = sizeof(struct sockaddr_in);
    logmsg("[STATUS] now waiting for connections...\n");
    new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
    
    if(new_sockfd == -1)
    {
      //printf("[ERROR] failed to accpet incoming socket\n");
      logmsg("[ERROR] failed to accpet incoming socket\n");
    }
    else // handle new socket
    {
      //printf("[STATUS] new connection received\n");
      logmsg("[STATUS] new connection received\n");
      // log all the details on the client address to the log file 
      logmsg("[INFO] logging connection details\n");
      sprintf(messages,"[INFO] incoming connection from client: %s on port: %d \n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      logmsg(messages);

      // if enabled then spin off a new process to handle 
      #ifdef MULTIPROC
      //printf("[STATUS] starting a new process to handle connection from parrent process: %d \n",getpid());
      sprintf(message,"[STATUS] starting a new process to handle connection from parrent process: %d \n",getpid()); 
      pid_t pid = fork();
      if(pid == 0) // child process
      {
        //printf("[STATUS] child process running with pid: %d \n",getpid());
        sprintf(message,"[STATUS] starting a new process to handle connection from parrent process: %d \n",getpid());
        logmsg(message);
        handle_connection(new_sockfd,&client_addr);
        exit(0); // once the request has been hand
      }
      #else
      handle_connection(new_sockfd,&client_addr);
      #endif
      //printf("[STATUS] shutting down client socket \n");
      logmsg("[STATUS] shutting down client socket \n");  
    }
  }

  //printf("[STATUS] server terminated, now clearning up...\n");
  logmsg("[STATUS] server terminated, now clearning up...\n");
  close(sockfd); // close out the server socket
}

void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr)
{
  unsigned char *ptr,request[500],resource[500],messages[80];
  int fd,length;

  memset(request,'\0',500); // make sure that the request string is all nulled out
  logmsg("\t [DEBUG] started the handle connection function\n");
  length = recv_line(sockfd,request);  // for some reason we're getting hung up here
  //printf("[STATUS] Got request from %s:%d \"%s\" \n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);
  sprintf(messages,"[STATUS] Got request from %s:%d \"%s\" \n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);
  logmsg(messages);

  // log the body of the request in debug
  // this is just to make this work real quick
  printf("%s\n","hex dump for http request body");
  for(unsigned int i = 0;i<strlen(request);i++)
  {
    printf("0x%02x:%c ",request[i],request[i]);
  }
  printf("\n");

  ptr = strstr(request," HTTP/");
  if(ptr == NULL)
  {
    close(sockfd);
    //printf("[ERROR] Not valid HTTP\n");
    logmsg("[ERROR] Not valid HTTP\n");
    
  }
  else // if there's a valid HTTP reqest
  {
    *ptr = 0;
    ptr = NULL;

    // NOTE:  each of these if statements handle a diffrent type of HTTP command
    // I'm thinking about redoing it, but I don't really have a good way of how I'd do that right now.

    // get reqest
    if(strncmp(request, "GET ", 4) == 0)
    {
      ptr = request + 4; // ptr is the URL
      //printf("[STATUS] received HTTP GET request \n");
      //printf("[INFO] request: %s, with size %d \n",ptr,strlen(ptr)); // print out the contencts of the request
      logmsg("[STATUS] received HTTP GET request \n");
      sprintf(messages,"[INFO] request: %s, with size %d \n",ptr,strlen(ptr));
      logmsg(messages);

      // respond to the get request 
      //send_string(sockfd, "HTTP/1.0 200 OK\r\n");
      //send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
      char path[200];

      if(strcmp(ptr,"/") == 0) // check if the client is trying to get the index.html file
      {
        //printf("[STATUS] index.html reqested\n");
        logmsg("[STATUS] index.html reqested\n");
        strcpy(path,"./webroot/index.html");
        //strcpy(path,WEBROOT);
        //strcat(path,"/");
        //strcat(path,INDEX);
      }
      else // if the path is something besides the index, then figure out what the path is, then end the path
      {  
        // non index paths    
        strcpy(path,WEBROOT);
        strcat(path,ptr);
      }
      
      // if there's not path found, then see if the call is for a file or an endpoint
      // see if the file exists 
      if(hasFile(path) && !isFolder(path))
      {
        // if exisits then send the file over
        send200(sockfd);
        sprintf(messages,"[INFO] sending file: \"%s\" \n",path);
        logmsg(messages); 
        if(sendFile(sockfd,path) < 0)
        {
          sprintf(messages,"[ERROR] failed to send resource: \"%s\" to client \n",path);
          logmsg(messages);
          // try to generate some more information for error tracking
          if(fileTooLarge(path))
          {
            logmsg("[ERROR] file is too large\n");
            // send http code for file being too large... BOOKMARK
            // TODO send needed http error code if the file is too large 
          }
        }
      }
      else if(hasEndPoint(ptr,GET) && !isFolder(ptr)) // see if the requested path is a endpoint, ptr has the actual body of request with out the webroot dir
      {
        char responce[MAXRESSIZE];
        sprintf(messages,"[STATUS] endpoint \" %s \" requested \n",ptr);
        runEndPoint(ptr,responce,GET);
        send200(sockfd);
        send(sockfd,responce,strlen(responce),0); // this is more of an exspriment
      }
      else if(isFolder(ptr)) // detrmine if the ptr is a folder
      {
        // real quick... make sure that we don't also have already have an existing folder.  If so than send a 500
        if(hasEndPoint(ptr,GET) || hasEndPoint(ptr,POST)) 
        {
          // if this is a folder and an endpoint than send a 500 error
          sprintf(messages,"[ERROR] requested destionation is a folder as well as an endpoint\n"); // remeber the log file...
          logmsg(messages);
          // send a responce back to the client 
          send500(sockfd);
          char *error505 = "<title>505 error</title> <h1> 500 internal error </h1>";
          send(sockfd,error505,strlen(error505),0);
        }
        else // otherwise we can send the folder deets
        {
          
          send200(sockfd); // let the client know that they send a correct an input
          // call the folder handling function
          handleFolders(path,sockfd);
        }
      }
      else // file not found, send 404.  So thing found.  
      {
        // just makeup a 404 page for now
        logmsg("[STATUS] file not found, sending 404 error \n");
        send404(sockfd);
        char *errorPage = "<title> 404 not found </title> <h1> 404 Page not found! </h1>"; // litterly all the html for the 404 page
        send(sockfd, errorPage,strlen(errorPage), 0);       
      }
    }


    // post reqest
    if(strncmp(request,"POST ",5) == 0)
    {
      ptr = request + 5;
      size_t reqsize = strlen(request);
      sprintf(messages,"[STATUS] received HTTP POST request with size %ld bytes \n",reqsize);
      logmsg(messages);
      sprintf(messages,"[STATUS] POST request body: %s \n",ptr);
      logmsg(messages);
      // see if the request of in the list of owned get requests 
      if(hasEndPoint(ptr,POST))
      {
        char responce[MAXRESSIZE];
        sprintf(messages,"[STATUS] received request %s ",ptr);
        logmsg(messages);
        // run the end point, then send a 200.  Afterwords send the send the responce from the callback
        // just like in the get request :)
        runEndPoint(ptr,responce,POST);
        send200(sockfd);
        send(sockfd,responce,strlen(responce),0);
      }
      else
      {
        send404(sockfd);
      }
    }


    // head reqest
    if(strncmp(request,"HEAD ",5) == 0)
    {
      logmsg("[STATUS] got a HTTP head reqest \n");
      // how to handle the post reqest 
      ptr = request + 5; // ptr is the URL
    }
    // print out the reqest uri
    //printf("[DEBUG] request uri is: \"%s\" \n",ptr);

    logmsg("[INFO] closing client socket \n");
    shutdown(sockfd, SHUT_RDWR); 
  }
  
   logmsg("\t [DEBUG] ending the handle connection function\n");
}

void handleFolders(char *path,int sockfd)
{
  // get the path
  char folderPath[80];
  char lineBuffer[160];
  // we're just gonna assume that the correct webroot is already in the parth
  //strcpy(folderPath,WEBROOT);
  //strcat(folderPath,path);
  // figure out how many folders we'er dealing with
  size_t folderCount = countFolders(path);
  // allocate space for the html, using the stack and heap.  FML...
  char *folderNames[folderCount];
  for(size_t i=0;i<folderCount;i++)
  {
    folderNames[i] = (char*)malloc(MAXDIRNAMESIZE);
  }
  // now we get a list of folder names and save them to our dynamic array lol
  listFolders(path,folderNames,folderCount);  
  // build of the folder list page 
  char *folderListPage = (char*)malloc(FOLDERLISTPAGEBUFF);
  // header page and html header
  sprintf(lineBuffer,"<head> <title> directory listing of %s </title> </head> <body> <h2> listing of folders for %s </h2><br>",path,path);
  strcat(folderListPage,lineBuffer);
  // iterate through all the folder names and add them to the listing page 
  char filePath[160];
  for(size_t i=0;i<folderCount;i++)
  {
    strcpy(filePath,path);
    strcat(filePath,"/");
    strcat(filePath,folderNames[i]);
    sprintf(lineBuffer,"<a href=\"%s\">%s</a><br>",filePath,folderNames[i]);
    strcat(folderListPage,lineBuffer);
  }

  strcat(folderListPage,"<button onclick=\"window.history.back()\">back</button><br>");
  strcat(folderListPage,"</body>");
  // send the page back up to the server
  send(sockfd,folderListPage,strlen(folderListPage),0);
  // ok now make sure to clean up so we don't leave a mess in the heap
  free(folderListPage);
  for(size_t i=0;i<folderCount;i++) // clean up the array names 
  {
    free(folderNames[i]);
  }

}

void send404(int sockfd)
{
  send_string(sockfd, "HTTP/1.0 404 OK\r\n");
  send_string(sockfd, "Server: crappy webserver\r\n\r\n");
}

void send200(int sockfd)
{
  send_string(sockfd, "HTTP/1.0 200 OK\r\n");
  send_string(sockfd, "Server: crappy webserver\r\n\r\n");
}

void send500(int sockfd)
{
  send_string(sockfd, "HTTP/1.0 500 OK\r\n");
  send_string(sockfd, "Server: crappy webserver\r\n\r\n");
}

int cutStr(char *str)
{
  int countedBytes = 0;
  size_t len = strlen(str);
  size_t i;
  for(i = 0;i < len;i++)
  {

  }

  return countedBytes;
}

void stopServer()
{
  logmsg("[INFO] Stopping server...\n");
  running = false;
}

