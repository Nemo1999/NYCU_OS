.PHONY:clean test all images compile plot
compile:process thread single_thread
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
	rm -f *.png
test:single_thread process thread
	./single_thread 10000000 6
	./process       10000000 6 4
	./thread        10000000 6 16
images: thread.png process.png

thread.png: plot_thread.py compile
	python3 plot_thread.py
process.png: plot_process.py compile
	python3 plot_process.py

plot: thread.png process.png
all: plot
run: plot
