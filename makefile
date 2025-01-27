

main.o : main.cpp
	g++ -std=c++20 -c main.cpp game.cpp -I/root/in204/projet/IN204/SFML-3.0.0/include 

main : main.o
	export LD_LIBRARY_PATH=/root/in204/projet/IN204/SFML-3.0.0/lib
	g++  main.o -o main -L/root/in204/projet/IN204/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

run: main

	LD_LIBRARY_PATH=/root/in204/projet/IN204/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./main  # Exécution avec le bon chemin


makeclear:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm main.o main