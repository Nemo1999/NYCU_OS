.PHONY:clean test all
all:process thread single_thread
process:process.c
	gcc -o3 process.c -o process
thread:thread.c
	gcc -o3 -pthread thread.c -o thread
single_thread:single_thread.c
	gcc -o3 single_thread.c -o single_thread
clean:
	rm -f thread
	rm -f process
	rm -f single_thread
test:single_thread process thread
	./single_thread 10000000 6
	./process       10000000 6 4
	./thread        10000000 6 16
