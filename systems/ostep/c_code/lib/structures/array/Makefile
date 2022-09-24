TARGET = array.test
CC = gcc

SRC = array.c array.test.c
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) -Wall -Werror $(OBJ) -o $(TARGET)

%.o : %.c
	$(CC) -c -Wall -Werror $< -o $@

clean :
	rm -f $(TARGET) $(OBJ)

test : clean $(TARGET)
	./$(TARGET)
