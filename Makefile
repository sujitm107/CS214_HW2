all: all
	gcc -Wall -fsanitize=address tree.c -o tree
	gcc -Wall -fsanitize=address find.c -o find
	gcc -Wall -fsanitize=address ls.c -o ls


clean:  
	rm tree find ls
