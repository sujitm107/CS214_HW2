CFLAGS = -Wall -g -fsanitize=address

all: all
	gcc $(CFLAGS) tree.c -o tree
	gcc $(CFLAGS) find.c -o find
	gcc $(CFLAGS) ls.c -o ls

%: %.c
	gcc -o $@ $^

clean:  
	rm tree find ls
