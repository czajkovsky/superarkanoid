CC=g++
LD=g++

CFLAGS=-Wall -lSDL -lSDL_image
LDFLAGS=-Wall -lSDL -lSDL_image

SRCS=geo.cpp Border.cpp Platform.cpp Ball.cpp Block.cpp Bonus.cpp Player.cpp Level.cpp main.cpp 
TARGET=prog

OBJS=$(addprefix ./obj/, $(addsuffix .o, $(SRCS)))

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
./obj/%.cpp.o: ./src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)
