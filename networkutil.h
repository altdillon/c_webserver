#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// simple hext dump function; basicly lifted from hacking the art of...
// get a hex dump of the data bassed into buffer and save it to str buffer
void hex_dump(unsigned char *buffer,unsigned int length,unsigned char *strbuffer);



#endif
