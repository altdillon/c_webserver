# basic build script until we need a make file
echo "compiling"
rm a.out
rm *.gch
gcc -Wall -g *.c *.h
