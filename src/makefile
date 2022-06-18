CC=gcc
CFLAGS =-lm -Wall -g -ggdb
TARGET = main
OBJs=main.o a.o
$(TARGET):$(OBJs)
	$(CC) $(OBJs) $(CFLAGS) -o main	
	rm $(OBJs)
$(OBJs):
	$(CC) main.c -c -o main.o $(CFLAGS)
	$(CC) a.c -c -o a.o $(CFLAGS)
clean:
	rm $(TARGET) 	
