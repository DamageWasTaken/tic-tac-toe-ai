all:
	g++ main.cpp src/globals.cpp -g -o main -I src/include/
run:
	g++ main.cpp src/globals.cpp -g -o main -I src/include/
	./main
clean:
	rm -f main
cleanrun:
	rm -f main
	g++ main.cpp src/globals.cpp -g -o main -I src/include/
	./main