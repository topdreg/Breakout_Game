This game, based on Breakout, was adapted from the Breakout game example posted on learnopengl.com (a very helpful tutorial website on OpenGL). 

To compile the source code in the game, please run g++ -g -std=c++14 program.cpp game.cpp resource_manager.cpp glad.c texture.cpp shader.cpp sprite_renderer.cpp game_level.cpp game_object.cpp ball_object.cpp -lSOIL -lglfw3 -lGLEW -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -ldl -lGL -lm -lpthread -o game

(I'm currently learning how to create makefiles. Sorry!)  
