clean:
	rm -rf task

task: task.cpp
	g++ task.cpp -o task -g

debug: task
	gdb task

bf: bf.cpp
	g++ bf.cpp -g -o bf

debugbf: bf
	gdb bf

test: task bf
	./test.sh
