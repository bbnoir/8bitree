main: main.cpp tree.o dl.o
	g++ -o main main.cpp tree.o dl.o  -std=c++11
	rm *.o
tree.o: tree.cpp
	g++ -c tree.cpp -std=c++11
dl.o: dl.cpp
	g++ -c dl.cpp -std=c++11
sa.o: sa.cpp
	g++ -c sa.cpp
test: test.cpp Encoder.o Decoder.o CodeTree.o
	g++ -o test test.cpp Encoder.o Decoder.o CodeTree.o -std=c++11
	rm *.o
Encoder.o: Encoder.cpp
	g++ -c Encoder.cpp -std=c++11
Decoder.o: Decoder.cpp
	g++ -c Decoder.cpp -std=c++11
CodeTree.o: CodeTree.cpp
	g++ -c CodeTree.cpp -std=c++11
clean:
	rm main
	rm test
	rm *.o
