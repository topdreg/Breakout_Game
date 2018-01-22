#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector> 

#include <GL/glew.h> 
#include <glm/glm.hpp> 

#include "game_object.h" 
#include "ball_object.h" 
#include "sprite_renderer.h" 
#include "resource_manager.h" 

// GameLevel holds all invaders as part of the game and hosts functionality to load/render levels from harddisk

class GameLevel 
{ 
public: 
	//Level state
	std::vector<GameObject> Bricks; 
	//Constructor
	GameLevel() { } 
	//Loads level from file
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight); 
	//Render level
	void Draw(SpriteRenderer &renderer); 
	//Check if level is completed (all invaders are destroyed) 
	GLboolean IsCompleted(); 
private: 
	//Initialize level from invader data 
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
}; 

#endif
