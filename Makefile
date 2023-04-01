# regula implicita
all: build

# regula de compilare si generare executabil
build:
	gcc -Wall banda.c -o tema1

# regula pentru rularea executabilului
run:
	./tema1

# regula pentru stergerea executabilelor
clean:
	rm -rf tema1