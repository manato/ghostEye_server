TARGET = GhostEye_server
CC = gcc

INC_PATH	= -I/usr/local/include/opencv

LIB_PATH	= -L/usr/local/lib

LIBS	= -lpthread -lopencv_highgui -lopencv_core


$(TARGET) : GhostEye_server.o watchman.o
	$(CC) $(INC_PATH) $(LIB_PATH) -o $@ GhostEye_server.o $(LIBS)

# server.o : GhostEye_server.c
# 	$(CC) $(INC_PATH) $(LIB_PATH) -c $? $(LIBS)
GhostEye_server.o : GhostEye_server.c
	$(CC) $(INC_PATH) $(LIB_PATH) -c $? $(LIBS)


watchman.o : watchman.c
	$(CC) -o watchman $?


clean:;	rm -f GhostEye_server.o GhostEye_server watchman ./*~

install:
	cp ./watchman /usr/local/geye/bin/image_receiver/task_observer
	cp ./GhostEye_server /usr/local/geye/bin/image_receiver/GhostEye_server
