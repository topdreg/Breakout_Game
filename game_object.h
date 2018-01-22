#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H 

#include <GL/glew.h> 
#include <glm/glm.hpp>

#include "texture.h" 
#include "sprite_renderer.h" 

class GameObject 
{
public: 
	//object state
	glm::vec2	Position, Size, Velocity;
	glm::vec3 	Color; 
	GLfloat		Rotation;
        GLboolean	IsSolid; 	
	GLboolean	Destroyed; 
	//Render state
	Texture2D	Sprite; 
	//Constructors
	GameObject(); 
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f)); 
	//Draw sprite
	virtual void Draw(SpriteRenderer &renderer); 
}; 

#endif
