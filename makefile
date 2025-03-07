

main.o : main.cpp
	g++ -std=c++20 -c main.cpp game.cpp -I/root/in204/projet/SFML-3.0.0/include 

main : main.o
	export LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib
	g++  main.o game.o -o main -L/root/in204/projet/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

run: main

	LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./main  # Exécution avec le bon chemin


makeclear:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm main.o main game.o




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




pause.o : pause.cpp
	g++ -std=c++20 -c pause.cpp usefull.cpp -I/root/in204/projet/SFML-3.0.0/include 

pause : pause.o
	export LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib
	g++  pause.o -o pause -L/root/in204/projet/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network


runPause: pause

	LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./pause  # Exécution avec le bon chemin


makeclearPause:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm pause.o pause usefull.o 



settings.o : settings.cpp
	g++ -std=c++20 -c settings.cpp usefull.cpp -I/root/in204/projet/SFML-3.0.0/include 

settings : settings.o
	export LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib
	g++  settings.o -o settings -L/root/in204/projet/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network


runSettings: settings

	LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./settings  # Exécution avec le bon chemin


makeclearSettings:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm settings.o settings usefull.o 


welcome.o : welcome.cpp
	g++ -std=c++20 -c welcome.cpp usefull.cpp -I/root/in204/projet/SFML-3.0.0/include 

welcome : welcome.o
	export LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib
	g++  welcome.o -o welcome -L/root/in204/projet/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network


runWelcome: welcome

	LD_LIBRARY_PATH=/root/in204/projet/SFML-3.0.0/lib:$LD_LIBRARY_PATH ./welcome  # Exécution avec le bon chemin


makeclearWelcome:
	export LIBGL_ALWAYS_SOFTWARE=1
	rm welcome.o welcome usefull.o 