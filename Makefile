Simulator: Simulator.o Event.o
	g++ Simulator.o Event.o -o Simulator

Simulator.o: Simulator.cpp Simulator.h
	g++ -c Simulator.cpp

Event.o: Event.cpp Event.h
	g++ -c Event.cpp

clean:
	rm *.o Simulator
