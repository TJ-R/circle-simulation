#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum aabbCollisionDirection {
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
};

bool checkAABBCollision(sf::RectangleShape s1, sf::RectangleShape s2) {
	const float minS1X = s1.getPosition().x;
	const float minS1Y = s1.getPosition().y;
	const float maxS1X = minS1X + s1.getSize().x;
	const float maxS1Y = minS1Y + s1.getSize().y;

	const float minS2X = s2.getPosition().x;
	const float minS2Y = s2.getPosition().y;
	const float maxS2X = minS2X + s2.getSize().x;
	const float maxS2Y = minS2Y + s2.getSize().y;

	if (minS1X <= maxS2X &&
		maxS1X >= minS2X &&
		minS1Y <= maxS2Y &&
		maxS1Y >= minS2Y) {
		return true;
	}


	return false;
}

class Ball : public sf::CircleShape {
	using sf::CircleShape::CircleShape;

	public: 
		void createRandomBall(sf::Vector2u windowSize) {

			sf::Color colors[7] = { sf::Color::White, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };

			// Random Choose color 0 - 6
			int colorIndex = 0 + (rand() % (6 - 0 + 1));
			this->setFillColor(colors[colorIndex]);

			// Random Choose radius 49.f - 50.f
			int radius = 49 + (rand() & (50 - 49 + 1));
			this->setRadius(static_cast<float>(radius));

			// Random position
			float x = static_cast<float>(radius + (rand() % (windowSize.x - (radius*2) + 1)));
			float y = static_cast<float>(radius + (rand() % (windowSize.y - (radius*2) + 1)));
			this->setPosition(sf::Vector2f(x, y));

			// Create AABB Collider Box
			this->setAABBCollider({ radius * 2.1f , radius * 2.1f });
			sf::RectangleShape& collider = this->getAABBCollider();
			collider.setPosition(this->getPosition());

			// Random Movement Vector
			float xSpeed = static_cast<float>(5 + (rand() % (10 - 5 + 1)));
			float ySpeed = static_cast<float>(5 + (rand() % (10 - 5 + 1)));
			this->setMovementVector(sf::Vector2f(xSpeed, ySpeed));

			// Set the AABB collider box
			sf::Vector2f circleCenter = this->getPosition() + this->getGeometricCenter();
			this->AABB_Collider.setPosition(circleCenter - this->AABB_Collider.getGeometricCenter());

		}

		void setMovementVector(sf::Vector2f vector) {
			movementVector = vector;
		}

		sf::Vector2f getMovementVector() {
			return this->movementVector;
		}

		sf::RectangleShape& getAABBCollider() {
			return this->AABB_Collider;
		}

		// Recommendation to set collider bigger than ball size
		void setAABBCollider(sf::Vector2f size) {
			this->AABB_Collider.setSize(size);
			this->AABB_Collider.setOutlineColor(sf::Color::Red);
			this->AABB_Collider.setFillColor(sf::Color::Transparent);
			this->AABB_Collider.setOutlineThickness(3.f);

			sf::Vector2f circleCenter = this->getPosition() + this->getGeometricCenter();
			this->AABB_Collider.setPosition(circleCenter - this->AABB_Collider.getGeometricCenter());
		}

		void checkBorderCollision(sf::RenderWindow* window) {
			// If out of bounds on left or right side flip the x cordinate on the vector
			if (this->AABB_Collider.getPosition().x < 0 || this->AABB_Collider.getPosition().x + this->AABB_Collider.getSize().x > window->getSize().x) {
				this->setMovementVector({ -this->getMovementVector().x, this->getMovementVector().y });
			}

			if (this->AABB_Collider.getPosition().y < 0 || this->AABB_Collider.getPosition().y + this->AABB_Collider.getSize().y > window->getSize().y) {
				this->setMovementVector({ this->getMovementVector().x, -this->getMovementVector().y });
			}
		}

		void update(sf::RenderWindow* window, Ball* ballArr[], int ballArrSize) {
			float currentX = this->getPosition().x;
			float currentY = this->getPosition().y;
			
			this->checkBorderCollision(window);

			// TODO update collision between balls	

			this->move(this->getMovementVector());
			this->AABB_Collider.move(this->getMovementVector());

			
			//bool collision = false;
			//// Colliding with both directions
			//if ((currentX < 0 || currentX > window->getSize().x - (2 * this->getRadius()))
			//	&& (currentY < 0 || currentY > window->getSize().y - (2 * this->getRadius()))) {
			//	// Invert the speeds and move opposite direction
			//	this->setMovementVector(-this->getMovementVector());
			//	this->move(this->getMovementVector());
			//	this->AABB_Collider.move(this->getMovementVector());
			//}
			//else if (currentX < 0 || currentX > window->getSize().x - (2 * this->getRadius())) {
			//	// Invert just the x direction
			//	this->setMovementVector({ -this->getMovementVector().x, this->getMovementVector().y });
			//	this->move(this->getMovementVector());
			//	this->AABB_Collider.move(this->getMovementVector());
			//}
			//else if (currentY < 0 || currentY > window->getSize().y - (2 * this->getRadius())) {
			//	// Invert just the y direction
			//	this->setMovementVector({ this->getMovementVector().x, -this->getMovementVector().y });
			//	this->move(this->getMovementVector());
			//	this->AABB_Collider.move(this->getMovementVector());
			//}
			//else {
			//	this->move(this->getMovementVector());
			//	this->AABB_Collider.move(this->getMovementVector());
			//}
			//else {

			//	for (int i = 0; i < ballArrSize; ++i) {
			//		if (ballArr[i] == this) {
			//			continue;
			//		}

			//		collision = checkAABBCollision(ballArr[i]->getAABBCollider(), this->getAABBCollider());
			//		if (collision) {
			//			break;
			//		}
			//	}
			//}

			//// Set object precollision with inverse vector (with twice the power)
			//// Want to come up with a better system but this will work for now
			//if (collision) {
			//	this->setMovementVector(-this->getMovementVector());
			//	this->move({ this->getMovementVector().x * 2, this->getMovementVector().y * 2 });
			//	collision = false;
			//}
		}

	private: 
		sf::Vector2f movementVector;
		sf::RectangleShape AABB_Collider;
};

using namespace std;
int main() {
	sf::RenderWindow window(sf::VideoMode({ 1200, 900 }), "SFML works!");
	float speed = 3.f;

	Ball* circleArr[6];
	int circleArrSize = sizeof(circleArr) / sizeof(circleArr[0]);

	// Should Make this an enum instead?
	window.setFramerateLimit(60);
	srand(time(0));

	// Generate the different size circles
	for (int i = 0; i < circleArrSize; i++) {
		Ball* ball = new Ball(0.f);
		ball->createRandomBall(window.getSize());
		circleArr[i] = ball;
	}
	/*
		REFACTOR each loop of for circle make sure we are getting reference to collider so that we modify the original
	*/
	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		for (Ball* circle : circleArr) {
			
			float windowWidth = window.getSize().x;
			float windowHeight = window.getSize().y;

			circle->update(&window, circleArr, circleArrSize);

			sf::CircleShape leftMarker = sf::CircleShape(5.f);
			leftMarker.setPosition({ circle->getPosition().x, circle->getPosition().y + (circle->getRadius()) });
			leftMarker.setPosition(leftMarker.getPosition() - leftMarker.getGeometricCenter());

			sf::CircleShape rightMarker = sf::CircleShape(5.f);
			rightMarker.setPosition({ circle->getPosition().x + (circle->getRadius() * 2), circle->getPosition().y + circle->getRadius()});
			rightMarker.setPosition(rightMarker.getPosition() - rightMarker.getGeometricCenter());

			sf::CircleShape topMarker = sf::CircleShape(5.f);
			topMarker.setPosition({ circle->getPosition().x + circle->getRadius(), circle->getPosition().y });
			topMarker.setPosition(topMarker.getPosition() - topMarker.getGeometricCenter());

			sf::CircleShape bottomMarker = sf::CircleShape(5.f);
			bottomMarker.setPosition({ circle->getPosition().x + circle->getRadius(), circle->getPosition().y + (circle->getRadius() * 2) });
			bottomMarker.setPosition(bottomMarker.getPosition() - bottomMarker.getGeometricCenter());

			window.draw(*circle);
			window.draw(circle->getAABBCollider());
			window.draw(leftMarker);
			window.draw(rightMarker);
			window.draw(topMarker);
			window.draw(bottomMarker);
		}

		window.display();
	}
}
