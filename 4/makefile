clean:
	rm -rf task

task: task.cpp
	g++ task.cpp -o task -g

debug: task
	gdb task

bf: bf.cpp
	g++ bf.cpp -g -o bf

bf2: bf2.c
	gcc bf2.c -g -o bf2

debugbf: bf
	gdb bf

test: task bf bf2
	./test.sh
