#include "ball.h"
#include <SFML/Graphics.hpp>
#include <iostream>

enum aabbCollisionDirection { TOP, RIGHT, BOTTOM, LEFT };

bool checkAABBCollision(sf::RectangleShape, sf::RectangleShape);

int main() {
  unsigned int windowX{1280};
  unsigned int windowY{720};
  sf::RenderWindow window(sf::VideoMode({windowX, windowY}),
                          "Circle_Simulation");
  sf::Clock clock;

  float dt = clock.restart().asSeconds();

  window.setFramerateLimit(120);

  float currentWindowX = 200;
  float currentWindowY = 200;

  Ball ball = Ball(20.f, windowX, windowY, 0.8);

  float totalTime{
      0}; // This is direct-list-initialization rather than copy-initialization
  int fpsCount{0};

  // Event Loop
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

    ball.update(&window, dt);

    window.draw(ball);
    window.draw(ball.getAABBCollider());
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
