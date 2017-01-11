CC = g++

CFLAGS = -std=c++11 -Wall -g -Iinclude

LDFLAGS = -L/usr/local/lib -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -Llibs -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

TARGET = part

SRC = src/main.cc \
		src/tools.cc \
		src/shader_handler.cc \
		src/window_handler.cc \
		src/particle_handler.cc \
		libs/glad.c 

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)


