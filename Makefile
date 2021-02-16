all:
	gcc -D INFO -D DEBUG -D TIME -Wall -g -o main *.c 
clean:
	gcc -D TIME -g -o main *.c 