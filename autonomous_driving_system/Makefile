##########
#Makefile#
##########

# Target name
TARGET	= autod
LOAD_IMG	= load_img
DISPLAY	= display_shm
WHILE1	= while1_display_shm
DR_RECT	= draw_rects
SHM_DELETE	= shm_deleter

# Source files
SRC	= $(TARGET).c

# Compile option
OPT	= -g

# Compiler flags
CC	= gcc $(OPT)

# Additional libraries
OPENCV_LIBS	= -lopencv_highgui -lopencv_core
LIBS	= -lrt $(OPENCV_LIBS) -lstdc++

# Include paths
INC_PATH	= /usr/local/include/opencv/

# Library paths
LIB_PATH	= /usr/local/lib/

all:	load_img.o display_shm.o while1_display_shm.o draw_rects.o shm_delete.o
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(TARGET) $(SRC) $(LIBS) -lpthread

load_img.o:
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(LOAD_IMG) $(LOAD_IMG).cpp $(LIBS)

display_shm.o:
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(DISPLAY) $(DISPLAY).cpp $(LIBS)

while1_display_shm.o:
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(WHILE1) $(WHILE1).cpp $(LIBS)

draw_rects.o:
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(DR_RECT) $(DR_RECT).cpp $(LIBS)

shm_delete.o:
	$(CC) -I$(INC_PATH) -L$(LIB_PATH) -o $(SHM_DELETE) $(SHM_DELETE).c $(LIBS) -lpthread



clean:	clb
	rm -f $(TARGET) $(LOAD_IMG) $(DISPLAY) $(WHILE1) $(DR_RECT)

clb:	
	rm -f ./*~

install:
	cp ./org_image/lena.jpg /usr/local/geye/bin/shm_creater/lena.jpg
	cp ./autod /usr/local/geye/bin/shm_creater/shm_creater
	cp ./shm_deleter /usr/local/geye/bin/shm_deleter/shm_deleter
