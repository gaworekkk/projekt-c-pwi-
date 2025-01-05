#include "ObstacleManager.h"
#include <cstdlib>
#include <ctime>

enum GameState { MainMenu, OptionsMenu, Gameplay, Pause, GameOver }; 

extern GameState gameState;

ObstacleManager::ObstacleManager(float windowWidth, float windowHeight)
    : screenWidth(windowWidth), screenHeight(windowHeight), obstacleSpawnTimer(0.f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Inicjalizacja generatora losowego
    speed = 250.f; // Początkowa prędkość przeszkody w jednostkach na sekundę
    spawnInterval = getRandomSpawnInterval();
}

void ObstacleManager::update(float deltaTime) {
    obstacleSpawnTimer += deltaTime;

    // Generowanie przeszkód co pewien czas
    if (obstacleSpawnTimer >= spawnInterval) {
        generateObstacle();
        obstacleSpawnTimer = 0.f;
        spawnInterval = getRandomSpawnInterval();
    }

    // Aktualizacja przeszkód
    for (auto& obstacle : obstacles) {
        obstacle.update(deltaTime, speed);
    }

    // Usuwanie przeszkód poza ekranem
    removeOffscreenObstacles();
}

void ObstacleManager::draw(sf::RenderWindow& window) {
    for (auto& obstacle : obstacles) {
        obstacle.draw(window);
    }
}

void ObstacleManager::generateObstacle() {
    float obstacleHeight = static_cast<float>(rand() % 50 + 50);  // Losowa wysokość przeszkody (od 50 do 100)
    float obstacleWidth = 50.f;  // Szerokość przeszkody
    float obstacleY = groundHeight - obstacleHeight;  // Pozycja Y przeszkody (na ziemii)

    // Tworzenie przeszkody po prawej stronie ekranu
    obstacles.push_back(Obstacle(screenWidth, obstacleY, obstacleWidth, obstacleHeight));
}

bool ObstacleManager::checkCollisions(const sf::FloatRect& playerBounds) {
    for (auto& obstacle : obstacles) {
        if (playerBounds.intersects(obstacle.getBounds())) {
            gameState = GameOver;
            return true;
        }
    }
    return false;
}

void ObstacleManager::removeOffscreenObstacles() {
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obstacle) {
        return obstacle.getBounds().left + obstacle.getBounds().width < 0; // Sprawdzanie, czy przeszkoda jest poza ekranem
    }), obstacles.end());
}
void ObstacleManager::restart() {
    obstacles.clear();  // Usuwamy wszystkie przeszkody
    obstacleSpawnTimer = 0.f;   // Resetujemy timer spawnu
}

std::vector<sf::FloatRect> ObstacleManager::getObstacleBounds() const {
    std::vector<sf::FloatRect> bounds;
    for (const auto& obstacle : obstacles) {
        bounds.push_back(obstacle.getBounds());
    }
    return bounds;
}

float ObstacleManager::getRandomSpawnInterval() const {
    return 1.5f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 2.5f)); 
    // Losowe generowanie czasu spawnu przeszkód od 1.5 do 4 sekund
}

float ObstacleManager::getSpeed() const {
    return speed;
}

void ObstacleManager::setSpeed(float newSpeed) {
    // Zmiana prędkości przeszkody
    speed = newSpeed;
}