all: ur

ur: main.o rectangle.o house.o city.o being.o field.o beingai.o
	g++ -O -o ur main.o rectangle.o house.o city.o being.o beingai.o field.o -std=c++11 -lpanel -lncurses

debug: maindbg.o rectangledbg.o housedbg.o citydbg.o beingdbg.o fielddbg.o beingaidbg.o
	g++ -g -o ur maindbg.o rectangledbg.o housedbg.o citydbg.o beingdbg.o beingaidbg.o fielddbg.o -std=c++11 -lpanel -lncurses

main.o:
	g++ -O -c main.cpp -std=c++11

maindbg.o:
	g++ -c -g -o maindbg.o main.cpp -std=c++11

rectangle.o:
	g++ -O -c rectangle.cpp -std=c++11

rectangledbg.o:
	g++ -c -g -o rectangledbg.o rectangle.cpp -std=c++11

house.o:
	g++ -O -c house.cpp -std=c++11

housedbg.o:
	g++ -c -g -o housedbg.o house.cpp -std=c++11

city.o:
	g++ -O -c city.cpp -std=c++11

citydbg.o:
	g++ -c -g -o citydbg.o city.cpp -std=c++11

being.o:
	g++ -O -c being.cpp -std=c++11

beingdbg.o:
	g++ -c -g -o beingdbg.o being.cpp -std=c++11

beingai.o:
	g++ -O -c beingai.cpp -std=c++11

beingaidbg.o:
	g++ -c -g -o beingaidbg.o beingai.cpp -std=c++11

field.o:
	g++ -O -c field.cpp -std=c++11

fielddbg.o:
	g++ -c -g -o fielddbg.o field.cpp -std=c++11

clean:
	rm *.o ur
