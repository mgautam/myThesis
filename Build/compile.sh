#!/bin/sh

rm *.o

REL=..
DIRS=$REL/*
for DIR in $DIRS
do
    if [ -d "$DIR" ]
    then
	#rm $DIR/*.o
	FILES=$DIR/*.cpp
	for FILE in $FILES
	do
	    if [ -e "$FILE" ]
	    then
		echo "Compiling "$FILE"";
		ccache g++ -c -g $FILE -I $REL -o $FILE".o"
	        mv $FILE".o" .
	    fi
	done
    fi
done

ccache g++ -g *.o ../main.cpp -I $REL -o ../myThesis
rm *.o