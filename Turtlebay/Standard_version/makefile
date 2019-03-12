CFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi
CFLAGS2 = -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi -static-libgcc -lm

TARGET = std
TARGET2 = std_win
TARGET3 = std_test
TARGET4 = std_parser

SOURCES =  sdl.c main.c interpreter.c
testsources = sdl.c test.c interpreter.c
parserSOURCES =  parsermain.c parser.c
LIBS =  `sdl2-config --libs` -lm
CC = gcc
OT = -w -lmingw32 -lSDL2main -lSDL2


all: $(TARGET) $(TARGET3) $(TARGET4)

$(TARGET): $(SOURCES) interpreter.h
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS) -include interpreter.h
$(TARGET2): $(SOURCES) interpreter.h
		$(CC) $(SOURCES) $(OT) -o $(TARGET2) $(CFLAGS2) -lm -include interpreter.h
$(TARGET3): $(testsources) test.h
		$(CC) $(testsources) -o $(TARGET3) $(CFLAGS) $(LIBS) -include test.h
$(TARGET4): $(parserSOURCES) parser.h
		$(CC) $(parserSOURCES) -o $(TARGET4) $(CFLAGS) $(LIBS) -include parser.h
