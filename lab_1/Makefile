TARGET_1 = main
TARGET_2 = child_1
TARGET_3 = child_2
EXT = c
CC = gcc
FUNCT = function
CFLAGS = -std=c11 -pedantic -Wall 

$(TARGET_1) : $(TARGET_2) $(TARGET_3) $(FUNCT).$(EXT) $(TARGET_1).$(EXT)  $(TARGET_3).$(EXT)
	$(CC) $(CFLAGS) $(TARGET_1).$(EXT) $(FUNCT).$(EXT) -o $(TARGET_1)

$(TARGET_2) : $(FUNCT).$(EXT) $(TARGET_2).$(EXT)
	$(CC) $(CFLAGS) $(TARGET_2).$(EXT) $(FUNCT).$(EXT) -o $(TARGET_2)

$(TARGET_3) : $(FUNCT).$(EXT) $(TARGET_3).$(EXT)
	$(CC) $(CFLAGS) $(TARGET_3).$(EXT) $(FUNCT).$(EXT) -o $(TARGET_3)

clean :
	rm $(TARGET_1) $(TARGET_2) $(TARGET_3) *.o