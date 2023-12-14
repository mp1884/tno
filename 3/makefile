clean:
	rm -rf task tests bm.txt

gentests:
	rm -rf tests
	mkdir -p tests
	python gentests.py < tests.txt

task: task.cpp
	g++ task.cpp -g -lm -o task

test: task
	./test.sh

debug: task
	gdb task

