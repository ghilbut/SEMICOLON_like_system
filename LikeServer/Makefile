.SUFFIXES : .cpp .o

CC = g++

INC = -I../codebase -I/usr/include
LIBS = ../lib/codebase.lib -L/usr/lib -lboost_system -lboost_filesystem -ljson
CFLAGS = -g $(INC)

OBJS = main.o

TARGET = server

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<


