main: main.cpp tree.o dl.o
	g++ -o main main.cpp tree.o dl.o  -std=c++11
	rm -f *.o
tree.o: tree.cpp
	g++ -c tree.cpp -std=c++11
dl.o: dl.cpp
	g++ -c dl.cpp -std=c++11
# sa.o: sa.cpp
# 	g++ -c sa.cpp
clean:
	rm main
