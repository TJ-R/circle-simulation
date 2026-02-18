#include <SFML/Graphics.hpp>

#ifndef BALL_H
#define BALL_H

enum BorderCollision {
  None = 0,
  Top = 1,
  Right = 2,
  Bottom = 3,
  Left = 4,
  TR = 5,
  BR = 6,
  BL = 7,
  TL = 8
};

class Ball : public sf::CircleShape {
private:
  sf::Vector2f movementVector{};
  sf::RectangleShape AABB_Collider{};

  float restitution;

public:
  Ball();
  Ball(float, int, int, float); // Inherit from CircleShape
                                //
  void createRandomBall(float, float);

  void setMovementVector(sf::Vector2f);
  sf::Vector2f getMovementVector();

  void setResitiutionCoefficient(float r);

  void setAABBCollider(sf::Vector2f size);
  sf::RectangleShape &getAABBCollider();

  bool checkParticleCollision(Ball *ball);
  void resolveParticleCollision(Ball *ball, float dt);

  int checkBorderCollisions(sf::RenderWindow *window);
  void resolveBorderCollisions(sf::RenderWindow *window, float dt, int res);

  // Minkowski's Sum
  void staticCollisions(sf::RenderWindow *window, float dt);

  void update(sf::RenderWindow *window, float dt);
};

#endif
