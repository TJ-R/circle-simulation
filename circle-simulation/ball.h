#include <SFML/Graphics.hpp>

#ifndef BALL_H
#define BALL_H

class Ball : public sf::CircleShape 
{
private:
	sf::Vector2f movementVector{};
	sf::RectangleShape AABB_Collider{};
	int id{};

public:
	Ball();
	Ball(int, float); // Inherit from CircleShape
	void setId(int);
	int getId();
	void createRandomBall(float, float);
	void setMovementVector(sf::Vector2f);
	sf::Vector2f getMovementVector();
	sf::RectangleShape& getAABBCollider();
	void setAABBCollider(sf::Vector2f size);
	void checkBorderCollision(sf::RenderWindow* window);
	bool checkParticleCollision(Ball* ball);
	void resolveParticleCollision(Ball* ball, float dt);
	void handleCollisions(sf::RenderWindow* window, Ball* ballArr, int ballArrSize, float dt);
	void update(float dt);
};

#endif