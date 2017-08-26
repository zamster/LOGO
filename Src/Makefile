# Compiler settings
CC     = g++
CFLAGS =  -DDLL -Wall -O3 -ffast-math
#Target
BIN  = main.exe
OBJ  = main.o Fx2D.dll
# Linker settings
LIBS = -lglfw -lopengl32 -lglu32 -lirrKlang -s

# Subsystem settings
WINDOWS = -mwindows
CONSOLE = -mconsole

# Default: Build all tests
all: main.exe

# main
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o "main.exe" $(LIBS) $(CFLAGS)

main.o: main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)

Fx2D.dll: stdbi.o Fx2D.o
	$(CC) -shared -o "Fx2D.DLL" stdbi.o Fx2D.o $(LIBS) -Wl,--out-implib,Fx2D.a

stdbi.o: stdbi.cpp
	$(CC) -c stdbi.cpp -o stdbi.o $(CFLAGS)

Fx2D.o: Fx2D.cpp
	$(CC) -c Fx2D.cpp -o Fx2D.o $(CFLAGS)
