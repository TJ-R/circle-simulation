#include "ball.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <iostream>

Ball::Ball() {}

Ball::Ball(float radius, int windowX, int windowY, float restitiution)
    : CircleShape(radius) {
  this->setFillColor(sf::Color::White);
  this->setRadius(radius);
  this->setPosition({windowX / 2.f - 10.f, windowY / 2.f - 10.f});
  this->setAABBCollider({radius * 2.1f, radius * 2.1f});
  this->setMovementVector(sf::Vector2f(0, 0));
  this->setResitiutionCoefficient(restitiution);

  sf::RectangleShape &collider = this->getAABBCollider();
  collider.setPosition(this->getPosition());

  sf::Vector2f circleCenter = this->getPosition() + this->getGeometricCenter();
  this->AABB_Collider.setPosition(circleCenter -
                                  this->AABB_Collider.getGeometricCenter());
}

void Ball::createRandomBall(float posX, float posY) {
  sf::Color colors[7] = {
      sf::Color::White,  sf::Color::Red,     sf::Color::Green, sf::Color::Blue,
      sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

  // Create AABB Collider Box

  // Random Movement Vector // Only down right

  // Set the AABB collider box
}

void Ball::setMovementVector(sf::Vector2f vector) { movementVector = vector; }

sf::Vector2f Ball::getMovementVector() { return this->movementVector; }

void Ball::setResitiutionCoefficient(float e) { restitution = e; }

sf::RectangleShape &Ball::getAABBCollider() { return this->AABB_Collider; }

// Recommendation to set collider bigger than ball size
void Ball::setAABBCollider(sf::Vector2f size) {
  this->AABB_Collider.setSize(size);
  this->AABB_Collider.setOutlineColor(sf::Color::Red);
  this->AABB_Collider.setFillColor(sf::Color::Transparent);
  this->AABB_Collider.setOutlineThickness(3.f);

  sf::Vector2f circleCenter = this->getPosition() + this->getGeometricCenter();
  this->AABB_Collider.setPosition(circleCenter -
                                  this->AABB_Collider.getGeometricCenter());
}

// int Ball::checkBorderCollisions(sf::RenderWindow *window) {
//   // TODO: Update to not use magic when reducing speeds and ensuring collsion
//   // with border does not exit the wall
//
//   // Left Wall
//   if (this->AABB_Collider.getPosition().x < 0) {
//     this->setPosition({0 + 10, this->getPosition().y});
//     this->setMovementVector({-(this->getMovementVector().x *
//     this->restitution),
//                              this->getMovementVector().y});
//     sf::Vector2f circleCenter =
//         this->getPosition() + this->getGeometricCenter();
//     this->AABB_Collider.setPosition(circleCenter -
//                                     this->AABB_Collider.getGeometricCenter());
//   } else if (this->AABB_Collider.getPosition().x +
//                  this->AABB_Collider.getSize().x >
//              window->getSize().x) {
//     this->setPosition(
//         {static_cast<float>(window->getSize().x) - (this->getRadius() * 2) -
//         10,
//          this->getPosition().y});
//     this->setMovementVector({-(this->getMovementVector().x *
//     this->restitution),
//                              this->getMovementVector().y});
//     sf::Vector2f circleCenter =
//         this->getPosition() + this->getGeometricCenter();
//     this->AABB_Collider.setPosition(circleCenter -
//                                     this->AABB_Collider.getGeometricCenter());
//   }
//
//   if (this->AABB_Collider.getPosition().y < 0) {
//     std::cout << "Top Collision" << std::endl;
//     this->setPosition({this->getPosition().x, 0 + 10});
//     sf::Vector2f circleCenter =
//         this->getPosition() + this->getGeometricCenter();
//     this->AABB_Collider.setPosition(circleCenter -
//                                     this->AABB_Collider.getGeometricCenter());
//
//     this->setMovementVector(
//         {this->getMovementVector().x,
//          -(this->getMovementVector().y * this->restitution)});
//
//   } else if (this->AABB_Collider.getPosition().y +
//                  this->AABB_Collider.getSize().y >
//              window->getSize().y) {
//
//     std::cout << "Bottom Collision" << std::endl;
//     if (this->getMovementVector().y < 10.f) {
//       this->setPosition(
//           {this->getPosition().x,
//            static_cast<float>(window->getSize().y) - (this->getRadius() *
//            2)});
//     } else {
//       // this->setPosition({ this->getPosition().x,
//       // static_cast<float>(window->getSize().y) - (this->getRadius() * 2) -
//       10
//       // });
//       this->setPosition(
//           {this->getPosition().x, static_cast<float>(window->getSize().y) -
//                                       (this->getRadius() * 2) - 3.f});
//     }
//     this->setMovementVector(
//         {this->getMovementVector().x,
//          -(this->getMovementVector().y * this->restitution)});
//     sf::Vector2f circleCenter =
//         this->getPosition() + this->getGeometricCenter();
//     this->AABB_Collider.setPosition(circleCenter -
//                                     this->AABB_Collider.getGeometricCenter());
//   }
// }

// This  function should check the different border for any collisions
// if it finds some collisions return the correct enum value so that it can
// be evaluated properly in resolveBorderCollisions
int Ball::checkBorderCollisions(sf::RenderWindow *window) {
  bool top = false;
  bool right = false;
  bool bottom = false;
  bool left = false;

  float window_x = window->getSize().x;
  float window_y = window->getSize().y;

  float x_pos = this->AABB_Collider.getPosition().x;
  float y_pos = this->AABB_Collider.getPosition().y;

  float x_size = this->AABB_Collider.getSize().x;
  float y_size = this->AABB_Collider.getSize().y;

  if (x_pos < 0) {
    // Check the left wall
    left = true;

  } else if (x_pos + x_size > window_x) {
    // Check the right wall
    right = true;
  }

  if (y_pos < 0) {
    // Check top
    top = true;
  } else if (y_pos + y_size > window_y) {
    // Check bottom
    bottom = true;
  }

  if (top && !right && !bottom && !left) {
    return Top;
  } else if (!top && right && !bottom && !left) {
    return Right;
  } else if (!top && !right && bottom && !left) {
    return Bottom;
  } else if (!top && !right && !bottom && left) {
    return Left;
  } else if (top && right && !bottom && !left) {
    return TR;
  } else if (!top && right && bottom && !left) {
    return BR;
  } else if (!top && !right && bottom && left) {
    return BL;
  } else if (top && !right && !bottom && left) {
    return TL;
  } else {
    return None;
  }
}

// This function should adjust the position of the appropriately and invert the
// movement vector appropriately
void Ball::resolveBorderCollisions(sf::RenderWindow *window, float dt,
                                   int res) {
  switch (res) {
  case Top:
    break;
  case Right:
    break;
  case Bottom:
    break;
  case Left:
    break;
  case TR:
    break;
  case BR:
    break;
  case BL:
    break;
  case TL:
    break;
  }
}

// A  different approach CCD using cirlce casting
// Probably should not be a member function
void Ball::staticCollisions(sf::RenderWindow *window, float dt) {
  // Calculate the Minowski's Sum for every wall
  // Pwall +/- Radius = Vobj * t + Pobj solve for t
  // if t > 1 or t < 0 not going to collide on this frame
  // 0 < t < 1 then going to collide
  //
  //
  //
  // Going to do some position updates in here even though it probably should be
  // done here
  float p_top = 0.f + this->getRadius();
  float p_bot = window->getSize().y - this->getRadius();
  float p_left = 0.f + this->getRadius();
  float p_right = window->getSize().x - this->getRadius();

  float mSumTop = (p_top - this->getPosition().y) / this->getMovementVector().y;
  float mSumBot = (p_bot - this->getPosition().y) / this->getMovementVector().y;
  float mSumLeft =
      (p_left - this->getPosition().x) / this->getMovementVector().x;
  float mSumRight =
      (p_right - this->getPosition().x) / this->getMovementVector().x;

  // Need to determine if any direction if collision is going to happen and then
  // approach it based on which is first
  // Might be good to do some kind of array real quick and only push the ones
  // that are postitive with new object type that tracks the collision code and
  // the time t then solve the min the correct order
  //
  // For example if t=.7 for bottom and  do bottom first for 70% of the frame.
  // Update the position onto the bottom and the vector. Then handle the second
  // wall that was at t=.9 and update the movement vector. Then do the last 10%
  // of the collsion in the final movement direction. Would need a helper
  // function update the positon based on t and dt
}

bool Ball::checkParticleCollision(Ball *ball) {

  // First check if collision is even going to happen
  float b1Radius = this->getRadius();
  float b2Radius = ball->getRadius();

  sf::Vector2f b1Center = this->getPosition() + this->getGeometricCenter();
  sf::Vector2f b2Center = ball->getPosition() + ball->getGeometricCenter();

  // Get Eucldiean distance between the two centers (x2 - x1)^2 + (y2 - y1)^2
  // = radii^2
  double sqrX{pow(b2Center.x - b1Center.x, 2)};
  double sqrY{pow(b2Center.y - b1Center.y, 2)};

  if (sqrX + sqrY <= pow(b1Radius + b2Radius, 2)) {
    return true;
  }

  return false;
}

// Do collision based on future position but instead of after it happens
void Ball::resolveParticleCollision(Ball *ball, float dt) {
  // Following https://www.vobarian.com/collisions/2dcollisions2.pdf
  sf::Vector2f b1Center = this->getPosition() + this->getGeometricCenter();
  sf::Vector2f b2Center = ball->getPosition() + ball->getGeometricCenter();

  sf::Vector2f normalVec =
      sf::Vector2f(b2Center.x - b1Center.x, b2Center.y - b1Center.y);

  sf::Vector2f unitNormalVec =
      normalVec.normalized(); // should be the unit normal
  sf::Vector2f unitTangentVec = sf::Vector2f(-unitNormalVec.y, unitNormalVec.x);

  sf::Vector2f b1VelVec = this->getMovementVector();
  sf::Vector2f b2VelVec = ball->getMovementVector();

  float nV1 = unitNormalVec.dot(b1VelVec);  // normal velocity
  float tV1 = unitTangentVec.dot(b1VelVec); // tangental velocity

  float nV2 = unitNormalVec.dot(b2VelVec);  // normal velocity
  float tV2 = unitTangentVec.dot(b2VelVec); // tangental velocity

  // tangental velocity after collision is same as before since no force in
  // the tangental direction

  // normal velcoity after collision assuming mass of each ball is 1 can
  // update this to use the balls mass if added later
  float m1 = 1;
  float m2 = 1;

  float nPrimeV1 = (nV1 * (m1 - m2) + 2 * m2 * nV2) / (m1 + m2);
  float nPrimeV2 = (nV2 * (m2 - m1) + 2 * m1 * nV1) / (m1 + m2);

  sf::Vector2f nPrimeVec1 = nPrimeV1 * unitNormalVec;
  sf::Vector2f tPrimeVec1 = tV1 * unitTangentVec;

  sf::Vector2f nPrimeVec2 = nPrimeV2 * unitNormalVec;
  sf::Vector2f tPrimeVec2 = tV2 * unitTangentVec;

  sf::Vector2f primeV1Vec = nPrimeVec1 + tPrimeVec1;
  sf::Vector2f primeV2Vec = nPrimeVec2 + tPrimeVec2;

  this->setMovementVector(primeV1Vec);
  ball->setMovementVector(primeV2Vec);

  // Also going to move them away from each other till collison is gone
  while (this->checkParticleCollision(ball)) {
    this->move(this->getMovementVector() * dt);
    this->getAABBCollider().move(this->getMovementVector() * dt);
    ball->move(ball->getMovementVector() * dt);
    ball->getAABBCollider().move(ball->getMovementVector() * dt);
  }
}

// TODO Refactor this next
// Currently ball just bounces endlessly when it is sitting on the border
// since we are adding onto the movementVector endlessly. How should I handle
// the idea of an opposite force canceling out the gravity pulling it down?
void Ball::update(sf::RenderWindow *window, float dt) {
  const float window_y = window->getSize().y;
  const float gravity = window_y / 10.f;

  int borderRes = this->checkBorderCollisions(window);

  if (borderRes != None) {
    this->resolveBorderCollisions(window, dt, borderRes);
  }

  // this->setMovementVector({this->getMovementVector().x,
  //                          this->getMovementVector().y + (gravity * dt)});
  // this->move(this->getMovementVector() * dt);
  // this->AABB_Collider.move(this->getMovementVector() * dt);
  //
  // Apply gravity this feels like it should be a part of
  std::cout << this->getMovementVector().y << "\n";
}
