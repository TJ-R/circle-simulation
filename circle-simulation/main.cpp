#include "ball.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

enum aabbCollisionDirection { TOP, RIGHT, BOTTOM, LEFT };

bool checkAABBCollision(sf::RectangleShape, sf::RectangleShape);

int main() {
  unsigned int windowX{1920};
  unsigned int windowY{1080};
  sf::RenderWindow window(sf::VideoMode({windowX, windowY}),
                          "Circle_Simulation");
  sf::Clock clock;

  float dt = clock.restart().asSeconds();

  const int circleArrSize = 2;
  Ball circleArr[circleArrSize];

  window.setFramerateLimit(240);
  unsigned int unsignedRandom = static_cast<unsigned int>(time(0));
  srand(unsignedRandom);

  // Generate the different size circles
  float currentWindowX = 200;
  float currentWindowY = 200;
  for (int i = 0; i < circleArrSize; i++) {
    Ball ball = Ball(i, 0.f);
    ball.createRandomBall(currentWindowX, currentWindowY);
    circleArr[i] = ball;

    if (currentWindowX + 80 >= windowX - 200) {
      currentWindowX = 200;
      currentWindowY += 80;

      if (currentWindowY + 80 >= windowY - 200) {
        break;
      }
    } else {
      currentWindowX += 80;
    }
  }

  /*
          REFACTOR each loop of for circle make sure we are getting reference to
     collider so that we modify the original
  */
  float totalTime{
      0}; // This is direct-list-initialization rather than copy-initialization
  int fpsCount{0};
  while (window.isOpen()) {
    if (totalTime >= 1) {
      std::cout << "FPS: " << fpsCount << "\n";
      fpsCount = 0;
      totalTime = 0;
    }
    // Gets the elapsed time since the clock was started and restarts the clock
    dt = clock.restart().asSeconds();
    totalTime += dt;
    fpsCount += 1;

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);

    // Does not seem very efficient to do this loop twice scaling wise

    // TODO See if there is a way to just do this event loop once per object
    // effectively
    for (int i = 0; i < circleArrSize; ++i) {
      // checkCollisions for each ball and adjust the movement vector
      // appopriately
      circleArr[i].handleCollisions(&window, circleArr, circleArrSize, dt);
    }

    for (int i = 0; i < circleArrSize; ++i) {
      circleArr[i].update(dt);

      // sf::CircleShape leftMarker = sf::CircleShape(5.f);
      // leftMarker.setPosition({ circleArr[i].getPosition().x,
      // circleArr[i].getPosition().y + (circleArr[i].getRadius())});
      // leftMarker.setPosition(leftMarker.getPosition() -
      // leftMarker.getGeometricCenter());

      // sf::CircleShape rightMarker = sf::CircleShape(5.f);
      // rightMarker.setPosition({ circleArr[i].getPosition().x +
      // (circleArr[i].getRadius() * 2), circleArr[i].getPosition().y +
      // circleArr[i].getRadius()});
      // rightMarker.setPosition(rightMarker.getPosition() -
      // rightMarker.getGeometricCenter());

      // sf::CircleShape topMarker = sf::CircleShape(5.f);
      // topMarker.setPosition({ circleArr[i].getPosition().x +
      // circleArr[i].getRadius(), circleArr[i].getPosition().y});
      // topMarker.setPosition(topMarker.getPosition() -
      // topMarker.getGeometricCenter());

      // sf::CircleShape bottomMarker = sf::CircleShape(5.f);
      // bottomMarker.setPosition({ circleArr[i].getPosition().x +
      // circleArr[i].getRadius(), circleArr[i].getPosition().y +
      // (circleArr[i].getRadius() * 2)});
      // bottomMarker.setPosition(bottomMarker.getPosition() -
      // bottomMarker.getGeometricCenter());

      window.draw(circleArr[i]);
      // window.draw(circleArr[i].getAABBCollider());
      // window.draw(leftMarker);
      // window.draw(rightMarker);
      // window.draw(topMarker);
      // window.draw(bottomMarker);
    }

    window.display();
  }
}

bool checkAABBCollision(sf::RectangleShape s1, sf::RectangleShape s2) {
  const float minS1X = s1.getPosition().x;
  const float minS1Y = s1.getPosition().y;
  const float maxS1X = minS1X + s1.getSize().x;
  const float maxS1Y = minS1Y + s1.getSize().y;

  const float minS2X = s2.getPosition().x;
  const float minS2Y = s2.getPosition().y;
  const float maxS2X = minS2X + s2.getSize().x;
  const float maxS2Y = minS2Y + s2.getSize().y;

  if (minS1X <= maxS2X && maxS1X >= minS2X && minS1Y <= maxS2Y &&
      maxS1Y >= minS2Y) {
    return true;
  }

  return false;
}
