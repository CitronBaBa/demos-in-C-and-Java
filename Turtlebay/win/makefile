CFLAGS = -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi

TARGET = happy_turtle
OT = -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2
SOURCES =  sdl.c main.c parser.c parser.h
LIBS =  -lm
CC = gcc


all: $(TARGET)

$(TARGET):
	$(CC) $(SOURCES) $(OT) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
