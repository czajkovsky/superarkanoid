CC=g++
LD=g++

CFLAGS=-Wall -lSDL -lSDL_image
LDFLAGS=-Wall -lSDL -lSDL_image

SRCS=geo.cpp Border.cpp Platform.cpp Ball.cpp Block.cpp Bonus.cpp Player.cpp Level.cpp Screen.cpp main.cpp
HEADERS=geo.h Border.h Platform.h Ball.h Block.h Bonus.h Player.h Level.h Screen.h main.h 
TARGET=prog

OBJS=$(addprefix ./obj/, $(addsuffix .o, $(SRCS)))
HDRS=$(addprefix ./src/, $(HEADERS))

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
./obj/%.cpp.o: ./src/%.cpp $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)
