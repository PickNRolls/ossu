strings.test : strings.test.o strings.o
	gcc -Wall -Werror strings.test.o strings.o -o strings.test

strings.test.o : strings.test.c
	gcc -c -Wall -Werror strings.test.c -o strings.test.o

strings.o : strings.c
	gcc -c -Wall -Werror strings.c -o strings.o
