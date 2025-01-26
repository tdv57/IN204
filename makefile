

v3.o : version/v3.cpp
	g++ -std=c++20 -c version/v3.cpp -I/root/in204/projet/IN204/SFML-3.0.0/include 

main : v3.o
	export LD_LIBRARY_PATH=/root/in204/projet/IN204/SFML-3.0.0/lib
	g++  v3.o -o main -L/root/in204/projet/IN204/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

run: main

	LD_LIBRARY_PATH=/root/in204/projet/IN204/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./main  # Exécution avec le bon chemin


makeclear:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm v3.o main 




test.o : test.cpp
	g++ -std=c++20 -c test.cpp -I/root/in204/projet/SFML-3.0.0/include 

test : test.o
	export LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib
	g++  test.o -o test -L/root/in204/projet/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

run2: test
	LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./test  # Exécution avec le bon chemin


makeclear2:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm test.o test
