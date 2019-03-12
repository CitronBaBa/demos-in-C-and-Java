CFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi
CFWIN = -O2 -Wall -Wextra -pedantic -Wfloat-equal -ansi
TARGET = sdl
TARGET2 = sdlwin
WIN = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -w -Wl,-subsystem,windows
SOURCES =  sdl.c main.c parser.c parser.h
LIBS =  `sdl2-config --libs` -lm
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

$(TARGET2): $(SOURCES)
		$(CC) $(SOURCES) $(WIN) -o $(TARGET2) $(CFWIN) -lm

clean:
	rm -f $(TARGET)
	del -f $(TARGET2)

run: all
	./$(TARGET)
