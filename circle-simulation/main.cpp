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
	public: 
		Ball() {}

		Ball(int newId, float radius) : CircleShape(radius) {
			this->id = newId;
		}

		void setId(int newId) {
			this->id = newId;
		}

		int getId() {
			return this->id;
		}

		// TODO FIX BALLS SPAWNING OUTSIDE OF WINDOW
		void createRandomBall(sf::Vector2u windowSize) {

			sf::Color colors[7] = { sf::Color::White, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };

			// Random Choose color 0 - 6
			int colorIndex = 0 + (rand() % (6 - 0 + 1));
			this->setFillColor(colors[colorIndex]);

			// Random Choose radius 49.f - 50.f
			int radius = 49 + (rand() & (50 - 49 + 1));
			this->setRadius(static_cast<float>(radius));

			// Random position
			float x = static_cast<float>(1 + (rand() % (windowSize.x - 1 + 1)));
			float y = static_cast<float>(1 + (rand() % (windowSize.y - 1 + 1)));
			this->setPosition(sf::Vector2f(x, y));

			// Create AABB Collider Box
			this->setAABBCollider({ radius * 2.1f , radius * 2.1f });
			sf::RectangleShape& collider = this->getAABBCollider();
			collider.setPosition(this->getPosition());

			// Random Movement Vector
			float xSpeed = static_cast<float>(500 + (rand() % (750 - 500 + 1)));
			float ySpeed = static_cast<float>(500 + (rand() % (750 - 500 + 1)));
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

		bool checkParticleCollision(Ball* ball) {

				//First check if collision is even going to happen
				float b1Radius = this->getRadius();
				float b2Radius = ball->getRadius();

				sf::Vector2f b1Center = this->getPosition() - this->getGeometricCenter();
				sf::Vector2f b2Center = ball->getPosition() - ball->getGeometricCenter();

				// Get Eucldiean distance between the two centers sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
				double sqrX = pow(b2Center.x - b1Center.x, 2);
				double sqrY = pow(b2Center.y - b1Center.y, 2);
				double dist = sqrt(sqrX + sqrY);

				if (dist <= b1Radius + b2Radius) {
					std::cout << "Collision Detected" << std::endl;
					return true;
				}

				return false;
		}

		void resolveParticleCollision(Ball* ball, float dt) {
			// Following https://www.vobarian.com/collisions/2dcollisions2.pdf
			
			sf::Vector2f normalVec = sf::Vector2f(ball->getPosition().x - this->getPosition().x, ball->getPosition().y - this->getPosition().y);

			sf::Vector2f unitNormalVec = normalVec.normalized(); // should be the unit normal
			sf::Vector2f unitTangentVec = sf::Vector2f(-unitNormalVec.y, unitNormalVec.x);
			
			sf::Vector2f b1VelVec = this->getMovementVector();
			sf::Vector2f b2VelVec = ball->getMovementVector();
			
			float nV1 = unitNormalVec.dot(b1VelVec);  // normal velocity
			float tV1 = unitTangentVec.dot(b1VelVec); // tangental velocity

			float nV2 = unitNormalVec.dot(b2VelVec);  // normal velocity
			float tV2 = unitTangentVec.dot(b2VelVec); // tangental velocity

			// tangental velocity after collision is same as before since no force in the tangental direction
			
			// normal velcoity after collision assuming mass of each ball is 1 can update this to use the balls mass
			// if added later
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

		void handleCollisions (sf::RenderWindow* window, Ball* ballArr, int ballArrSize, float dt) {
			this->checkBorderCollision(window);
			// TODO update collision between balls	
			// There has to be a better way to loop through this rather than checking every ball against every other ball to determine collisions
			// and responding

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

		void update(float dt) {
			this->move(this->getMovementVector() * dt);
			this->AABB_Collider.move(this->getMovementVector() * dt);
		}

	private: 
		sf::Vector2f movementVector;
		sf::RectangleShape AABB_Collider;
		int id{};
};

using namespace std;
int main() {
	sf::RenderWindow window(sf::VideoMode({ 1200, 900 }), "SFML works!");
	sf::Clock clock;

	float dt = clock.restart().asSeconds();

	const int circleArrSize = 100;
	Ball circleArr[circleArrSize];

	// Should Make this an enum instead?
	window.setFramerateLimit(240);
	srand(unsigned int(time(0)));


	// Generate the different size circles
	for (int i = 0; i < circleArrSize; i++) {
		Ball ball = Ball(i, 0.f);
		ball.createRandomBall(window.getSize());
		circleArr[i] = ball;
	}
	/*
		REFACTOR each loop of for circle make sure we are getting reference to collider so that we modify the original
	*/
	while (window.isOpen())
	{
		// Gets the elapsed time since the clock was started and restarts the clock
		dt = clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		// Does not seem very efficient to do this loop twice scaling wise

		// TODO See if there is a way to just do this event loop once per object effectively
		for (int i = 0; i < circleArrSize; ++i) {
			// checkCollisions for each ball and adjust the movement vector appopriately 
			circleArr[i].handleCollisions(&window, circleArr, circleArrSize, dt);
		}

		for (int i = 0; i < circleArrSize; ++i) {
			circleArr[i].update(dt);

			sf::CircleShape leftMarker = sf::CircleShape(5.f);
			leftMarker.setPosition({ circleArr[i].getPosition().x, circleArr[i].getPosition().y + (circleArr[i].getRadius())});
			leftMarker.setPosition(leftMarker.getPosition() - leftMarker.getGeometricCenter());

			sf::CircleShape rightMarker = sf::CircleShape(5.f);
			rightMarker.setPosition({ circleArr[i].getPosition().x + (circleArr[i].getRadius() * 2), circleArr[i].getPosition().y + circleArr[i].getRadius()});
			rightMarker.setPosition(rightMarker.getPosition() - rightMarker.getGeometricCenter());

			sf::CircleShape topMarker = sf::CircleShape(5.f);
			topMarker.setPosition({ circleArr[i].getPosition().x + circleArr[i].getRadius(), circleArr[i].getPosition().y});
			topMarker.setPosition(topMarker.getPosition() - topMarker.getGeometricCenter());

			sf::CircleShape bottomMarker = sf::CircleShape(5.f);
			bottomMarker.setPosition({ circleArr[i].getPosition().x + circleArr[i].getRadius(), circleArr[i].getPosition().y + (circleArr[i].getRadius() * 2)});
			bottomMarker.setPosition(bottomMarker.getPosition() - bottomMarker.getGeometricCenter());

			window.draw(circleArr[i]);
			window.draw(circleArr[i].getAABBCollider());
			window.draw(leftMarker);
			window.draw(rightMarker);
			window.draw(topMarker);
			window.draw(bottomMarker);
		}

		window.display();
	}
}
