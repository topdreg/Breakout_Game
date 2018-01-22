#ifndef BALLOBJECT_H 
#define BALLOBJECT_H

#include <GL/glew.h> 
#include <glm/glm.hpp>

#include "texture.h" 
#include "sprite_renderer.h" 
#include "game_object.h" 

class BallObject : public GameObject 
{
public: 
	//ball state
	GLfloat	Radius;
	GLfloat	Stuck; 
	//constructors
	BallObject(); 
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite); 
	//moves the ball, keeping it constrained within the window bounds 
	glm::vec2 Move(GLfloat dt, GLuint window_width); 
	//resets ball to original state 
	void	Reset(glm::vec2 position, glm::vec2 velocity);
}; 

#endif
