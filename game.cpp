#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_level.h"

//Game-related data
SpriteRenderer  *Renderer;
GameObject	*Player; 
BallObject 	*Ball;

Game::Game(GLuint width, GLuint height) 
		: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
	delete Renderer;
	delete Player; 
}

void Game::Init()
{
	//Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite"); 
	//Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f); 
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0); 
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection); 
	//set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite")); 
	//Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background"); 
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face"); 
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block"); 
	ResourceManager::LoadTexture("textures/solid_block.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");	
	//Load levels
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5); 
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5); 
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5); 
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two); 
	this->Levels.push_back(three); 
	this->Levels.push_back(four); 
        this->Level = 0; 
	//paddle information 
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2, 
		this->Height - PLAYER_SIZE.y 
	); 
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle")); 	
	//ball information
	//initial velocity of ball
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2); 
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face")); 
}

void Game::Update(GLfloat dt)
{
	//update objects
	Ball->Move(dt, this->Width); 
	//check for collisions
	this->DoCollisions();
        //check if ball reached bottom edge
	if (Ball->Position.y >= this->Height) 
	{
		this->ResetLevel(); 
		this->ResetPlayer(); 
	}	
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt; 
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0) 
				Player->Position.x -= velocity; 
			if (Ball->Stuck) 
				Ball->Position.x -= velocity; 
		}
		if (this->Keys[GLFW_KEY_D]) 
		{
			if (Player->Position.x <= this->Width - Player->Size.x) 
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity; 
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false; 
	}

}

void Game::Render()
{
	if(this->State == GAME_ACTIVE)
	{
		//Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0,0), glm::vec2(this->Width, this->Height), 0.0f);
		//Draw level 
		this->Levels[this->Level].Draw(*Renderer);

		//Draw paddle
		Player->Draw(*Renderer); 

		//Draw ball
		Ball->Draw(*Renderer); 		
	}
}

void Game::ResetLevel() 
{
	if (this->Level == 0) this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f); 
	else if (this->Level == 1) 
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f); 
	else if (this->Level == 2) 
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f); 
	else if (this->Level == 3) 
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f); 
}

void Game::ResetPlayer() 
{
	//Reset player/ball stats
	Player->Size = PLAYER_SIZE; 
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y); 
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY); 
}



Collision CheckCollision(BallObject &one, GameObject &two); // AABB - AABB collision

Direction VectorDirection(glm::vec2 target); 

void Game::DoCollisions()
{
	for (GameObject &box : this->Levels[this->Level].Bricks) 
	{
		if (!box.Destroyed) 
		{
			Collision collision = CheckCollision(*Ball, box); 
			if (std::get<0>(collision)) //if collision is true
			{
				//destroy block if not solid
				if (!box.IsSolid) 
					box.Destroyed = GL_TRUE; 
				//collision resolution
				Direction dir = std::get<1>(collision); 
				glm::vec2 diff_vector = std::get<2>(collision); 
				if (dir == LEFT || dir == RIGHT) //Horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; //Reverse horizontal velocity 
					//Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x); 
					if (dir == LEFT) 
						Ball->Position.x += penetration; //Move ball to right
					else 
						Ball->Position.x -= penetration; //Move ball to left
				}
				else 
				{
					Ball->Velocity.y = -Ball->Velocity.y; //reverse vertical velocity
					//Relocate 
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y); 
					if (dir == UP) 
						Ball->Position.y -= penetration; //Move ball back up 
					else
						Ball->Position.y += penetration; //Move ball back down
				}
			}
		}
	}
	Collision result = CheckCollision(*Ball, *Player); 
	if (!Ball->Stuck && std::get<0>(result)) 
	{
		//Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2; 
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard; 
		GLfloat percentage = distance / (Player->Size.x / 2); 
		//then move accordingly 
		GLfloat strength = 2.0f; 
		glm::vec2 oldVelocity = Ball->Velocity; 
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y); 
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); 
	}		
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - AABB collision
{
	//Get center point circle first
	glm::vec2 center(one.Position + one.Radius);
	//Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2); 
	glm::vec2 aabb_center( 
		two.Position.x + aabb_half_extents.x, 
		two.Position.y + aabb_half_extents.y
	); 
	//Get difference vector between both centers
	glm::vec2 difference = center - aabb_center; 
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents); 
	//Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped; 
	//Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center; 

	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference); 
	else 
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0,0)); 
}

Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	//up
		glm::vec2(1.0f, 0.0f),	//right
		glm::vec2(0.0f, -1.0f),	//down
		glm::vec2(-1.0f, 0.0f)	//left
	};
	GLfloat max = 0.0f; 
	GLuint best_match = -1; 
	for (GLuint i = 0; i < 4; i++) 
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]); 
		if (dot_product > max) 
		{
			max = dot_product; 
			best_match = i; 
		}
	}
	return (Direction)best_match; 
}


