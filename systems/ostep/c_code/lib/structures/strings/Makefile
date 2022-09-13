TARGET = strings.test
CC = gcc

SRC = strings.c strings.test.c
OBJ = $(patsubst %.c, %.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) -Wall -Werror $(OBJ) -o $(TARGET)

%.o : %.c
	$(CC) -c -Wall -Werror $< -o $@

clean :
	rm $(TARGET) $(OBJ)

test : clean $(TARGET)
	./$(TARGET)
