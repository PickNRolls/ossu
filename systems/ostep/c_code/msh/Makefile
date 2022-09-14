NAME = msh
TARGET = $(NAME)
DEBUG_TARGET = $(NAME).debug
CC = gcc
CARGS = -Wall -Werror
CARGS_DEBUG = -g

SRC = $(NAME).c
LIB_SRC = ../lib/structures/strings/strings.c

OBJ = $(patsubst %.c, %.o, $(SRC))
LIB_OBJ = $(patsubst %.c, %.o, $(LIB_SRC))

DEBUG_OBJ = $(patsubst %.c, %.debug.o, $(SRC))
DEBUG_LIB_OBJ = $(patsubst %.c, %.debug.o, $(LIB_SRC))

$(TARGET) : $(OBJ) $(LIB_OBJ)
	$(CC) $(CARGS) $(OBJ) $(LIB_OBJ) -o $(TARGET)

$(DEBUG_TARGET) : $(DEBUG_OBJ) $(DEBUG_LIB_OBJ)
	$(CC) $(CARGS) $(DEBUG_OBJ) $(DEBUG_LIB_OBJ) -o $(DEBUG_TARGET)

%.o : %.c
	$(CC) -c $(CARGS) $< -o $@

%.debug.o : %.c
	$(CC) -c $(CARGS) $(CARGS_DEBUG) $< -o $@

clean :
	rm -f $(TARGET) $(DEBUG_TARGET) $(OBJ) $(LIB_OBJ) $(DEBUG_OBJ) $(DEBUG_LIB_OBJ)

debug : $(DEBUG_TARGET)
	gdb $(DEBUG_TARGET)
