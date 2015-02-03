# Object files
OBJS = mp3.o camera.o model.o face.o vertex.o vector.o

# EXE name
EXENAME = mp3

# Macro assignments
CC = g++
CCOPTS = -c -g -O0 -Wall 
LINK = g++
LINKOPTS = -o

all : $(EXENAME)

# The main compiling step
$(EXENAME) : $(OBJS)
	$(LINK) $(LINKOPTS) $(EXENAME) $(OBJS) -lglut -lGLU -lGL

# Compiles object files.
mp3.o : mp3.cpp 
	$(CC) $(CCOPTS) mp3.cpp

camera.o : camera.cpp 
	$(CC) $(CCOPTS) camera.cpp

model.o : model.cpp 
	$(CC) $(CCOPTS) model.cpp

face.o : face.cpp 
	$(CC) $(CCOPTS) face.cpp

vertex.o : vertex.cpp 
	$(CC) $(CCOPTS) vertex.cpp

vector.o : vector.cpp
	$(CC) $(CCOPTS) vector.cpp

# Clean
clean : 
	rm -f *.o $(EXENAME) 2>/dev/null
