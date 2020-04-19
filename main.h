#ifndef MAIN_H
#define MAIN_H

// local includes
#include "server.h"
#include "severconfig.h"

// prototypes...
void fatal(char *message);
void *ec_malloc(unsigned int size);
void dump(const unsigned char *data_buffer, const unsigned int length);

#endif
