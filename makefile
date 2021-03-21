
CC=gcc


# generate object files
#main.o : main.c main.h webpage.h endpoint.h severconfig.h
#	$(CC) -c main.c
#
#server.o : server.c server.h debug.h httpcodes.h severconfig.h endpoint.h datalogging.h
#	$(CC) -c server.c
#
#datalogging.o : datalogging.c dataloggin.h severconfig.h
#	$(CC) -c datalogging.c
#
#

# main
main.o : server.h severconfig.h main.h webpage.h endpoint.h
	$(CC) -c main.c

# server header
server.o : severconfig.h endpoint.h datalogging.h server.h debug.h severconfig.h endpoint.h datalogging.h
	$(CC) -c server.c

# datalogging
datalogging.o : severconfig.h datalogging.h
	$(CC) -c datalogging.c

# endpoint
endpoint.o : httpcodes.h endpoint.h endpoint.c
	$(CC) -c endpoint.c
