#include "ball.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

Ball::Ball() {}

Ball::Ball(int newId, float radius) : CircleShape(radius) { this->id = newId; }

void Ball::setId(int newId) { this->id = newId; }

int Ball::getId() { return this->id; }

// TODO How to handle the initial collision of balls spawing on top of one
// another causes tunneling outside of the border of the window and then they
// can't reenter might have to change when the balls are moved during a
// collision
void Ball::createRandomBall(float posX, float posY) {
  sf::Color colors[7] = {
      sf::Color::White,  sf::Color::Red,     sf::Color::Green, sf::Color::Blue,
      sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan};

  // Random math works
  // min + ((srand) % (max - min + 1));

  // Random Choose color 0 - 6
  int colorIndex = 0 + (rand() % (6 - 0 + 1));
  this->setFillColor(colors[colorIndex]);

  // Random Choose radius 20.f - 30.f
  int radius = 20 + (rand() & (30 - 20 + 1));
  this->setRadius(static_cast<float>(radius));

  this->setPosition(sf::Vector2f(posX, posY));

  // Create AABB Collider Box
  this->setAABBCollider({radius * 2.1f, radius * 2.1f});
  sf::RectangleShape &collider = this->getAABBCollider();
  collider.setPosition(this->getPosition());

  // Random Movement Vector // Only down right
  float xSpeed = static_cast<float>(100 + (rand() % (200 - 100 + 1)));
  float ySpeed = static_cast<float>(100 + (rand() % (200 - 100 + 1)));
  this->setMovementVector(sf::Vector2f(xSpeed, ySpeed));

  // Set the AABB collider box
  sf::Vector2f circleCenter = this->getPosition() + this->getGeometricCenter();
  this->AABB_Collider.setPosition(circleCenter -
                                  this->AABB_Collider.getGeometricCenter());
}

void Ball::setMovementVector(sf::Vector2f vector) { movementVector = vector; }

sf::Vector2f Ball::getMovementVector() { return this->movementVector; }

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

void Ball::checkBorderCollision(sf::RenderWindow *window) {
  // TODO: Update to not use magic when reducing speeds and ensuring collsion
  // with border does not exit the wall
  if (this->AABB_Collider.getPosition().x < 0) {
    this->setPosition({0 + 10, this->getPosition().y});
    this->setMovementVector(
        {-(this->getMovementVector().x * .5f), this->getMovementVector().y});
    sf::Vector2f circleCenter =
        this->getPosition() + this->getGeometricCenter();
    this->AABB_Collider.setPosition(circleCenter -
                                    this->AABB_Collider.getGeometricCenter());
  } else if (this->AABB_Collider.getPosition().x +
                 this->AABB_Collider.getSize().x >
             window->getSize().x) {
    this->setPosition(
        {static_cast<float>(window->getSize().x) - (this->getRadius() * 2) - 10,
         this->getPosition().y});
    this->setMovementVector(
        {-(this->getMovementVector().x * .5f), this->getMovementVector().y});
    sf::Vector2f circleCenter =
        this->getPosition() + this->getGeometricCenter();
    this->AABB_Collider.setPosition(circleCenter -
                                    this->AABB_Collider.getGeometricCenter());
  }

  if (this->AABB_Collider.getPosition().y < 0) {
    this->setPosition({this->getPosition().x, 0 + 10});
    this->setMovementVector(
        {this->getMovementVector().x, -(this->getMovementVector().y * .5f)});
    sf::Vector2f circleCenter =
        this->getPosition() + this->getGeometricCenter();
    this->AABB_Collider.setPosition(circleCenter -
                                    this->AABB_Collider.getGeometricCenter());

  } else if (this->AABB_Collider.getPosition().y +
                 this->AABB_Collider.getSize().y >
             window->getSize().y) {
    if (this->getMovementVector().y < 10.f) {
      this->setPosition(
          {this->getPosition().x,
           static_cast<float>(window->getSize().y) - (this->getRadius() * 2)});
    } else {
      // this->setPosition({ this->getPosition().x,
      // static_cast<float>(window->getSize().y) - (this->getRadius() * 2) - 10
      // });
      this->setPosition(
          {this->getPosition().x, static_cast<float>(window->getSize().y) -
                                      (this->getRadius() * 2) - 3.f});
    }
    this->setMovementVector(
        {this->getMovementVector().x, -(this->getMovementVector().y * .5f)});
    sf::Vector2f circleCenter =
        this->getPosition() + this->getGeometricCenter();
    this->AABB_Collider.setPosition(circleCenter -
                                    this->AABB_Collider.getGeometricCenter());
  }
}

bool Ball::checkParticleCollision(Ball *ball) {

  // First check if collision is even going to happen
  float b1Radius = this->getRadius();
  float b2Radius = ball->getRadius();

  sf::Vector2f b1Center = this->getPosition() + this->getGeometricCenter();
  sf::Vector2f b2Center = ball->getPosition() + ball->getGeometricCenter();

  // Get Eucldiean distance between the two centers (x2 - x1)^2 + (y2 - y1)^2 =
  // radii^2
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

  // tangental velocity after collision is same as before since no force in the
  // tangental direction

  // normal velcoity after collision assuming mass of each ball is 1 can update
  // this to use the balls mass if added later
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

void Ball::handleCollisions(sf::RenderWindow *window, Ball *ballArr,
                            int ballArrSize, float dt) {
  this->checkBorderCollision(window);
  // TODO update collision between balls
  // There has to be a better way to loop through this rather than checking
  // every ball against every other ball to determine collisions and responding

  for (int i = 0; i < ballArrSize; ++i) {
    // skip ball if it is itself
    if (this->getId() == ballArr[i].getId()) {
      continue;
    }

    if (this->checkParticleCollision(&ballArr[i])) {
      this->resolveParticleCollision(&ballArr[i], dt);
    }
  }
}

void Ball::update(float dt) {
  this->move(this->getMovementVector() * dt);
  this->AABB_Collider.move(this->getMovementVector() * dt);

  // Apply
  this->setMovementVector(
      {this->getMovementVector().x, this->getMovementVector().y + (15.f * dt)});
  std::cout << this->getMovementVector().y << "\n";
}
