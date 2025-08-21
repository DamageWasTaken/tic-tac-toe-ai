all:
	g++ main.cpp src/globals.cpp -o main -g -I src/include/
run:
	g++ main.cpp src/globals.cpp -o main -g -I src/include/
	./main
clean:
	rm -f main
	g++ main.cpp src/globals.cpp -o main -g -I src/include/
cleanrun:
	rm -f main
	g++ main.cpp src/globals.cpp -o main -g -I src/include/
	./main