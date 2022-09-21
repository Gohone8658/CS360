wordpairs: main.o getwordpairs.o hashtable.o getWord.o getWord.h crc64.o crc64.h functions.h
	gcc -g -o wordpairs main.o getwordpairs.o hashtable.o getword.o crc64.o
main.o: main.c functions.h
	gcc -g -c main.c
getwordpairs.o: getwordpairs.c functions.h	
	gcc -g -c getwordpairs.c
hashtable.o: hashtable.c functions.h
	gcc -g -c hashtable.c
getWord.o: getWord.c getWord.h
	gcc -g -c getword.c
crc64.o: crc64.c crc64.h
	gcc -g -c crc64.c
clean:
	echo "cleaning stuff up boss..."; rm wordpairs main.o getwordpairs.o hashtable.o getWord.o crc64.o