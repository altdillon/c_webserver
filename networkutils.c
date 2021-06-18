#include "networkutil.h"

void hex_dump(unsigned char *buffer,unsigned int length,unsigned char *strbuffer)
{
    memset(strbuffer,"\0",length); // set strbuffer to 0 to all values up to length
    unsigned char byte;
    for(unsigned int i = 0;i<length;i++)
    {
        byte = buffer[i];
        printf("%02x ",buffer[i]); // show a byte
        if(((i%16)==15) || (i==length-1))
        {
            
        }
    }   
}
