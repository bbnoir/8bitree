main: main.cpp tree.o dl.o
	g++ -o main main.cpp tree.o dl.o  -std=c++11
	rm *.o
tree.o: tree.cpp
	g++ -c tree.cpp -std=c++11
dl.o: dl.cpp
	g++ -c dl.cpp -std=c++11
sa.o: sa.cpp
	g++ -c sa.cpp
test: test.cpp encoder.o
	g++ -o test test.cpp encoder.o -std=c++11
	rm *.o
encoder.o: encoder.cpp
	g++ -c encoder.cpp -std=c++11
clean:
	rm main
	rm test
	rm *.o
