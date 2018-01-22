LIBS: -lSOIL -lglfw2 -lGLEW -lX11 -lXinerama -lXi -lXxf86vm -lXcursor -ldl -lGL -lm -lpthread 

program: program.o game.o resource_manager.o texture.o shader.o 
program.o: program.cpp game.h resource_manager.h
	g++ -c program.cpp 
game.o:	game.cpp game.h resource_manager.h sprite_renderer.h
	g++ -c game.cpp
resourcer_manager.o: resource_manager.cpp resource_manager.h
	g++ -c resource_manager.cpp
texture.o: texture.cpp texture.h 
	g++ -c texture.cpp 
shader.o: shader.cpp shader.h
	g++ -c shader.cpp 
sprite_renderer.o: sprite_renderer.cpp sprite_renderer.h 
	g++ -c sprite_renderer.cpp

clean: 
	rm program program.o game.o resource_manager.o texture.o \ 
	shader.o sprite_renderer.o 


		
