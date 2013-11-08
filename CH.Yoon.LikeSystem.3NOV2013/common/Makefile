
INC = -I/usr/include
CFLAGS = -g -c

../lib/common.lib : like_client.o like_count.o like_result.o like_room.o like_server.o like_session.o
	ar rcs $@ $^

.cpp.o :
	g++ $(INC) $(CFLAGS) $<

