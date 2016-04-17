all: ur

ur: main.o rectangle.o house.o city.o
	g++ -o ur main.o rectangle.o house.o city.o -std=c++11 -lncurses

main.o:
	g++ -c main.cpp -std=c++11

rectangle.o:
	g++ -c rectangle.cpp -std=c++11

house.o:
	g++ -c house.cpp -std=c++11

city.o:
	g++ -c city.cpp -std=c++11

clean:
	rm *.o ur
