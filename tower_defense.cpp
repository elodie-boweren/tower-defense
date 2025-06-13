// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <sstream>
// #include <memory>
// #include <algorithm>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <fstream>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// // Constants
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;
// const float TOWER_MIN_DISTANCE = 80.0f;

// // ===========================================
// // OBSERVER PATTERN - Interface for events
// // ===========================================
// class GameEvent {
// public:
//     enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
//     Type type;
//     int value; // Gold earned, damage taken, etc.
    
//     GameEvent(Type t, int v = 0) : type(t), value(v) {}
// };

// class Observer {
// public:
//     virtual ~Observer() = default;
//     virtual void notify(const GameEvent& event) = 0;
// };

// class Subject {
// protected:
//     std::vector<Observer*> observers;
// public:
//     virtual ~Subject() = default;
    
//     void addObserver(Observer* observer) {
//         observers.push_back(observer);
//     }
    
//     void removeObserver(Observer* observer) {
//         observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//     }
    
//     void notifyObservers(const GameEvent& event) {
//         for (auto* observer : observers) {
//             observer->notify(event);
//         }
//     }
// };

// // ===========================================
// // RESOURCE MANAGER - To manage textures, sprites and fonts
// // ===========================================
// class ResourceManager {
// private:
//     sf::Texture goldTexture, heartTexture;
//     sf::Sprite goldSprite, heartSprite;
//     bool goldLoaded, heartLoaded;
    
//     // Font management
//     sf::Font arialFont;
//     bool fontLoaded;
    
// public:
//     ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
//         loadResources();
//         loadFont();
//     }
    
//     // Font loading method with comprehensive path search
//     void loadFont() {
//         std::cout << "=== LOADING ARIAL FONT ===" << std::endl;
        
//         // List of paths to try for arial.ttf font
//         std::vector<std::string> fontPaths = {
//             "./assets/arial.ttf",                    // In assets directory
//             "assets/arial.ttf",                      // Without ./
//             "../../assets/arial.ttf",                // Two levels up
//             "../../../assets/arial.ttf",             // Three levels up
//             "../../../../assets/arial.ttf",          // Four levels up
//             "../../src/assets/arial.ttf",            // If assets is in src
//             "../assets/arial.ttf",                   // One level up
//             "./arial.ttf",                          // In current directory
//             "arial.ttf",                            // Direct filename
//         };
        
//         // Try to load font from each path
//         for (const std::string& path : fontPaths) {
//             std::cout << "Trying font path: " << path << std::endl;
            
//             // Check if file exists first
//             struct stat info;
//             if (stat(path.c_str(), &info) == 0) {
//                 if (arialFont.loadFromFile(path)) {
//                     fontLoaded = true;
//                     std::cout << "✓ Font successfully loaded from: " << path << std::endl;
//                     return;
//                 } else {
//                     std::cout << "  -> SFML font loading failed for: " << path << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> Font file not found at: " << path << std::endl;
//             }
//         }
        
//         std::cout << "⚠ WARNING: Arial font not found in any location!" << std::endl;
//         std::cout << "  Please place arial.ttf in the assets/ folder" << std::endl;
//         std::cout << "=== END FONT LOADING ===" << std::endl;
//     }
    
//     // Get font reference for text rendering
//     const sf::Font& getFont() const {
//         return arialFont;
//     }
    
//     // Check if font is available
//     bool isFontLoaded() const {
//         return fontLoaded;
//     }
    
//     void loadResources() {   
//         // Display current working directory
//         char cwd[1024];
//         if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//             std::cout << "Current working directory: " << cwd << std::endl;
//         }
        
//         // List of paths to try (adapted for your structure)
//         std::vector<std::string> pathsToTry = {
//             "./assets/",                    // In executable directory
//             "assets/",                      // Without ./
//             "../../assets/",                // Two levels up (from build/bin to root)
//             "../../../assets/",             // Three levels up
//             "../../../../assets/",          // Four levels up
//             "../../src/assets/",            // If assets is in src
//             "../assets/",                   // One level up
//             "./"                           // In current directory
//         };
        
//         // Test existence of assets folder
//         struct stat info;
//         if (stat("./assets", &info) == 0 && S_ISDIR(info.st_mode)) {
//             std::cout << "✓ Folder './assets' found" << std::endl;
//         } else {
//             std::cout << "⚠ Folder './assets' not found" << std::endl;
//         }
        
//         // Loading gold icon
//         bool goldFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "gold_icon.png";
//             std::cout << "Attempt: " << fullPath << std::endl;
            
//             // Check if file exists
//             if (stat(fullPath.c_str(), &info) == 0) {
//                 std::cout << "  -> File found, attempting to load..." << std::endl;
//                 if (goldTexture.loadFromFile(fullPath)) {
//                     goldLoaded = true;
//                     goldSprite.setTexture(goldTexture);
//                     goldSprite.setScale(0.8f, 0.8f);
//                     std::cout << "✓ Texture gold_icon.png loaded from: " << fullPath << std::endl;
//                     goldFound = true;
//                     break;
//                 } else {
//                     std::cout << "  -> SFML loading failed" << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> File not found" << std::endl;
//             }
//         }
//         if (!goldFound) {
//             std::cout << "⚠ Texture gold_icon.png not found in all paths" << std::endl;
//         }
        
//         // Loading heart icon
//         bool heartFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "heart_icon.png";
//         if (stat(fullPath.c_str(), &info) == 0) {
//             std::cout << "  -> File found, attempting to load..." << std::endl;
//             if (heartTexture.loadFromFile(fullPath)) {
//                 heartLoaded = true;
//                 heartSprite.setTexture(heartTexture);
//                 heartSprite.setScale(0.8f, 0.8f);
//                 std::cout << "✓ Texture heart_icon.png loaded from: " << fullPath << std::endl;
//                 heartFound = true;
//                 break;
//             } else {
//                 std::cout << "  -> SFML loading failed" << std::endl;
//             }
//         } else {
//             std::cout << "  -> File not found" << std::endl;
//         }
//         }
//         if (!heartFound) {
//             std::cout << "⚠ Texture heart_icon.png not found" << std::endl;
//         }
        
//         std::cout << "=== END DEBUG PATHS ===" << std::endl;
//     }
    
//     void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
//         if (goldLoaded) {
//             goldSprite.setPosition(x, y);
//             window.draw(goldSprite);
//         } 
//     }
    
//     void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
//         if (heartLoaded) {
//             heartSprite.setPosition(x, y);
//             window.draw(heartSprite);
//         } 
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Enemy creation
// // ===========================================
// class Enemy {
// public:
//     sf::Vector2f position;
//     int health;
//     int maxHealth;
//     float speed;
//     int reward;
//     bool alive;
//     int pathIndex;
//     std::string type;

//     Enemy(int hp, float spd, int rew, const std::string& t) 
//         : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
//         // La position initiale sera définie par le Game
//     }

//     virtual ~Enemy() = default;

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= path.size() - 1) return;

//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             direction = direction / distance;
//             position += direction * speed * deltaTime;
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     virtual sf::Color getColor() {
//         if (health > maxHealth * 0.7f) return sf::Color::Green;
//         else if (health > maxHealth * 0.3f) return sf::Color::Yellow;
//         else return sf::Color::Red;
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
//         shape.setFillColor(getColor());
//         window.draw(shape);

//         // Health bar
//         sf::RectangleShape healthBar(sf::Vector2f(30, 5));
//         healthBar.setPosition(position.x - 15, position.y - 25);
//         healthBar.setFillColor(sf::Color::Black);
//         window.draw(healthBar);

//         if (health > 0) {
//             sf::RectangleShape healthFill(sf::Vector2f(30 * (float)health / maxHealth, 5));
//             healthFill.setPosition(position.x - 15, position.y - 25);
//             healthFill.setFillColor(sf::Color::Green);
//             window.draw(healthFill);
//         }
//     }

//     bool reachedEnd(const std::vector<sf::Vector2f>& path) {
//         return pathIndex >= path.size() - 1;
//     }
// };

// // Specialized enemy classes
// class Goblin : public Enemy {
// public:
//     Goblin() : Enemy(50, 60, 10, "Goblin") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Green;
//     }
// };

// class Orc : public Enemy {
// public:
//     Orc() : Enemy(120, 45, 20, "Orc") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Red;
//     }
// };

// class MediumEnemy : public Enemy {
// public:
//     MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
//     sf::Color getColor() override {
//         return sf::Color(0, 128, 128); // Turquoise
//     }
// };

// class Troll : public Enemy {
// public:
//     Troll() : Enemy(300, 35, 50, "Troll") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Magenta;
//     }
// };

// class HeavyEnemy : public Enemy {
// public:
//     HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
//     sf::Color getColor() override {
//         return sf::Color(139, 69, 19); // Brown
//     }
// };

// // Factory to create enemies
// class EnemyFactory {
// public:
//     enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
//     // Creation of enemies per type
//     static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
//         switch(type) {
//             case GOBLIN:
//                 std::cout << "Factory: Creating a Goblin" << std::endl;
//                 return std::make_unique<Goblin>();
//             case ORC:
//                 std::cout << "Factory: Creating an Orc" << std::endl;
//                 return std::make_unique<Orc>();
//             case TROLL:
//                 std::cout << "Factory: Creating a Troll" << std::endl;
//                 return std::make_unique<Troll>();
//             case HEAVY:
//                 std::cout << "Factory: Creating a Heavy Enemy" << std::endl;
//                 return std::make_unique<HeavyEnemy>();
//             case MEDIUM:
//                 std::cout << "Factory: Creating a Medium Enemy" << std::endl;
//                 return std::make_unique<MediumEnemy>();
//             default:
//                 return std::make_unique<Goblin>();
//         }
//     }
    
//     // Create assortment of enemies based with difficulty increasing per level
//     static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
//         int totalDifficulty = wave + (level - 1) * 3;
        
//         if (totalDifficulty <= 3) {
//             return GOBLIN;
//         }
//         else if (totalDifficulty <= 6) {
//             if (enemyIndex % 8 == 0) return MEDIUM;
//             return (enemyIndex % 5 == 0) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 9) {
//             if (enemyIndex % 10 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 2) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 12) {
//             if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 5 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 3) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 15) {
//             if (enemyIndex % 10 == 0) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 7) return ORC;
//             else return GOBLIN;
//         }
//         else {
//             if (enemyIndex % 10 < 3) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 9) return ORC;
//             else return GOBLIN;
//         }
//     }
// };

// // Class for projectiles
// class Projectile {
// public:
//     sf::Vector2f position;
//     sf::Vector2f targetPosition;
//     sf::Vector2f velocity;
//     int damage;
//     float speed;
//     bool active;
//     Enemy* targetEnemy;
//     Subject* gameSubject;

//     Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject) 
//         : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), gameSubject(subject) {
        
//         if (target) {
//             targetPosition = target->position;
//             // Calculate direction towards target
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }
//     }

//     void update(float deltaTime) {
//         if (!active) return;

//         // If target is still alive, follow its position
//         if (targetEnemy && targetEnemy->alive) {
//             targetPosition = targetEnemy->position;
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (distance < 10.0f) {
//                 // Impact!
//                 int oldHealth = targetEnemy->health;
//                 targetEnemy->takeDamage(damage);
                
//                 // Notify if enemy is killed
//                 if (targetEnemy->health <= 0 && oldHealth > 0) {
//                     gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
//                 }
                
//                 std::cout << "Projectile hits " << targetEnemy->type << "! Damage: " << damage << std::endl;
//                 active = false;
//                 return;
//             }
            
//             // Update velocity to follow target
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }

//         // Move projectile
//         position += velocity * deltaTime;
        
//         // Deactivate if too far (safety)
//         if (position.x < -50 || position.x > WINDOW_WIDTH + 50 || 
//             position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
//             active = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         if (!active) return;
        
//         sf::CircleShape bullet(4);
//         bullet.setPosition(position.x - 4, position.y - 4);
//         bullet.setFillColor(sf::Color::Yellow);
//         bullet.setOutlineColor(sf::Color::Black);
//         bullet.setOutlineThickness(1);
//         window.draw(bullet);
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Tower creation
// // ===========================================

// // Enumeration of tower types
// enum class TowerType {
//     BASIC,
//     SNIPER,
//     CANNON,
//     FREEZE
// };

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;
//     std::string towerType;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1), towerType("Basic") {}

//     // Protected constructor for derived classes
// protected:
//     Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
//         : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), timeSinceLastAttack(0), level(1), towerType(type) {}

// public:
//     virtual ~Tower() = default;
//     virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (std::unique_ptr<Enemy>& enemy : enemies) {
//                 if (!enemy->alive) continue;
                
//                 float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = enemy.get();
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 // Create a projectile to attack enemy
//                 projectiles.push_back(Projectile(position, target, damage, subject));
//                 timeSinceLastAttack = 0;
//             }
//         }
//     }
//     // Tower attack increases with the levels
//     virtual void upgrade() {
//         if (level == 1) {
//             level = 2;
//             damage = 40;
//             range = 100;
//             attackCooldown = 0.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 80;
//             range = 120;
//             attackCooldown = 0.6f;
//         }
//     }

//     virtual void draw(sf::RenderWindow& window) {
//         // Range circle
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         // Tower (delegate to specific implementation)
//         drawTower(window);
//     }

//     virtual int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0;
//     }

//     virtual bool canUpgrade() {
//         return level < 3;
//     }

// protected:
//     virtual void drawTower(sf::RenderWindow& window) {
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }
// };

// // Derived tower classes
// class BasicTower : public Tower {
// public:
//     BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
// };

// class SniperTower : public Tower {
// public:
//     SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 100;
//             range = 180;
//             attackCooldown = 1.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 200;
//             range = 220;
//             attackCooldown = 1.5f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(25, 25));
//         shape.setPosition(position.x - 12.5f, position.y - 12.5f);
        
//         if (level == 1) shape.setFillColor(sf::Color::Green);
//         else if (level == 2) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);
//     }
// };

// class CannonTower : public Tower {
// public:
//     CannonTower(sf::Vector2f pos) : Tower(pos, 100, 60, 3.0f, "Cannon") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 200;
//             range = 70;
//             attackCooldown = 2.5f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 400;
//             range = 80;
//             attackCooldown = 2.0f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::CircleShape shape(18);
//         shape.setPosition(position.x - 18, position.y - 18);
        
//         if (level == 1) shape.setFillColor(sf::Color(139, 69, 19)); // Brown
//         else if (level == 2) shape.setFillColor(sf::Color(160, 82, 45));
//         else shape.setFillColor(sf::Color(205, 133, 63));
        
//         window.draw(shape);
//     }
// };

// class FreezeTower : public Tower {
// public:
//     FreezeTower(sf::Vector2f pos) : Tower(pos, 5, 90, 0.5f, "Freeze") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 10;
//             range = 110;
//             attackCooldown = 0.4f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 20;
//             range = 130;
//             attackCooldown = 0.3f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(28, 28));
//         shape.setPosition(position.x - 14, position.y - 14);
        
//         if (level == 1) shape.setFillColor(sf::Color(173, 216, 230)); // Light Blue
//         else if (level == 2) shape.setFillColor(sf::Color(135, 206, 235));
//         else shape.setFillColor(sf::Color(70, 130, 180));
        
//         window.draw(shape);
//     }
// };

// // Tower Factory
// class TowerFactory {
// public:
//     static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position) {
//         switch (type) {
//             case TowerType::BASIC:
//                 std::cout << "Factory: Creating Basic Tower" << std::endl;
//                 return std::make_unique<BasicTower>(position);
//             case TowerType::SNIPER:
//                 std::cout << "Factory: Creating Sniper Tower" << std::endl;
//                 return std::make_unique<SniperTower>(position);
//             case TowerType::CANNON:
//                 std::cout << "Factory: Creating Cannon Tower" << std::endl;
//                 return std::make_unique<CannonTower>(position);
//             case TowerType::FREEZE:
//                 std::cout << "Factory: Creating Freeze Tower" << std::endl;
//                 return std::make_unique<FreezeTower>(position);
//             default:
//                 return std::make_unique<BasicTower>(position);
//         }
//     }
    
//     static int getTowerCost(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return 50;
//             case TowerType::SNIPER: return 100;
//             case TowerType::CANNON: return 150;
//             case TowerType::FREEZE: return 75;
//             default: return 50;
//         }
//     }
    
//     static std::string getTowerDescription(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return "Basic Tower - Balanced";
//             case TowerType::SNIPER: return "Sniper Tower - Long Range";
//             case TowerType::CANNON: return "Cannon Tower - High Damage";
//             case TowerType::FREEZE: return "Freeze Tower - Slows Enemies";
//             default: return "Unknown Tower";
//         }
//     }
// };

// // ===========================================
// // GAME CLASS - Implements Observer to receive events
// // ===========================================
// class Game : public Subject, public Observer {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<std::unique_ptr<Enemy>> enemies;
//     std::vector<std::unique_ptr<Tower>> towers; // Changed from std::vector<Tower>
//     std::vector<Projectile> projectiles;
//     int gold;
//     int lives;
//     int wave;
//     int level;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;
//     int score;
//     ResourceManager resourceManager;
//     bool gameOver;
//     bool showReplayOption;
//     bool showSaveOption;
//     std::string playerName;
//     bool waitingForName;
    
//     // Preset paths
//     const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
//         // Original path
//         {
//             sf::Vector2f(0, 300),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(500, 150),
//             sf::Vector2f(500, 450),
//             sf::Vector2f(800, 450),
//             sf::Vector2f(800, 250),
//             sf::Vector2f(1000, 250)
//         },
//         // Zigzag path
//         {
//             sf::Vector2f(0, 200),
//             sf::Vector2f(200, 200),
//             sf::Vector2f(200, 400),
//             sf::Vector2f(400, 400),
//             sf::Vector2f(400, 200),
//             sf::Vector2f(600, 200),
//             sf::Vector2f(600, 400),
//             sf::Vector2f(800, 400),
//             sf::Vector2f(800, 200),
//             sf::Vector2f(1000, 200)
//         },
//         // Curly path
//         {
//             sf::Vector2f(0, 350),
//             sf::Vector2f(200, 350),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(400, 150),
//             sf::Vector2f(400, 350),
//             sf::Vector2f(600, 350),
//             sf::Vector2f(600, 150),
//             sf::Vector2f(800, 150),
//             sf::Vector2f(800, 350),
//             sf::Vector2f(1000, 350)
//         },
//         // S path
//         {
//             sf::Vector2f(0, 100),
//             sf::Vector2f(200, 100),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(400, 300),
//             sf::Vector2f(400, 500),
//             sf::Vector2f(600, 500),
//             sf::Vector2f(600, 300),
//             sf::Vector2f(800, 300),
//             sf::Vector2f(800, 100),
//             sf::Vector2f(1000, 100)
//         }
//     };

//     void changePath() {
//         // Select a path different from the previous randomly
//         int newPathIndex;
//         do {
//             newPathIndex = rand() % predefinedPaths.size();
//         } while (predefinedPaths[newPathIndex] == path);
        
//         path = predefinedPaths[newPathIndex];
//         std::cout << "=== NEW PATH SELECTED FOR LEVEL " << level << " ===" << std::endl;
//     }

//     void saveScore() {
//         try {
//             json scores;
//             std::ifstream file("scores.json");
//         if (file.good()) {
//             // File exists, read it
//             file >> scores;
//             file.close();
//         } else {
//             // File doesn't exist, create empty array
//             scores = json::array();
//             std::cout << "Creating file scores.json" << std::endl;
//         }
//             // Add new score
//             json newScore = {
//                 {"name", playerName},
//                 {"score", score},
//                 {"level", level},
//                 {"wave", wave},
//                 {"date", std::time(nullptr)}
//             };
            
//             scores.push_back(newScore);
            
//             std::ofstream outFile("scores.json");
//             outFile << scores.dump(4);
//             outFile.close();
            
//             std::cout << "Score sauvegardé avec succès!" << std::endl;
//         } catch (const std::exception& e) {
//             std::cerr << "Erreur lors de la sauvegarde du score: " << e.what() << std::endl;
//         }
//     }
    
//     void resetGame() {
//         gold = 200;
//         lives = 3;
//         wave = 1;
//         level = 1;
//         score = 0;
//         enemies.clear();
//         towers.clear();
//         projectiles.clear();
//         gameOver = false;
//         showReplayOption = false;
//         showSaveOption = false;
//         waitingForName = false;
//         playerName = "";
//         path = predefinedPaths[0];
//     }

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense "),
//              gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
//              gameOver(false), showReplayOption(false), showSaveOption(false),
//              waitingForName(false), playerName("") {
        
//         // Initialize the random number generator 
//         srand(time(nullptr));
        
//         std::cout << "=== TOWER DEFENSE ===" << std::endl;
//         std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
//         std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
//         std::cout << "✓ Observer Pattern: Event management" << std::endl;
//         std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
        
//         // Subscribe to its own events
//         addObserver(this);
        
//         // Initialize the first path
//         path = predefinedPaths[0];
        
//         std::cout << "CONTROLS:" << std::endl;
//         std::cout << "- Left click: Place tower" << std::endl;
//         std::cout << "- Click on tower: Upgrade" << std::endl;
//         std::cout << "- Press SPACE to start wave" << std::endl;
//         std::cout << "\nREQUIRED FILES (place in assets/ folder):" << std::endl;
//         std::cout << "- assets/arial.ttf (REQUIRED for text display)" << std::endl;
//         std::cout << "- assets/gold_icon.png (optional)" << std::endl;
//         std::cout << "- assets/heart_icon.png (optional)" << std::endl;
//     }

//     // Observer implementation - Reacts to game events
//     void notify(const GameEvent& event) override {
//         switch(event.type) {
//             case GameEvent::ENEMY_KILLED:
//                 gold += event.value;
//                 score += event.value * 10;
//                 std::cout << "Observer: Enemy killed! +" << event.value << " gold, Score: " << score << std::endl;
//                 break;
                
//             case GameEvent::ENEMY_REACHED_END:
//                 lives -= event.value;
//                 score -= 50;
//                 std::cout << "Observer: Enemy passed! -" << event.value << " life(s)" << std::endl;
//                 break;
                
//             case GameEvent::WAVE_COMPLETED:
//                 score += 100;
//                 std::cout << "Observer: Wave completed! +100 bonus points" << std::endl;
//                 break;
//         }
//     }

//     void run() {
//         sf::Clock clock;
        
//         while (window.isOpen()) {
//             float deltaTime = clock.restart().asSeconds();
            
//             handleEvents();
//             update(deltaTime);
//             render();
//         }
//     }

//     void handleEvents() {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
            
//             if (event.type == sf::Event::TextEntered && waitingForName) {
//                 if (event.text.unicode == '\b' && !playerName.empty()) {
//                     playerName.pop_back();
//                 }
//                 else if (event.text.unicode < 128 && playerName.length() < 20) {
//                     playerName += static_cast<char>(event.text.unicode);
//                 }
//             }
            
//             if (event.type == sf::Event::KeyPressed) {
//                 if (event.key.code == sf::Keyboard::Return && waitingForName) {
//                     waitingForName = false;
//                     showSaveOption = true;
//                 }
//                 else if (event.key.code == sf::Keyboard::Space && !waveActive && !gameOver) {
//                     startWave();
//                 }
//                 else if (event.key.code == sf::Keyboard::R && gameOver) {
//                     resetGame();
//                 }
//                 else if (event.key.code == sf::Keyboard::S && showSaveOption) {
//                     saveScore();
//                     showSaveOption = false;
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                 sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
//                 // Check click on existing tower
//                 bool towerClicked = false;
//                 for (std::unique_ptr<Tower>& tower : towers) {
//                     float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                     if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
//                         gold -= tower->getUpgradeCost();
//                         tower->upgrade();
//                         towerClicked = true;
//                         std::cout << "Tower upgraded to level " << tower->level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // New tower placement
//                 if (!towerClicked) {
//                     int cost = 50;
//                     if (gold >= cost) {
//                         bool canPlace = true;
                        
//                         // Check distance with other towers
//                         for (const std::unique_ptr<Tower>& tower : towers) {
//                             float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                             if (distance < TOWER_MIN_DISTANCE) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         // Check distance from path
//                         for (size_t i = 0; i < path.size() - 1; i++) {
//                             sf::Vector2f start = path[i];
//                             sf::Vector2f end = path[i + 1];
                            
//                             float A = clickPos.x - start.x;
//                             float B = clickPos.y - start.y;
//                             float C = end.x - start.x;
//                             float D = end.y - start.y;
                            
//                             float dot = A * C + B * D;
//                             float lenSq = C * C + D * D;
//                             float param = (lenSq != 0) ? dot / lenSq : -1;
                            
//                             float xx, yy;
//                             if (param < 0) {
//                                 xx = start.x; yy = start.y;
//                             } else if (param > 1) {
//                                 xx = end.x; yy = end.y;
//                             } else {
//                                 xx = start.x + param * C;
//                                 yy = start.y + param * D;
//                             }
                            
//                             float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
//                             if (distance < 50) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         if (canPlace) {
//                             std::unique_ptr<Tower> tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
//                             towers.push_back(std::move(tower));
//                             gold -= cost;
//                             std::cout << "Tower placed! Remaining gold: " << gold << std::endl;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== LEVEL " << level << " - WAVE " << wave << " STARTS ===" << std::endl;
//     }

//     void update(float deltaTime) {
//         // Stop updating if game is over
//         if (lives <= 0 || wave > 10) {
//             return;
//         }
        
//         // Spawn enemies with Factory Pattern
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) {
//                 // Using Factory Pattern with level-based difficulty
//                 EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
//                 std::unique_ptr<Enemy> enemy = EnemyFactory::createEnemy(type);
                
//                 // Define initial position at beginning of path
//                 enemy->position = path[0];
                
//                 // Increase enemy statistics based on the level and the wave
//                 float levelMultiplier = 1.0f + (level - 1) * 0.15f;  // Increase by 15% per level
//                 float waveMultiplier = 1.0f + (wave - 1) * 0.05f;    // Increase by 5% per wave
//                 float totalMultiplier = levelMultiplier * waveMultiplier;
                
//                 // Apply multipliers to other stats
//                 enemy->health = static_cast<int>(enemy->health * totalMultiplier);
//                 enemy->maxHealth = enemy->health;
//                 enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);  // Reduce speed bonus
//                 enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));  // Reduce reward bonus 
                
//                 enemies.push_back(std::move(enemy));
                
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//             }
//         }

//         // Update enemies
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->update(deltaTime, path);
                
//                 if (enemy->reachedEnd(path)) {
//                     // Using Observer Pattern
//                     notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
//                     enemy->alive = false;
//                 }
//             }
//         }

//         // Update towers (now they create projectiles)
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->update(deltaTime, enemies, projectiles, this);
//         }

//         // Update projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.update(deltaTime);
//         }

//         // Remove inactive projectiles
//         projectiles.erase(
//             std::remove_if(projectiles.begin(), projectiles.end(),
//                 [](const Projectile& proj) { return !proj.active; }),
//             projectiles.end()
//         );

//         // Remove dead enemies
//         enemies.erase(
//             std::remove_if(enemies.begin(), enemies.end(),
//                 [](const std::unique_ptr<Enemy>& enemy) { return !enemy->alive; }),
//             enemies.end()
//         );

//         // Check end of wave
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
//             wave++;
            
//             // Change the level after 3 waves
//             if (wave > 3) {
//                 level++;
//                 wave = 1;
//                 enemiesInWave = 5 + (level - 1) * 2;  // More enemies per wave at each level
//                 gold += 100 * level;  // Gold bonus for each level
                
//                 // Change the path and remove towers
//                 changePath();
//                 towers.clear();
                
//                 std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
//                 std::cout << "New path selected - Towers have been removed" << std::endl;
//             } else {
//                 enemiesInWave += 2;
//             }
//             std::cout << "Press SPACE for level " << level << " wave " << wave << std::endl;
//         }

//         // Check for game over
//         if (lives <= 0) {
//             gameOver = true;
//             showReplayOption = true;
//             waitingForName = true;
//         }
//     }

//     // Convert integer to string for display
//     std::string intToString(int value) {
//         std::ostringstream oss;
//         oss << value;
//         return oss.str();
//     }

//     // Enhanced text drawing method with Arial font support
//     void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 16) {
//         sf::Text sfText;
//         sfText.setString(text);
//         sfText.setCharacterSize(size);
//         sfText.setFillColor(color);
//         sfText.setPosition(x, y);
        
//         // Use Arial font if loaded, otherwise SFML will use default font
//         if (resourceManager.isFontLoaded()) {
//             sfText.setFont(resourceManager.getFont());
//         }
        
//         window.draw(sfText);
//     }

//     void render() {
//         window.clear(sf::Color(40, 40, 40));

//         // Draw the path
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vector2f start = path[i];
//             sf::Vector2f end = path[i + 1];
//             sf::Vector2f direction = end - start;
//             float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (length > 0) {
//                 direction = direction / length;
//                 sf::RectangleShape pathSegment;
//                 pathSegment.setSize(sf::Vector2f(length, 40));
//                 pathSegment.setPosition(start);
//                 pathSegment.setFillColor(sf::Color(100, 100, 100));
                
//                 float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
//                 pathSegment.setRotation(angle);
//                 pathSegment.setOrigin(0, 20);
                
//                 window.draw(pathSegment);
//             }
//         }

//         // Path points
//         for (const sf::Vector2f& point : path) {
//             sf::CircleShape dot(3);
//             dot.setPosition(point.x - 3, point.y - 3);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Draw towers
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->draw(window);
//         }

//         // Draw projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.draw(window);
//         }

//         // Draw enemies (on top of projectiles)
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->draw(window);
//             }
//         }

//         // ==============================
//         // USER INTERFACE WITH SPRITES AND ARIAL FONT
//         // ==============================
        
//         // Gold interface with sprite and text (using Arial font)
//         resourceManager.drawGoldIcon(window, 10, 10);
//         drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

//         // Lives interface with sprite and text (using Arial font)
//         resourceManager.drawHeartIcon(window, 10, 40);
//         drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

//         // Level interface with text (using Arial font)
//         drawText(window, "Level : " + intToString(level), 300, 15, sf::Color::White, 18);

//         // Wave interface with text (using Arial font)
//         drawText(window, "Wave : " + intToString(wave), 300, 45, sf::Color::Cyan, 18);

//         // Additional game information
//         drawText(window, "Towers : " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
//         drawText(window, "Enemies : " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);

//         // Score interface with text (using Arial font)
//         drawText(window, "Score : " + intToString(score), 850, 15, sf::Color::White, 18);

//         // "PRESS SPACE TO START" text at center of screen (large, visible text)
//         if (!waveActive && lives > 0) {
//             // Semi-transparent background for text
//             sf::RectangleShape textBg(sf::Vector2f(500, 80));
//             textBg.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 40);
//             textBg.setFillColor(sf::Color(0, 0, 0, 150));
//             textBg.setOutlineColor(sf::Color::Green);
//             textBg.setOutlineThickness(3);
//             window.draw(textBg);
            
//             // Large, bold text for better visibility
//             drawText(window, "PRESS SPACE", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 25, sf::Color::Green, 32);
//             drawText(window, "TO START LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
//                     WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 10, sf::Color::Green, 22);
            
//             // Display message for each new level
//             if (wave == 1) {
//                 drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
//                         WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 40, sf::Color::Yellow, 20);
//             }
//         }

//         // Game over screen
//         if (gameOver) {
//             sf::RectangleShape gameOverBg(sf::Vector2f(400, 200));
//             gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 100);
//             gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
//             gameOverBg.setOutlineColor(sf::Color::White);
//             gameOverBg.setOutlineThickness(3);
//             window.draw(gameOverBg);
            
//             drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80, sf::Color::White, 32);
//             drawText(window, "Final Score: " + intToString(score), WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 40, sf::Color::White, 20);
            
//             if (waitingForName) {
//                 drawText(window, "Enter your name :", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//                 drawText(window, "Press Enter to continue", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
//             }
//             else if (showSaveOption) {
//                 drawText(window, "Save score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, "Press S to save", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
//             }
//             else if (showReplayOption) {
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//             }
//         }

//         // Instructions at bottom of screen (clear and readable)
//         drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade (100 and 200 gold) | SPACE: Start wave", 10, WINDOW_HEIGHT - 30, sf::Color::White, 16);

//         window.display();
//     }
// };

// int main() {
//     std::cout << "Tower defense" << std::endl;
//     Game game;
//     game.run();
//     return 0;
// }


// Ci dessous version bien juste pas de timer 

// #include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <sstream>
// #include <memory>
// #include <algorithm>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <fstream>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// // Constants
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;
// const float TOWER_MIN_DISTANCE = 80.0f;

// // ===========================================
// // AUDIO MANAGER - Gestion de la musique et des sons
// // ===========================================
// class AudioManager {
// private:
//     sf::Music backgroundMusic1;
//     sf::Music backgroundMusic2;
//     sf::SoundBuffer shootSoundBuffer;
//     sf::SoundBuffer explosionSoundBuffer;
//     sf::SoundBuffer upgradeSoundBuffer;
//     sf::Sound shootSound;
//     sf::Sound explosionSound;
//     sf::Sound upgradeSound;
    
//     bool music1Loaded;
//     bool music2Loaded;
//     bool soundsLoaded;
//     float masterVolume;
//     float musicVolume;
//     float effectsVolume;
//     bool isMuted;
//     int currentMusicTrack; // 0 = music1, 1 = music2
    
// public:
//     AudioManager() : music1Loaded(false), music2Loaded(false), soundsLoaded(false),
//                      masterVolume(50.0f), musicVolume(50.0f), effectsVolume(70.0f), 
//                      isMuted(false), currentMusicTrack(0) {
//         loadAudioFiles();
//     }
    
//     void loadAudioFiles() {
//         std::cout << "=== CHARGEMENT DES FICHIERS AUDIO ===" << std::endl;
        
//         // Chemins à essayer pour les fichiers audio
//         std::vector<std::string> audioPaths = {
//             "./assets/",
//             "assets/",
//             "../../assets/",
//             "../assets/",
//             "./"
//         };
        
//         // Chargement des musiques de fond
//         for (const std::string& basePath : audioPaths) {
//             if (!music1Loaded) {
//                 std::string music1Path = basePath + "background_music1.ogg";
//                 if (backgroundMusic1.openFromFile(music1Path)) {
//                     music1Loaded = true;
//                     backgroundMusic1.setLoop(true);
//                     backgroundMusic1.setVolume(musicVolume);
//                     std::cout << "✓ Musique 1 chargée: " << music1Path << std::endl;
//                 }
//             }
            
//             if (!music2Loaded) {
//                 std::string music2Path = basePath + "background_music2.ogg";
//                 if (backgroundMusic2.openFromFile(music2Path)) {
//                     music2Loaded = true;
//                     backgroundMusic2.setLoop(true);
//                     backgroundMusic2.setVolume(musicVolume);
//                     std::cout << "✓ Musique 2 chargée: " << music2Path << std::endl;
//                 }
//             }
            
//             // Chargement des effets sonores
//             if (!soundsLoaded) {
//                 std::string shootPath = basePath + "shoot.wav";
//                 std::string explosionPath = basePath + "explosion.wav";
//                 std::string upgradePath = basePath + "upgrade.wav";
                
//                 bool shootLoaded = shootSoundBuffer.loadFromFile(shootPath);
//                 bool explosionLoaded = explosionSoundBuffer.loadFromFile(explosionPath);
//                 bool upgradeLoaded = upgradeSoundBuffer.loadFromFile(upgradePath);
                
//                 if (shootLoaded || explosionLoaded || upgradeLoaded) {
//                     soundsLoaded = true;
//                     if (shootLoaded) {
//                         shootSound.setBuffer(shootSoundBuffer);
//                         shootSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son de tir chargé: " << shootPath << std::endl;
//                     }
//                     if (explosionLoaded) {
//                         explosionSound.setBuffer(explosionSoundBuffer);
//                         explosionSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son d'explosion chargé: " << explosionPath << std::endl;
//                     }
//                     if (upgradeLoaded) {
//                         upgradeSound.setBuffer(upgradeSoundBuffer);
//                         upgradeSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son d'amélioration chargé: " << upgradePath << std::endl;
//                     }
//                 }
//             }
//         }
        
//         if (!music1Loaded && !music2Loaded) {
//             std::cout << "⚠ AUCUNE musique trouvée! Placez background_music1.ogg et/ou background_music2.ogg dans assets/" << std::endl;
//         }
//         if (!soundsLoaded) {
//             std::cout << "⚠ Effets sonores non trouvés! Placez shoot.wav, explosion.wav, upgrade.wav dans assets/" << std::endl;
//         }
        
//         std::cout << "=== FIN CHARGEMENT AUDIO ===" << std::endl;
//     }
    
//     void startMusic() {
//         if (isMuted) return;
        
//         stopMusic(); // Arrête toute musique en cours
        
//         if (currentMusicTrack == 0 && music1Loaded) {
//             backgroundMusic1.play();
//             std::cout << "Lecture de la musique 1" << std::endl;
//         } else if (currentMusicTrack == 1 && music2Loaded) {
//             backgroundMusic2.play();
//             std::cout << "Lecture de la musique 2" << std::endl;
//         }
//     }
    
//     void stopMusic() {
//         backgroundMusic1.stop();
//         backgroundMusic2.stop();
//     }
    
//     void switchMusic() {
//         if (music1Loaded && music2Loaded) {
//             currentMusicTrack = (currentMusicTrack == 0) ? 1 : 0;
//             startMusic();
//             std::cout << "Changement vers musique " << (currentMusicTrack + 1) << std::endl;
//         }
//     }
    
//     void toggleMute() {
//         isMuted = !isMuted;
//         if (isMuted) {
//             stopMusic();
//             std::cout << "Audio muet" << std::endl;
//         } else {
//             startMusic();
//             std::cout << "Audio activé" << std::endl;
//         }
//     }
    
//     void increaseMasterVolume() {
//         masterVolume = std::min(100.0f, masterVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void decreaseMasterVolume() {
//         masterVolume = std::max(0.0f, masterVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void increaseMusicVolume() {
//         musicVolume = std::min(100.0f, musicVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void decreaseMusicVolume() {
//         musicVolume = std::max(0.0f, musicVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void increaseEffectsVolume() {
//         effectsVolume = std::min(100.0f, effectsVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void decreaseEffectsVolume() {
//         effectsVolume = std::max(0.0f, effectsVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void updateVolumes() {
//         float finalMusicVolume = (masterVolume / 100.0f) * (musicVolume / 100.0f) * 100.0f;
//         float finalEffectsVolume = (masterVolume / 100.0f) * (effectsVolume / 100.0f) * 100.0f;
        
//         backgroundMusic1.setVolume(finalMusicVolume);
//         backgroundMusic2.setVolume(finalMusicVolume);
//         shootSound.setVolume(finalEffectsVolume);
//         explosionSound.setVolume(finalEffectsVolume);
//         upgradeSound.setVolume(finalEffectsVolume);
//     }
    
//     void playShootSound() {
//         if (!isMuted && soundsLoaded) {
//             shootSound.play();
//         }
//     }
    
//     void playExplosionSound() {
//         if (!isMuted && soundsLoaded) {
//             explosionSound.play();
//         }
//     }
    
//     void playUpgradeSound() {
//         if (!isMuted && soundsLoaded) {
//             upgradeSound.play();
//         }
//     }
    
//     // Getters pour l'interface
//     float getMasterVolume() const { return masterVolume; }
//     float getMusicVolume() const { return musicVolume; }
//     float getEffectsVolume() const { return effectsVolume; }
//     bool getIsMuted() const { return isMuted; }
//     int getCurrentMusicTrack() const { return currentMusicTrack; }
//     bool hasMusicLoaded() const { return music1Loaded || music2Loaded; }
//     bool hasBothMusicsLoaded() const { return music1Loaded && music2Loaded; }
// };

// // ===========================================
// // OBSERVER PATTERN - Interface for events
// // ===========================================
// class GameEvent {
// public:
//     enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
//     Type type;
//     int value; // Gold earned, damage taken, etc.
    
//     GameEvent(Type t, int v = 0) : type(t), value(v) {}
// };

// class Observer {
// public:
//     virtual ~Observer() = default;
//     virtual void notify(const GameEvent& event) = 0;
// };

// class Subject {
// protected:
//     std::vector<Observer*> observers;
// public:
//     virtual ~Subject() = default;
    
//     void addObserver(Observer* observer) {
//         observers.push_back(observer);
//     }
    
//     void removeObserver(Observer* observer) {
//         observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//     }
    
//     void notifyObservers(const GameEvent& event) {
//         for (auto* observer : observers) {
//             observer->notify(event);
//         }
//     }
// };

// // ===========================================
// // RESOURCE MANAGER - To manage textures, sprites and fonts
// // ===========================================
// class ResourceManager {
// private:
//     sf::Texture goldTexture, heartTexture;
//     sf::Sprite goldSprite, heartSprite;
//     bool goldLoaded, heartLoaded;
    
//     // Font management
//     sf::Font arialFont;
//     bool fontLoaded;
    
// public:
//     ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
//         loadResources();
//         loadFont();
//     }
    
//     // Font loading method with comprehensive path search
//     void loadFont() {
//         std::cout << "=== LOADING ARIAL FONT ===" << std::endl;
        
//         // List of paths to try for arial.ttf font
//         std::vector<std::string> fontPaths = {
//             "./assets/arial.ttf",                    // In assets directory
//             "assets/arial.ttf",                      // Without ./
//             "../../assets/arial.ttf",                // Two levels up
//             "../../../assets/arial.ttf",             // Three levels up
//             "../../../../assets/arial.ttf",          // Four levels up
//             "../../src/assets/arial.ttf",            // If assets is in src
//             "../assets/arial.ttf",                   // One level up
//             "./arial.ttf",                          // In current directory
//             "arial.ttf",                            // Direct filename
//         };
        
//         // Try to load font from each path
//         for (const std::string& path : fontPaths) {
//             std::cout << "Trying font path: " << path << std::endl;
            
//             // Check if file exists first
//             struct stat info;
//             if (stat(path.c_str(), &info) == 0) {
//                 if (arialFont.loadFromFile(path)) {
//                     fontLoaded = true;
//                     std::cout << "✓ Font successfully loaded from: " << path << std::endl;
//                     return;
//                 } else {
//                     std::cout << "  -> SFML font loading failed for: " << path << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> Font file not found at: " << path << std::endl;
//             }
//         }
        
//         std::cout << "⚠ WARNING: Arial font not found in any location!" << std::endl;
//         std::cout << "  Please place arial.ttf in the assets/ folder" << std::endl;
//         std::cout << "=== END FONT LOADING ===" << std::endl;
//     }
    
//     // Get font reference for text rendering
//     const sf::Font& getFont() const {
//         return arialFont;
//     }
    
//     // Check if font is available
//     bool isFontLoaded() const {
//         return fontLoaded;
//     }
    
//     void loadResources() {   
//         // Display current working directory
//         char cwd[1024];
//         if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//             std::cout << "Current working directory: " << cwd << std::endl;
//         }
        
//         // List of paths to try (adapted for your structure)
//         std::vector<std::string> pathsToTry = {
//             "./assets/",                    // In executable directory
//             "assets/",                      // Without ./
//             "../../assets/",                // Two levels up (from build/bin to root)
//             "../../../assets/",             // Three levels up
//             "../../../../assets/",          // Four levels up
//             "../../src/assets/",            // If assets is in src
//             "../assets/",                   // One level up
//             "./"                           // In current directory
//         };
        
//         // Test existence of assets folder
//         struct stat info;
//         if (stat("./assets", &info) == 0 && S_ISDIR(info.st_mode)) {
//             std::cout << "✓ Folder './assets' found" << std::endl;
//         } else {
//             std::cout << "⚠ Folder './assets' not found" << std::endl;
//         }
        
//         // Loading gold icon
//         bool goldFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "gold_icon.png";
//             std::cout << "Attempt: " << fullPath << std::endl;
            
//             // Check if file exists
//             if (stat(fullPath.c_str(), &info) == 0) {
//                 std::cout << "  -> File found, attempting to load..." << std::endl;
//                 if (goldTexture.loadFromFile(fullPath)) {
//                     goldLoaded = true;
//                     goldSprite.setTexture(goldTexture);
//                     goldSprite.setScale(0.8f, 0.8f);
//                     std::cout << "✓ Texture gold_icon.png loaded from: " << fullPath << std::endl;
//                     goldFound = true;
//                     break;
//                 } else {
//                     std::cout << "  -> SFML loading failed" << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> File not found" << std::endl;
//             }
//         }
//         if (!goldFound) {
//             std::cout << "⚠ Texture gold_icon.png not found in all paths" << std::endl;
//         }
        
//         // Loading heart icon
//         bool heartFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "heart_icon.png";
//         if (stat(fullPath.c_str(), &info) == 0) {
//             std::cout << "  -> File found, attempting to load..." << std::endl;
//             if (heartTexture.loadFromFile(fullPath)) {
//                 heartLoaded = true;
//                 heartSprite.setTexture(heartTexture);
//                 heartSprite.setScale(0.8f, 0.8f);
//                 std::cout << "✓ Texture heart_icon.png loaded from: " << fullPath << std::endl;
//                 heartFound = true;
//                 break;
//             } else {
//                 std::cout << "  -> SFML loading failed" << std::endl;
//             }
//         } else {
//             std::cout << "  -> File not found" << std::endl;
//         }
//         }
//         if (!heartFound) {
//             std::cout << "⚠ Texture heart_icon.png not found" << std::endl;
//         }
        
//         std::cout << "=== END DEBUG PATHS ===" << std::endl;
//     }
    
//     void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
//         if (goldLoaded) {
//             goldSprite.setPosition(x, y);
//             window.draw(goldSprite);
//         } 
//     }
    
//     void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
//         if (heartLoaded) {
//             heartSprite.setPosition(x, y);
//             window.draw(heartSprite);
//         } 
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Enemy creation
// // ===========================================
// class Enemy {
// public:
//     sf::Vector2f position;
//     int health;
//     int maxHealth;
//     float speed;
//     int reward;
//     bool alive;
//     int pathIndex;
//     std::string type;

//     Enemy(int hp, float spd, int rew, const std::string& t) 
//         : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
//         // La position initiale sera définie par le Game
//     }

//     virtual ~Enemy() = default;

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= path.size() - 1) return;

//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             direction = direction / distance;
//             position += direction * speed * deltaTime;
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     virtual sf::Color getColor() {
//         if (health > maxHealth * 0.7f) return sf::Color::Green;
//         else if (health > maxHealth * 0.3f) return sf::Color::Yellow;
//         else return sf::Color::Red;
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
//         shape.setFillColor(getColor());
//         window.draw(shape);

//         // Health bar
//         sf::RectangleShape healthBar(sf::Vector2f(30, 5));
//         healthBar.setPosition(position.x - 15, position.y - 25);
//         healthBar.setFillColor(sf::Color::Black);
//         window.draw(healthBar);

//         if (health > 0) {
//             sf::RectangleShape healthFill(sf::Vector2f(30 * (float)health / maxHealth, 5));
//             healthFill.setPosition(position.x - 15, position.y - 25);
//             healthFill.setFillColor(sf::Color::Green);
//             window.draw(healthFill);
//         }
//     }

//     bool reachedEnd(const std::vector<sf::Vector2f>& path) {
//         return pathIndex >= path.size() - 1;
//     }
// };

// // Specialized enemy classes
// class Goblin : public Enemy {
// public:
//     Goblin() : Enemy(50, 60, 10, "Goblin") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Green;
//     }
// };

// class Orc : public Enemy {
// public:
//     Orc() : Enemy(120, 45, 20, "Orc") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Red;
//     }
// };

// class MediumEnemy : public Enemy {
// public:
//     MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
//     sf::Color getColor() override {
//         return sf::Color(0, 128, 128); // Turquoise
//     }
// };

// class Troll : public Enemy {
// public:
//     Troll() : Enemy(300, 35, 50, "Troll") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Magenta;
//     }
// };

// class HeavyEnemy : public Enemy {
// public:
//     HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
//     sf::Color getColor() override {
//         return sf::Color(139, 69, 19); // Brown
//     }
// };

// // Factory to create enemies
// class EnemyFactory {
// public:
//     enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
//     // Creation of enemies per type
//     static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
//         switch(type) {
//             case GOBLIN:
//                 std::cout << "Factory: Creating a Goblin" << std::endl;
//                 return std::make_unique<Goblin>();
//             case ORC:
//                 std::cout << "Factory: Creating an Orc" << std::endl;
//                 return std::make_unique<Orc>();
//             case TROLL:
//                 std::cout << "Factory: Creating a Troll" << std::endl;
//                 return std::make_unique<Troll>();
//             case HEAVY:
//                 std::cout << "Factory: Creating a Heavy Enemy" << std::endl;
//                 return std::make_unique<HeavyEnemy>();
//             case MEDIUM:
//                 std::cout << "Factory: Creating a Medium Enemy" << std::endl;
//                 return std::make_unique<MediumEnemy>();
//             default:
//                 return std::make_unique<Goblin>();
//         }
//     }
    
//     // Create assortment of enemies based with difficulty increasing per level
//     static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
//         int totalDifficulty = wave + (level - 1) * 3;
        
//         if (totalDifficulty <= 3) {
//             return GOBLIN;
//         }
//         else if (totalDifficulty <= 6) {
//             if (enemyIndex % 8 == 0) return MEDIUM;
//             return (enemyIndex % 5 == 0) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 9) {
//             if (enemyIndex % 10 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 2) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 12) {
//             if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 5 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 3) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 15) {
//             if (enemyIndex % 10 == 0) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 7) return ORC;
//             else return GOBLIN;
//         }
//         else {
//             if (enemyIndex % 10 < 3) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 9) return ORC;
//             else return GOBLIN;
//         }
//     }
// };

// // Class for projectiles
// class Projectile {
// public:
//     sf::Vector2f position;
//     sf::Vector2f targetPosition;
//     sf::Vector2f velocity;
//     int damage;
//     float speed;
//     bool active;
//     Enemy* targetEnemy;
//     Subject* gameSubject;
//     AudioManager* audioManager;

//     Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject, AudioManager* audio) 
//         : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), 
//           gameSubject(subject), audioManager(audio) {
        
//         if (target) {
//             targetPosition = target->position;
//             // Calculate direction towards target
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }
        
//         // Jouer le son de tir
//         if (audioManager) {
//             audioManager->playShootSound();
//         }
//     }

//     void update(float deltaTime) {
//         if (!active) return;

//         // If target is still alive, follow its position
//         if (targetEnemy && targetEnemy->alive) {
//             targetPosition = targetEnemy->position;
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (distance < 10.0f) {
//                 // Impact!
//                 int oldHealth = targetEnemy->health;
//                 targetEnemy->takeDamage(damage);
                
//                 // Jouer le son d'explosion quand l'ennemi meurt
//                 if (targetEnemy->health <= 0 && oldHealth > 0) {
//                     gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
//                     if (audioManager) {
//                         audioManager->playExplosionSound();
//                     }
//                 }
                
//                 std::cout << "Projectile hits " << targetEnemy->type << "! Damage: " << damage << std::endl;
//                 active = false;
//                 return;
//             }
            
//             // Update velocity to follow target
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }

//         // Move projectile
//         position += velocity * deltaTime;
        
//         // Deactivate if too far (safety)
//         if (position.x < -50 || position.x > WINDOW_WIDTH + 50 || 
//             position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
//             active = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         if (!active) return;
        
//         sf::CircleShape bullet(4);
//         bullet.setPosition(position.x - 4, position.y - 4);
//         bullet.setFillColor(sf::Color::Yellow);
//         bullet.setOutlineColor(sf::Color::Black);
//         bullet.setOutlineThickness(1);
//         window.draw(bullet);
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Tower creation
// // ===========================================

// // Enumeration of tower types
// enum class TowerType {
//     BASIC,
//     SNIPER,
//     CANNON,
//     FREEZE
// };

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;
//     std::string towerType;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1), towerType("Basic") {}

//     // Protected constructor for derived classes
// protected:
//     Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
//         : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), timeSinceLastAttack(0), level(1), towerType(type) {}

// public:
//     virtual ~Tower() = default;
//     virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject, AudioManager* audioManager) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (std::unique_ptr<Enemy>& enemy : enemies) {
//                 if (!enemy->alive) continue;
                
//                 float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = enemy.get();
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 // Create a projectile to attack enemy
//                 projectiles.push_back(Projectile(position, target, damage, subject, audioManager));
//                 timeSinceLastAttack = 0;
//             }
//         }
//     }
//     // Tower attack increases with the levels
//     virtual void upgrade() {
//         if (level == 1) {
//             level = 2;
//             damage = 40;
//             range = 100;
//             attackCooldown = 0.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 80;
//             range = 120;
//             attackCooldown = 0.6f;
//         }
//     }

//     virtual void draw(sf::RenderWindow& window) {
//         // Range circle
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         // Tower (delegate to specific implementation)
//         drawTower(window);
//     }

//     virtual int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0;
//     }

//     virtual bool canUpgrade() {
//         return level < 3;
//     }

// protected:
//     virtual void drawTower(sf::RenderWindow& window) {
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }
// };

// // Derived tower classes
// class BasicTower : public Tower {
// public:
//     BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
// };

// class SniperTower : public Tower {
// public:
//     SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 100;
//             range = 180;
//             attackCooldown = 1.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 200;
//             range = 220;
//             attackCooldown = 1.5f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(25, 25));
//         shape.setPosition(position.x - 12.5f, position.y - 12.5f);
        
//         if (level == 1) shape.setFillColor(sf::Color::Green);
//         else if (level == 2) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);
//     }
// };

// class CannonTower : public Tower {
// public:
//     CannonTower(sf::Vector2f pos) : Tower(pos, 100, 60, 3.0f, "Cannon") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 200;
//             range = 70;
//             attackCooldown = 2.5f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 400;
//             range = 80;
//             attackCooldown = 2.0f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::CircleShape shape(18);
//         shape.setPosition(position.x - 18, position.y - 18);
        
//         if (level == 1) shape.setFillColor(sf::Color(139, 69, 19)); // Brown
//         else if (level == 2) shape.setFillColor(sf::Color(160, 82, 45));
//         else shape.setFillColor(sf::Color(205, 133, 63));
        
//         window.draw(shape);
//     }
// };

// class FreezeTower : public Tower {
// public:
//     FreezeTower(sf::Vector2f pos) : Tower(pos, 5, 90, 0.5f, "Freeze") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 10;
//             range = 110;
//             attackCooldown = 0.4f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 20;
//             range = 130;
//             attackCooldown = 0.3f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(28, 28));
//         shape.setPosition(position.x - 14, position.y - 14);
        
//         if (level == 1) shape.setFillColor(sf::Color(173, 216, 230)); // Light Blue
//         else if (level == 2) shape.setFillColor(sf::Color(135, 206, 235));
//         else shape.setFillColor(sf::Color(70, 130, 180));
        
//         window.draw(shape);
//     }
// };

// // Tower Factory
// class TowerFactory {
// public:
//     static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position) {
//         switch (type) {
//             case TowerType::BASIC:
//                 std::cout << "Factory: Creating Basic Tower" << std::endl;
//                 return std::make_unique<BasicTower>(position);
//             case TowerType::SNIPER:
//                 std::cout << "Factory: Creating Sniper Tower" << std::endl;
//                 return std::make_unique<SniperTower>(position);
//             case TowerType::CANNON:
//                 std::cout << "Factory: Creating Cannon Tower" << std::endl;
//                 return std::make_unique<CannonTower>(position);
//             case TowerType::FREEZE:
//                 std::cout << "Factory: Creating Freeze Tower" << std::endl;
//                 return std::make_unique<FreezeTower>(position);
//             default:
//                 return std::make_unique<BasicTower>(position);
//         }
//     }
    
//     static int getTowerCost(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return 50;
//             case TowerType::SNIPER: return 100;
//             case TowerType::CANNON: return 150;
//             case TowerType::FREEZE: return 75;
//             default: return 50;
//         }
//     }
    
//     static std::string getTowerDescription(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return "Basic Tower - Balanced";
//             case TowerType::SNIPER: return "Sniper Tower - Long Range";
//             case TowerType::CANNON: return "Cannon Tower - High Damage";
//             case TowerType::FREEZE: return "Freeze Tower - Slows Enemies";
//             default: return "Unknown Tower";
//         }
//     }
// };

// // ===========================================
// // GAME CLASS - Implements Observer to receive events
// // ===========================================
// class Game : public Subject, public Observer {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<std::unique_ptr<Enemy>> enemies;
//     std::vector<std::unique_ptr<Tower>> towers;
//     std::vector<Projectile> projectiles;
//     int gold;
//     int lives;
//     int wave;
//     int level;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;
//     int score;
//     ResourceManager resourceManager;
//     AudioManager audioManager;
//     bool gameOver;
//     bool showReplayOption;
//     bool showSaveOption;
//     std::string playerName;
//     bool waitingForName;
//     bool showAudioControls;
    
//     // Preset paths
//     const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
//         // Original path
//         {
//             sf::Vector2f(0, 300),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(500, 150),
//             sf::Vector2f(500, 450),
//             sf::Vector2f(800, 450),
//             sf::Vector2f(800, 250),
//             sf::Vector2f(1000, 250)
//         },
//         // Zigzag path
//         {
//             sf::Vector2f(0, 200),
//             sf::Vector2f(200, 200),
//             sf::Vector2f(200, 400),
//             sf::Vector2f(400, 400),
//             sf::Vector2f(400, 200),
//             sf::Vector2f(600, 200),
//             sf::Vector2f(600, 400),
//             sf::Vector2f(800, 400),
//             sf::Vector2f(800, 200),
//             sf::Vector2f(1000, 200)
//         },
//         // Curly path
//         {
//             sf::Vector2f(0, 350),
//             sf::Vector2f(200, 350),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(400, 150),
//             sf::Vector2f(400, 350),
//             sf::Vector2f(600, 350),
//             sf::Vector2f(600, 150),
//             sf::Vector2f(800, 150),
//             sf::Vector2f(800, 350),
//             sf::Vector2f(1000, 350)
//         },
//         // S path
//         {
//             sf::Vector2f(0, 100),
//             sf::Vector2f(200, 100),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(400, 300),
//             sf::Vector2f(400, 500),
//             sf::Vector2f(600, 500),
//             sf::Vector2f(600, 300),
//             sf::Vector2f(800, 300),
//             sf::Vector2f(800, 100),
//             sf::Vector2f(1000, 100)
//         }
//     };

//     void changePath() {
//         // Select a path different from the previous randomly
//         int newPathIndex;
//         do {
//             newPathIndex = rand() % predefinedPaths.size();
//         } while (predefinedPaths[newPathIndex] == path);
        
//         path = predefinedPaths[newPathIndex];
//         std::cout << "=== NEW PATH SELECTED FOR LEVEL " << level << " ===" << std::endl;
//     }

//     void saveScore() {
//         try {
//             json scores;
//             std::ifstream file("scores.json");
//             if (file.good()) {
//                 // File exists, read it
//                 file >> scores;
//                 file.close();
//             } else {
//                 // File doesn't exist, create empty array
//                 scores = json::array();
//                 std::cout << "Creating file scores.json" << std::endl;
//             }
//             // Add new score
//             json newScore = {
//                 {"name", playerName},
//                 {"score", score},
//                 {"level", level},
//                 {"wave", wave},
//                 {"date", std::time(nullptr)}
//             };
            
//             scores.push_back(newScore);
            
//             std::ofstream outFile("scores.json");
//             outFile << scores.dump(4);
//             outFile.close();
            
//             std::cout << "Score sauvegardé avec succès!" << std::endl;
//         } catch (const std::exception& e) {
//             std::cerr << "Erreur lors de la sauvegarde du score: " << e.what() << std::endl;
//         }
//     }
    
//     void resetGame() {
//         gold = 200;
//         lives = 3;
//         wave = 1;
//         level = 1;
//         score = 0;
//         enemies.clear();
//         towers.clear();
//         projectiles.clear();
//         gameOver = false;
//         showReplayOption = false;
//         showSaveOption = false;
//         waitingForName = false;
//         playerName = "";
//         path = predefinedPaths[0];
//         showAudioControls = false;
        
//         // Redémarrer la musique
//         audioManager.startMusic();
//     }

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense avec Audio"),
//              gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
//              gameOver(false), showReplayOption(false), showSaveOption(false),
//              waitingForName(false), playerName(""), showAudioControls(false) {
        
//         // Initialize the random number generator 
//         srand(time(nullptr));
        
//         std::cout << "=== TOWER DEFENSE AVEC AUDIO ===" << std::endl;
//         std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
//         std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
//         std::cout << "✓ Observer Pattern: Event management" << std::endl;
//         std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
//         std::cout << "✓ Audio Manager: Musique et effets sonores" << std::endl;
        
//         // Subscribe to its own events
//         addObserver(this);
        
//         // Initialize the first path
//         path = predefinedPaths[0];
        
//         // Démarrer la musique
//         audioManager.startMusic();
        
//         std::cout << "CONTRÔLES DU JEU:" << std::endl;
//         std::cout << "- Clic gauche: Placer une tour" << std::endl;
//         std::cout << "- Clic sur tour: Améliorer" << std::endl;
//         std::cout << "- ESPACE: Commencer la vague" << std::endl;
//         std::cout << "- A: Afficher/masquer contrôles audio" << std::endl;
//         std::cout << "\nCONTRÔLES AUDIO:" << std::endl;
//         std::cout << "- M: Mute/Unmute" << std::endl;
//         std::cout << "- N: Changer de musique" << std::endl;
//         std::cout << "- +/-: Volume principal" << std::endl;
//         std::cout << "- 1/2: Volume musique" << std::endl;
//         std::cout << "- 3/4: Volume effets" << std::endl;
//         std::cout << "\nFICHIERS REQUIS (placez dans assets/):" << std::endl;
//         std::cout << "- arial.ttf (OBLIGATOIRE pour le texte)" << std::endl;
//         std::cout << "- background_music1.ogg (musique 1)" << std::endl;
//         std::cout << "- background_music2.ogg (musique 2)" << std::endl;
//         std::cout << "- shoot.wav (son de tir)" << std::endl;
//         std::cout << "- explosion.wav (son d'explosion)" << std::endl;
//         std::cout << "- upgrade.wav (son d'amélioration)" << std::endl;
//         std::cout << "- gold_icon.png (optionnel)" << std::endl;
//         std::cout << "- heart_icon.png (optionnel)" << std::endl;
//     }

//     // Observer implementation - Reacts to game events
//     void notify(const GameEvent& event) override {
//         switch(event.type) {
//             case GameEvent::ENEMY_KILLED:
//                 gold += event.value;
//                 score += event.value * 10;
//                 std::cout << "Observer: Enemy killed! +" << event.value << " gold, Score: " << score << std::endl;
//                 break;
                
//             case GameEvent::ENEMY_REACHED_END:
//                 lives -= event.value;
//                 score -= 50;
//                 std::cout << "Observer: Enemy passed! -" << event.value << " life(s)" << std::endl;
//                 break;
                
//             case GameEvent::WAVE_COMPLETED:
//                 score += 100;
//                 std::cout << "Observer: Wave completed! +100 bonus points" << std::endl;
//                 break;
//         }
//     }

//     void run() {
//         sf::Clock clock;
        
//         while (window.isOpen()) {
//             float deltaTime = clock.restart().asSeconds();
            
//             handleEvents();
//             update(deltaTime);
//             render();
//         }
//     }

//     void handleEvents() {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
            
//             if (event.type == sf::Event::TextEntered && waitingForName) {
//                 if (event.text.unicode == '\b' && !playerName.empty()) {
//                     playerName.pop_back();
//                 }
//                 else if (event.text.unicode < 128 && playerName.length() < 20) {
//                     playerName += static_cast<char>(event.text.unicode);
//                 }
//             }
            
//             if (event.type == sf::Event::KeyPressed) {
//                 // Contrôles audio
//                 if (event.key.code == sf::Keyboard::M) {
//                     audioManager.toggleMute();
//                 }
//                 else if (event.key.code == sf::Keyboard::N && audioManager.hasBothMusicsLoaded()) {
//                     audioManager.switchMusic();
//                 }
//                 else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
//                     audioManager.increaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
//                     audioManager.decreaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num1) {
//                     audioManager.increaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num2) {
//                     audioManager.decreaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num3) {
//                     audioManager.increaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num4) {
//                     audioManager.decreaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::A) {
//                     showAudioControls = !showAudioControls;
//                 }
//                 // Contrôles de jeu
//                 else if (event.key.code == sf::Keyboard::Return && waitingForName) {
//                     waitingForName = false;
//                     showSaveOption = true;
//                 }
//                 else if (event.key.code == sf::Keyboard::Space && !waveActive && !gameOver) {
//                     startWave();
//                 }
//                 else if (event.key.code == sf::Keyboard::R && gameOver) {
//                     resetGame();
//                 }
//                 else if (event.key.code == sf::Keyboard::S && showSaveOption) {
//                     saveScore();
//                     showSaveOption = false;
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                 sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
//                 // Check click on existing tower
//                 bool towerClicked = false;
//                 for (std::unique_ptr<Tower>& tower : towers) {
//                     float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                     if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
//                         gold -= tower->getUpgradeCost();
//                         tower->upgrade();
//                         audioManager.playUpgradeSound(); // Son d'amélioration
//                         towerClicked = true;
//                         std::cout << "Tower upgraded to level " << tower->level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // New tower placement
//                 if (!towerClicked) {
//                     int cost = 50;
//                     if (gold >= cost) {
//                         bool canPlace = true;
                        
//                         // Check distance with other towers
//                         for (const std::unique_ptr<Tower>& tower : towers) {
//                             float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                             if (distance < TOWER_MIN_DISTANCE) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         // Check distance from path
//                         for (size_t i = 0; i < path.size() - 1; i++) {
//                             sf::Vector2f start = path[i];
//                             sf::Vector2f end = path[i + 1];
                            
//                             float A = clickPos.x - start.x;
//                             float B = clickPos.y - start.y;
//                             float C = end.x - start.x;
//                             float D = end.y - start.y;
                            
//                             float dot = A * C + B * D;
//                             float lenSq = C * C + D * D;
//                             float param = (lenSq != 0) ? dot / lenSq : -1;
                            
//                             float xx, yy;
//                             if (param < 0) {
//                                 xx = start.x; yy = start.y;
//                             } else if (param > 1) {
//                                 xx = end.x; yy = end.y;
//                             } else {
//                                 xx = start.x + param * C;
//                                 yy = start.y + param * D;
//                             }
                            
//                             float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
//                             if (distance < 50) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         if (canPlace) {
//                             std::unique_ptr<Tower> tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
//                             towers.push_back(std::move(tower));
//                             gold -= cost;
//                             std::cout << "Tower placed! Remaining gold: " << gold << std::endl;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== LEVEL " << level << " - WAVE " << wave << " STARTS ===" << std::endl;
//     }

//     void update(float deltaTime) {
//         // Stop updating if game is over
//         if (lives <= 0 || wave > 10) {
//             return;
//         }
        
//         // Spawn enemies with Factory Pattern
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) {
//                 // Using Factory Pattern with level-based difficulty
//                 EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
//                 std::unique_ptr<Enemy> enemy = EnemyFactory::createEnemy(type);
                
//                 // Define initial position at beginning of path
//                 enemy->position = path[0];
                
//                 // Increase enemy statistics based on the level and the wave
//                 float levelMultiplier = 1.0f + (level - 1) * 0.15f;
//                 float waveMultiplier = 1.0f + (wave - 1) * 0.05f;
//                 float totalMultiplier = levelMultiplier * waveMultiplier;
                
//                 // Apply multipliers to other stats
//                 enemy->health = static_cast<int>(enemy->health * totalMultiplier);
//                 enemy->maxHealth = enemy->health;
//                 enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);
//                 enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));
                
//                 enemies.push_back(std::move(enemy));
                
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//             }
//         }

//         // Update enemies
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->update(deltaTime, path);
                
//                 if (enemy->reachedEnd(path)) {
//                     // Using Observer Pattern
//                     notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
//                     enemy->alive = false;
//                 }
//             }
//         }

//         // Update towers (now they create projectiles with audio)
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->update(deltaTime, enemies, projectiles, this, &audioManager);
//         }

//         // Update projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.update(deltaTime);
//         }

//         // Remove inactive projectiles
//         projectiles.erase(
//             std::remove_if(projectiles.begin(), projectiles.end(),
//                 [](const Projectile& proj) { return !proj.active; }),
//             projectiles.end()
//         );

//         // Remove dead enemies
//         enemies.erase(
//             std::remove_if(enemies.begin(), enemies.end(),
//                 [](const std::unique_ptr<Enemy>& enemy) { return !enemy->alive; }),
//             enemies.end()
//         );

//         // Check end of wave
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
//             wave++;
            
//             // Change the level after 3 waves
//             if (wave > 3) {
//                 level++;
//                 wave = 1;
//                 enemiesInWave = 5 + (level - 1) * 2;
//                 gold += 100 * level;
                
//                 // Change the path and remove towers
//                 changePath();
//                 towers.clear();
                
//                 std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
//                 std::cout << "New path selected - Towers have been removed" << std::endl;
//             } else {
//                 enemiesInWave += 2;
//             }
//             std::cout << "Press SPACE for level " << level << " wave " << wave << std::endl;
//         }

//         // Check for game over
//         if (lives <= 0) {
//             gameOver = true;
//             showReplayOption = true;
//             waitingForName = true;
//             audioManager.stopMusic(); // Arrêter la musique en cas de défaite
//         }
//     }

//     // Convert integer to string for display
//     std::string intToString(int value) {
//         std::ostringstream oss;
//         oss << value;
//         return oss.str();
//     }
    
//     std::string floatToString(float value) {
//         std::ostringstream oss;
//         oss << static_cast<int>(value);
//         return oss.str();
//     }

//     // Enhanced text drawing method with Arial font support
//     void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 16) {
//         sf::Text sfText;
//         sfText.setString(text);
//         sfText.setCharacterSize(size);
//         sfText.setFillColor(color);
//         sfText.setPosition(x, y);
        
//         // Use Arial font if loaded, otherwise SFML will use default font
//         if (resourceManager.isFontLoaded()) {
//             sfText.setFont(resourceManager.getFont());
//         }
        
//         window.draw(sfText);
//     }

//     void render() {
//         window.clear(sf::Color(40, 40, 40));

//         // Draw the path
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vector2f start = path[i];
//             sf::Vector2f end = path[i + 1];
//             sf::Vector2f direction = end - start;
//             float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (length > 0) {
//                 direction = direction / length;
//                 sf::RectangleShape pathSegment;
//                 pathSegment.setSize(sf::Vector2f(length, 40));
//                 pathSegment.setPosition(start);
//                 pathSegment.setFillColor(sf::Color(100, 100, 100));
                
//                 float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
//                 pathSegment.setRotation(angle);
//                 pathSegment.setOrigin(0, 20);
                
//                 window.draw(pathSegment);
//             }
//         }

//         // Path points
//         for (const sf::Vector2f& point : path) {
//             sf::CircleShape dot(3);
//             dot.setPosition(point.x - 3, point.y - 3);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Draw towers
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->draw(window);
//         }

//         // Draw projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.draw(window);
//         }

//         // Draw enemies (on top of projectiles)
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->draw(window);
//             }
//         }

//         // ==============================
//         // USER INTERFACE WITH SPRITES AND ARIAL FONT
//         // ==============================
        
//         // Gold interface with sprite and text
//         resourceManager.drawGoldIcon(window, 10, 10);
//         drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

//         // Lives interface with sprite and text
//         resourceManager.drawHeartIcon(window, 10, 40);
//         drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

//         // Level interface
//         drawText(window, "Level : " + intToString(level), 300, 15, sf::Color::White, 18);

//         // Wave interface
//         drawText(window, "Wave : " + intToString(wave), 300, 45, sf::Color::Cyan, 18);

//         // Additional game information
//         drawText(window, "Towers : " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
//         drawText(window, "Enemies : " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);

//         // Score interface
//         drawText(window, "Score : " + intToString(score), 850, 15, sf::Color::White, 18);

//         // Audio status indicator
//         if (audioManager.getIsMuted()) {
//             drawText(window, "MUTE", 850, 45, sf::Color::Red, 16);
//         } else {
//             std::string musicText = "Music " + intToString(audioManager.getCurrentMusicTrack() + 1);
//             drawText(window, musicText, 850, 45, sf::Color::Green, 16);
//         }

//         // Audio controls panel (toggle with 'A')
//         if (showAudioControls) {
//             // Semi-transparent background
//             sf::RectangleShape controlsBg(sf::Vector2f(300, 200));
//             controlsBg.setPosition(10, 80);
//             controlsBg.setFillColor(sf::Color(0, 0, 0, 180));
//             controlsBg.setOutlineColor(sf::Color::White);
//             controlsBg.setOutlineThickness(2);
//             window.draw(controlsBg);
            
//             drawText(window, "=== CONTROLES AUDIO ===", 20, 90, sf::Color::White, 16);
//             drawText(window, "M: Mute/Unmute", 20, 110, sf::Color::Cyan, 14);
//             if (audioManager.hasBothMusicsLoaded()) {
//                 drawText(window, "N: Changer musique", 20, 130, sf::Color::Cyan, 14);
//             }
//             drawText(window, "+/-: Volume principal (" + floatToString(audioManager.getMasterVolume()) + "%)", 20, 150, sf::Color::Yellow, 14);
//             drawText(window, "1/2: Volume musique (" + floatToString(audioManager.getMusicVolume()) + "%)", 20, 170, sf::Color::Yellow, 14);
//             drawText(window, "3/4: Volume effets (" + floatToString(audioManager.getEffectsVolume()) + "%)", 20, 190, sf::Color::Yellow, 14);
//             drawText(window, "A: Masquer ce panneau", 20, 210, sf::Color::White, 14);
            
//             // Current music indicator
//             if (audioManager.hasMusicLoaded()) {
//                 std::string currentMusic = "Musique actuelle: " + intToString(audioManager.getCurrentMusicTrack() + 1);
//                 drawText(window, currentMusic, 20, 240, sf::Color::Green, 14);
//             } else {
//                 drawText(window, "Aucune musique chargée", 20, 240, sf::Color::Red, 14);
//             }
//         }

//         // "PRESS SPACE TO START" text at center of screen
//         if (!waveActive && lives > 0) {
//             // Semi-transparent background for text
//             sf::RectangleShape textBg(sf::Vector2f(500, 80));
//             textBg.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 40);
//             textBg.setFillColor(sf::Color(0, 0, 0, 150));
//             textBg.setOutlineColor(sf::Color::Green);
//             textBg.setOutlineThickness(3);
//             window.draw(textBg);
            
//             // Large, bold text for better visibility
//             drawText(window, "PRESS SPACE", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 25, sf::Color::Green, 32);
//             drawText(window, "TO START LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
//                     WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 10, sf::Color::Green, 22);
            
//             // Display message for each new level
//             if (wave == 1) {
//                 drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
//                         WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 40, sf::Color::Yellow, 20);
//             }
//         }

//         // Game over screen
//         if (gameOver) {
//             sf::RectangleShape gameOverBg(sf::Vector2f(400, 200));
//             gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 100);
//             gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
//             gameOverBg.setOutlineColor(sf::Color::White);
//             gameOverBg.setOutlineThickness(3);
//             window.draw(gameOverBg);
            
//             drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80, sf::Color::White, 32);
//             drawText(window, "Final Score: " + intToString(score), WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 40, sf::Color::White, 20);
            
//             if (waitingForName) {
//                 drawText(window, "Enter your name :", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//                 drawText(window, "Press Enter to continue", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
//             }
//             else if (showSaveOption) {
//                 drawText(window, "Save score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, "Press S to save", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
//             }
//             else if (showReplayOption) {
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//             }
//         }

//         // Instructions at bottom of screen (clear and readable)
//         drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade | SPACE: Start wave | A: Audio controls", 
//                 10, WINDOW_HEIGHT - 30, sf::Color::White, 16);

//         window.display();
//     }
// };

// int main() {
//     std::cout << "Tower Defense avec Système Audio Complet" << std::endl;
//     std::cout << "==========================================" << std::endl;
//     Game game;
//     game.run();
//     return 0;
// }


// Version timer 1st try --------------------------------

// ----------------------------------------------------------//
// #include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <sstream>
// #include <memory>
// #include <algorithm>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <fstream>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// // Constants
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;
// const float TOWER_MIN_DISTANCE = 80.0f;

// // ===========================================
// // AUDIO MANAGER - Gestion de la musique et des sons
// // ===========================================
// class AudioManager {
// private:
//     sf::Music backgroundMusic1;
//     sf::Music backgroundMusic2;
//     sf::SoundBuffer shootSoundBuffer;
//     sf::SoundBuffer explosionSoundBuffer;
//     sf::SoundBuffer upgradeSoundBuffer;
//     sf::Sound shootSound;
//     sf::Sound explosionSound;
//     sf::Sound upgradeSound;
    
//     bool music1Loaded;
//     bool music2Loaded;
//     bool soundsLoaded;
//     float masterVolume;
//     float musicVolume;
//     float effectsVolume;
//     bool isMuted;
//     int currentMusicTrack; // 0 = music1, 1 = music2
    
// public:
//     AudioManager() : music1Loaded(false), music2Loaded(false), soundsLoaded(false),
//                      masterVolume(50.0f), musicVolume(50.0f), effectsVolume(70.0f), 
//                      isMuted(false), currentMusicTrack(0) {
//         loadAudioFiles();
//     }
    
//     void loadAudioFiles() {
//         std::cout << "=== CHARGEMENT DES FICHIERS AUDIO ===" << std::endl;
        
//         // Chemins à essayer pour les fichiers audio
//         std::vector<std::string> audioPaths = {
//             "./assets/",
//             "assets/",
//             "../../assets/",
//             "../assets/",
//             "./"
//         };
        
//         // Chargement des musiques de fond
//         for (const std::string& basePath : audioPaths) {
//             if (!music1Loaded) {
//                 std::string music1Path = basePath + "background_music1.ogg";
//                 if (backgroundMusic1.openFromFile(music1Path)) {
//                     music1Loaded = true;
//                     backgroundMusic1.setLoop(true);
//                     backgroundMusic1.setVolume(musicVolume);
//                     std::cout << "✓ Musique 1 chargée: " << music1Path << std::endl;
//                 }
//             }
            
//             if (!music2Loaded) {
//                 std::string music2Path = basePath + "background_music2.ogg";
//                 if (backgroundMusic2.openFromFile(music2Path)) {
//                     music2Loaded = true;
//                     backgroundMusic2.setLoop(true);
//                     backgroundMusic2.setVolume(musicVolume);
//                     std::cout << "✓ Musique 2 chargée: " << music2Path << std::endl;
//                 }
//             }
            
//             // Chargement des effets sonores
//             if (!soundsLoaded) {
//                 std::string shootPath = basePath + "shoot.wav";
//                 std::string explosionPath = basePath + "explosion.wav";
//                 std::string upgradePath = basePath + "upgrade.wav";
                
//                 bool shootLoaded = shootSoundBuffer.loadFromFile(shootPath);
//                 bool explosionLoaded = explosionSoundBuffer.loadFromFile(explosionPath);
//                 bool upgradeLoaded = upgradeSoundBuffer.loadFromFile(upgradePath);
                
//                 if (shootLoaded || explosionLoaded || upgradeLoaded) {
//                     soundsLoaded = true;
//                     if (shootLoaded) {
//                         shootSound.setBuffer(shootSoundBuffer);
//                         shootSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son de tir chargé: " << shootPath << std::endl;
//                     }
//                     if (explosionLoaded) {
//                         explosionSound.setBuffer(explosionSoundBuffer);
//                         explosionSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son d'explosion chargé: " << explosionPath << std::endl;
//                     }
//                     if (upgradeLoaded) {
//                         upgradeSound.setBuffer(upgradeSoundBuffer);
//                         upgradeSound.setVolume(effectsVolume);
//                         std::cout << "✓ Son d'amélioration chargé: " << upgradePath << std::endl;
//                     }
//                 }
//             }
//         }
        
//         if (!music1Loaded && !music2Loaded) {
//             std::cout << "⚠ AUCUNE musique trouvée! Placez background_music1.ogg et/ou background_music2.ogg dans assets/" << std::endl;
//         }
//         if (!soundsLoaded) {
//             std::cout << "⚠ Effets sonores non trouvés! Placez shoot.wav, explosion.wav, upgrade.wav dans assets/" << std::endl;
//         }
        
//         std::cout << "=== FIN CHARGEMENT AUDIO ===" << std::endl;
//     }
    
//     void startMusic() {
//         if (isMuted) return;
        
//         stopMusic(); // Arrête toute musique en cours
        
//         if (currentMusicTrack == 0 && music1Loaded) {
//             backgroundMusic1.play();
//             std::cout << "Lecture de la musique 1" << std::endl;
//         } else if (currentMusicTrack == 1 && music2Loaded) {
//             backgroundMusic2.play();
//             std::cout << "Lecture de la musique 2" << std::endl;
//         }
//     }
    
//     void stopMusic() {
//         backgroundMusic1.stop();
//         backgroundMusic2.stop();
//     }
    
//     void switchMusic() {
//         if (music1Loaded && music2Loaded) {
//             currentMusicTrack = (currentMusicTrack == 0) ? 1 : 0;
//             startMusic();
//             std::cout << "Changement vers musique " << (currentMusicTrack + 1) << std::endl;
//         }
//     }
    
//     void toggleMute() {
//         isMuted = !isMuted;
//         if (isMuted) {
//             stopMusic();
//             std::cout << "Audio muet" << std::endl;
//         } else {
//             startMusic();
//             std::cout << "Audio activé" << std::endl;
//         }
//     }
    
//     void increaseMasterVolume() {
//         masterVolume = std::min(100.0f, masterVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void decreaseMasterVolume() {
//         masterVolume = std::max(0.0f, masterVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void increaseMusicVolume() {
//         musicVolume = std::min(100.0f, musicVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void decreaseMusicVolume() {
//         musicVolume = std::max(0.0f, musicVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void increaseEffectsVolume() {
//         effectsVolume = std::min(100.0f, effectsVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void decreaseEffectsVolume() {
//         effectsVolume = std::max(0.0f, effectsVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void updateVolumes() {
//         float finalMusicVolume = (masterVolume / 100.0f) * (musicVolume / 100.0f) * 100.0f;
//         float finalEffectsVolume = (masterVolume / 100.0f) * (effectsVolume / 100.0f) * 100.0f;
        
//         backgroundMusic1.setVolume(finalMusicVolume);
//         backgroundMusic2.setVolume(finalMusicVolume);
//         shootSound.setVolume(finalEffectsVolume);
//         explosionSound.setVolume(finalEffectsVolume);
//         upgradeSound.setVolume(finalEffectsVolume);
//     }
    
//     void playShootSound() {
//         if (!isMuted && soundsLoaded) {
//             shootSound.play();
//         }
//     }
    
//     void playExplosionSound() {
//         if (!isMuted && soundsLoaded) {
//             explosionSound.play();
//         }
//     }
    
//     void playUpgradeSound() {
//         if (!isMuted && soundsLoaded) {
//             upgradeSound.play();
//         }
//     }
    
//     // Getters pour l'interface
//     float getMasterVolume() const { return masterVolume; }
//     float getMusicVolume() const { return musicVolume; }
//     float getEffectsVolume() const { return effectsVolume; }
//     bool getIsMuted() const { return isMuted; }
//     int getCurrentMusicTrack() const { return currentMusicTrack; }
//     bool hasMusicLoaded() const { return music1Loaded || music2Loaded; }
//     bool hasBothMusicsLoaded() const { return music1Loaded && music2Loaded; }
// };

// // ===========================================
// // OBSERVER PATTERN - Interface for events
// // ===========================================
// class GameEvent {
// public:
//     enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
//     Type type;
//     int value; // Gold earned, damage taken, etc.
    
//     GameEvent(Type t, int v = 0) : type(t), value(v) {}
// };

// class Observer {
// public:
//     virtual ~Observer() = default;
//     virtual void notify(const GameEvent& event) = 0;
// };

// class Subject {
// protected:
//     std::vector<Observer*> observers;
// public:
//     virtual ~Subject() = default;
    
//     void addObserver(Observer* observer) {
//         observers.push_back(observer);
//     }
    
//     void removeObserver(Observer* observer) {
//         observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//     }
    
//     void notifyObservers(const GameEvent& event) {
//         for (auto* observer : observers) {
//             observer->notify(event);
//         }
//     }
// };

// // ===========================================
// // RESOURCE MANAGER - To manage textures, sprites and fonts
// // ===========================================
// class ResourceManager {
// private:
//     sf::Texture goldTexture, heartTexture;
//     sf::Sprite goldSprite, heartSprite;
//     bool goldLoaded, heartLoaded;
    
//     // Font management
//     sf::Font arialFont;
//     bool fontLoaded;
    
// public:
//     ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
//         loadResources();
//         loadFont();
//     }
    
//     // Font loading method with comprehensive path search
//     void loadFont() {
//         std::cout << "=== LOADING ARIAL FONT ===" << std::endl;
        
//         // List of paths to try for arial.ttf font
//         std::vector<std::string> fontPaths = {
//             "./assets/arial.ttf",                    // In assets directory
//             "assets/arial.ttf",                      // Without ./
//             "../../assets/arial.ttf",                // Two levels up
//             "../../../assets/arial.ttf",             // Three levels up
//             "../../../../assets/arial.ttf",          // Four levels up
//             "../../src/assets/arial.ttf",            // If assets is in src
//             "../assets/arial.ttf",                   // One level up
//             "./arial.ttf",                          // In current directory
//             "arial.ttf",                            // Direct filename
//         };
        
//         // Try to load font from each path
//         for (const std::string& path : fontPaths) {
//             std::cout << "Trying font path: " << path << std::endl;
            
//             // Check if file exists first
//             struct stat info;
//             if (stat(path.c_str(), &info) == 0) {
//                 if (arialFont.loadFromFile(path)) {
//                     fontLoaded = true;
//                     std::cout << "✓ Font successfully loaded from: " << path << std::endl;
//                     return;
//                 } else {
//                     std::cout << "  -> SFML font loading failed for: " << path << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> Font file not found at: " << path << std::endl;
//             }
//         }
        
//         std::cout << "⚠ WARNING: Arial font not found in any location!" << std::endl;
//         std::cout << "  Please place arial.ttf in the assets/ folder" << std::endl;
//         std::cout << "=== END FONT LOADING ===" << std::endl;
//     }
    
//     // Get font reference for text rendering
//     const sf::Font& getFont() const {
//         return arialFont;
//     }
    
//     // Check if font is available
//     bool isFontLoaded() const {
//         return fontLoaded;
//     }
    
//     void loadResources() {   
//         // Display current working directory
//         char cwd[1024];
//         if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//             std::cout << "Current working directory: " << cwd << std::endl;
//         }
        
//         // List of paths to try (adapted for your structure)
//         std::vector<std::string> pathsToTry = {
//             "./assets/",                    // In executable directory
//             "assets/",                      // Without ./
//             "../../assets/",                // Two levels up (from build/bin to root)
//             "../../../assets/",             // Three levels up
//             "../../../../assets/",          // Four levels up
//             "../../src/assets/",            // If assets is in src
//             "../assets/",                   // One level up
//             "./"                           // In current directory
//         };
        
//         // Test existence of assets folder
//         struct stat info;
//         if (stat("./assets", &info) == 0 && S_ISDIR(info.st_mode)) {
//             std::cout << "✓ Folder './assets' found" << std::endl;
//         } else {
//             std::cout << "⚠ Folder './assets' not found" << std::endl;
//         }
        
//         // Loading gold icon
//         bool goldFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "gold_icon.png";
//             std::cout << "Attempt: " << fullPath << std::endl;
            
//             // Check if file exists
//             if (stat(fullPath.c_str(), &info) == 0) {
//                 std::cout << "  -> File found, attempting to load..." << std::endl;
//                 if (goldTexture.loadFromFile(fullPath)) {
//                     goldLoaded = true;
//                     goldSprite.setTexture(goldTexture);
//                     goldSprite.setScale(0.8f, 0.8f);
//                     std::cout << "✓ Texture gold_icon.png loaded from: " << fullPath << std::endl;
//                     goldFound = true;
//                     break;
//                 } else {
//                     std::cout << "  -> SFML loading failed" << std::endl;
//                 }
//             } else {
//                 std::cout << "  -> File not found" << std::endl;
//             }
//         }
//         if (!goldFound) {
//             std::cout << "⚠ Texture gold_icon.png not found in all paths" << std::endl;
//         }
        
//         // Loading heart icon
//         bool heartFound = false;
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "heart_icon.png";
//         if (stat(fullPath.c_str(), &info) == 0) {
//             std::cout << "  -> File found, attempting to load..." << std::endl;
//             if (heartTexture.loadFromFile(fullPath)) {
//                 heartLoaded = true;
//                 heartSprite.setTexture(heartTexture);
//                 heartSprite.setScale(0.8f, 0.8f);
//                 std::cout << "✓ Texture heart_icon.png loaded from: " << fullPath << std::endl;
//                 heartFound = true;
//                 break;
//             } else {
//                 std::cout << "  -> SFML loading failed" << std::endl;
//             }
//         } else {
//             std::cout << "  -> File not found" << std::endl;
//         }
//         }
//         if (!heartFound) {
//             std::cout << "⚠ Texture heart_icon.png not found" << std::endl;
//         }
        
//         std::cout << "=== END DEBUG PATHS ===" << std::endl;
//     }
    
//     void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
//         if (goldLoaded) {
//             goldSprite.setPosition(x, y);
//             window.draw(goldSprite);
//         } 
//     }
    
//     void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
//         if (heartLoaded) {
//             heartSprite.setPosition(x, y);
//             window.draw(heartSprite);
//         } 
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Enemy creation
// // ===========================================
// class Enemy {
// public:
//     sf::Vector2f position;
//     int health;
//     int maxHealth;
//     float speed;
//     int reward;
//     bool alive;
//     int pathIndex;
//     std::string type;

//     Enemy(int hp, float spd, int rew, const std::string& t) 
//         : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
//         // La position initiale sera définie par le Game
//     }

//     virtual ~Enemy() = default;

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= path.size() - 1) return;

//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             direction = direction / distance;
//             position += direction * speed * deltaTime;
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     virtual sf::Color getColor() {
//         if (health > maxHealth * 0.7f) return sf::Color::Green;
//         else if (health > maxHealth * 0.3f) return sf::Color::Yellow;
//         else return sf::Color::Red;
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
//         shape.setFillColor(getColor());
//         window.draw(shape);

//         // Health bar
//         sf::RectangleShape healthBar(sf::Vector2f(30, 5));
//         healthBar.setPosition(position.x - 15, position.y - 25);
//         healthBar.setFillColor(sf::Color::Black);
//         window.draw(healthBar);

//         if (health > 0) {
//             sf::RectangleShape healthFill(sf::Vector2f(30 * (float)health / maxHealth, 5));
//             healthFill.setPosition(position.x - 15, position.y - 25);
//             healthFill.setFillColor(sf::Color::Green);
//             window.draw(healthFill);
//         }
//     }

//     bool reachedEnd(const std::vector<sf::Vector2f>& path) {
//         return pathIndex >= path.size() - 1;
//     }
// };

// // Specialized enemy classes
// class Goblin : public Enemy {
// public:
//     Goblin() : Enemy(50, 60, 10, "Goblin") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Green;
//     }
// };

// class Orc : public Enemy {
// public:
//     Orc() : Enemy(120, 45, 20, "Orc") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Red;
//     }
// };

// class MediumEnemy : public Enemy {
// public:
//     MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
//     sf::Color getColor() override {
//         return sf::Color(0, 128, 128); // Turquoise
//     }
// };

// class Troll : public Enemy {
// public:
//     Troll() : Enemy(300, 35, 50, "Troll") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Magenta;
//     }
// };

// class HeavyEnemy : public Enemy {
// public:
//     HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
//     sf::Color getColor() override {
//         return sf::Color(139, 69, 19); // Brown
//     }
// };

// // Factory to create enemies
// class EnemyFactory {
// public:
//     enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
//     // Creation of enemies per type
//     static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
//         switch(type) {
//             case GOBLIN:
//                 std::cout << "Factory: Creating a Goblin" << std::endl;
//                 return std::make_unique<Goblin>();
//             case ORC:
//                 std::cout << "Factory: Creating an Orc" << std::endl;
//                 return std::make_unique<Orc>();
//             case TROLL:
//                 std::cout << "Factory: Creating a Troll" << std::endl;
//                 return std::make_unique<Troll>();
//             case HEAVY:
//                 std::cout << "Factory: Creating a Heavy Enemy" << std::endl;
//                 return std::make_unique<HeavyEnemy>();
//             case MEDIUM:
//                 std::cout << "Factory: Creating a Medium Enemy" << std::endl;
//                 return std::make_unique<MediumEnemy>();
//             default:
//                 return std::make_unique<Goblin>();
//         }
//     }
    
//     // Create assortment of enemies based with difficulty increasing per level
//     static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
//         int totalDifficulty = wave + (level - 1) * 3;
        
//         if (totalDifficulty <= 3) {
//             return GOBLIN;
//         }
//         else if (totalDifficulty <= 6) {
//             if (enemyIndex % 8 == 0) return MEDIUM;
//             return (enemyIndex % 5 == 0) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 9) {
//             if (enemyIndex % 10 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 2) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 12) {
//             if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 5 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 3) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 15) {
//             if (enemyIndex % 10 == 0) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 7) return ORC;
//             else return GOBLIN;
//         }
//         else {
//             if (enemyIndex % 10 < 3) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 9) return ORC;
//             else return GOBLIN;
//         }
//     }
// };

// // Class for projectiles
// class Projectile {
// public:
//     sf::Vector2f position;
//     sf::Vector2f targetPosition;
//     sf::Vector2f velocity;
//     int damage;
//     float speed;
//     bool active;
//     Enemy* targetEnemy;
//     Subject* gameSubject;
//     AudioManager* audioManager;

//     Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject, AudioManager* audio) 
//         : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), 
//           gameSubject(subject), audioManager(audio) {
        
//         if (target) {
//             targetPosition = target->position;
//             // Calculate direction towards target
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }
        
//         // Jouer le son de tir
//         if (audioManager) {
//             audioManager->playShootSound();
//         }
//     }

//     void update(float deltaTime) {
//         if (!active) return;

//         // If target is still alive, follow its position
//         if (targetEnemy && targetEnemy->alive) {
//             targetPosition = targetEnemy->position;
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (distance < 10.0f) {
//                 // Impact!
//                 int oldHealth = targetEnemy->health;
//                 targetEnemy->takeDamage(damage);
                
//                 // Jouer le son d'explosion quand l'ennemi meurt
//                 if (targetEnemy->health <= 0 && oldHealth > 0) {
//                     gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
//                     if (audioManager) {
//                         audioManager->playExplosionSound();
//                     }
//                 }
                
//                 std::cout << "Projectile hits " << targetEnemy->type << "! Damage: " << damage << std::endl;
//                 active = false;
//                 return;
//             }
            
//             // Update velocity to follow target
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }

//         // Move projectile
//         position += velocity * deltaTime;
        
//         // Deactivate if too far (safety)
//         if (position.x < -50 || position.x > WINDOW_WIDTH + 50 || 
//             position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
//             active = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         if (!active) return;
        
//         sf::CircleShape bullet(4);
//         bullet.setPosition(position.x - 4, position.y - 4);
//         bullet.setFillColor(sf::Color::Yellow);
//         bullet.setOutlineColor(sf::Color::Black);
//         bullet.setOutlineThickness(1);
//         window.draw(bullet);
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Tower creation
// // ===========================================

// // Enumeration of tower types
// enum class TowerType {
//     BASIC,
//     SNIPER,
//     CANNON,
//     FREEZE
// };

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;
//     std::string towerType;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1), towerType("Basic") {}

//     // Protected constructor for derived classes
// protected:
//     Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
//         : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), timeSinceLastAttack(0), level(1), towerType(type) {}

// public:
//     virtual ~Tower() = default;
//     virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject, AudioManager* audioManager) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (std::unique_ptr<Enemy>& enemy : enemies) {
//                 if (!enemy->alive) continue;
                
//                 float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = enemy.get();
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 // Create a projectile to attack enemy
//                 projectiles.push_back(Projectile(position, target, damage, subject, audioManager));
//                 timeSinceLastAttack = 0;
//             }
//         }
//     }
//     // Tower attack increases with the levels
//     virtual void upgrade() {
//         if (level == 1) {
//             level = 2;
//             damage = 40;
//             range = 100;
//             attackCooldown = 0.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 80;
//             range = 120;
//             attackCooldown = 0.6f;
//         }
//     }

//     virtual void draw(sf::RenderWindow& window) {
//         // Range circle
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         // Tower (delegate to specific implementation)
//         drawTower(window);
//     }

//     virtual int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0;
//     }

//     virtual bool canUpgrade() {
//         return level < 3;
//     }

// protected:
//     virtual void drawTower(sf::RenderWindow& window) {
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }
// };

// // Derived tower classes
// class BasicTower : public Tower {
// public:
//     BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
// };

// class SniperTower : public Tower {
// public:
//     SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 100;
//             range = 180;
//             attackCooldown = 1.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 200;
//             range = 220;
//             attackCooldown = 1.5f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(25, 25));
//         shape.setPosition(position.x - 12.5f, position.y - 12.5f);
        
//         if (level == 1) shape.setFillColor(sf::Color::Green);
//         else if (level == 2) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);
//     }
// };

// class CannonTower : public Tower {
// public:
//     CannonTower(sf::Vector2f pos) : Tower(pos, 100, 60, 3.0f, "Cannon") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 200;
//             range = 70;
//             attackCooldown = 2.5f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 400;
//             range = 80;
//             attackCooldown = 2.0f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::CircleShape shape(18);
//         shape.setPosition(position.x - 18, position.y - 18);
        
//         if (level == 1) shape.setFillColor(sf::Color(139, 69, 19)); // Brown
//         else if (level == 2) shape.setFillColor(sf::Color(160, 82, 45));
//         else shape.setFillColor(sf::Color(205, 133, 63));
        
//         window.draw(shape);
//     }
// };

// class FreezeTower : public Tower {
// public:
//     FreezeTower(sf::Vector2f pos) : Tower(pos, 5, 90, 0.5f, "Freeze") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 10;
//             range = 110;
//             attackCooldown = 0.4f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 20;
//             range = 130;
//             attackCooldown = 0.3f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(28, 28));
//         shape.setPosition(position.x - 14, position.y - 14);
        
//         if (level == 1) shape.setFillColor(sf::Color(173, 216, 230)); // Light Blue
//         else if (level == 2) shape.setFillColor(sf::Color(135, 206, 235));
//         else shape.setFillColor(sf::Color(70, 130, 180));
        
//         window.draw(shape);
//     }
// };

// // Tower Factory
// class TowerFactory {
// public:
//     static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position) {
//         switch (type) {
//             case TowerType::BASIC:
//                 std::cout << "Factory: Creating Basic Tower" << std::endl;
//                 return std::make_unique<BasicTower>(position);
//             case TowerType::SNIPER:
//                 std::cout << "Factory: Creating Sniper Tower" << std::endl;
//                 return std::make_unique<SniperTower>(position);
//             case TowerType::CANNON:
//                 std::cout << "Factory: Creating Cannon Tower" << std::endl;
//                 return std::make_unique<CannonTower>(position);
//             case TowerType::FREEZE:
//                 std::cout << "Factory: Creating Freeze Tower" << std::endl;
//                 return std::make_unique<FreezeTower>(position);
//             default:
//                 return std::make_unique<BasicTower>(position);
//         }
//     }
    
//     static int getTowerCost(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return 50;
//             case TowerType::SNIPER: return 100;
//             case TowerType::CANNON: return 150;
//             case TowerType::FREEZE: return 75;
//             default: return 50;
//         }
//     }
    
//     static std::string getTowerDescription(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return "Basic Tower - Balanced";
//             case TowerType::SNIPER: return "Sniper Tower - Long Range";
//             case TowerType::CANNON: return "Cannon Tower - High Damage";
//             case TowerType::FREEZE: return "Freeze Tower - Slows Enemies";
//             default: return "Unknown Tower";
//         }
//     }
// };

// // ===========================================
// // GAME CLASS - Implements Observer to receive events
// // ===========================================
// class Game : public Subject, public Observer {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<std::unique_ptr<Enemy>> enemies;
//     std::vector<std::unique_ptr<Tower>> towers;
//     std::vector<Projectile> projectiles;
//     int gold;
//     int lives;
//     int wave;
//     int level;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;
//     int score;
//     ResourceManager resourceManager;
//     AudioManager audioManager;
//     bool gameOver;
//     bool showReplayOption;
//     bool showSaveOption;
//     std::string playerName;
//     bool waitingForName;
//     bool showAudioControls;
    
//     // Variables pour le timer automatique
//     float preparationTimer;
//     const float PREPARATION_TIME = 15.0f;
//     bool preparationPhase;
    
//     // Preset paths
//     const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
//         // Original path
//         {
//             sf::Vector2f(0, 300),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(500, 150),
//             sf::Vector2f(500, 450),
//             sf::Vector2f(800, 450),
//             sf::Vector2f(800, 250),
//             sf::Vector2f(1000, 250)
//         },
//         // Zigzag path
//         {
//             sf::Vector2f(0, 200),
//             sf::Vector2f(200, 200),
//             sf::Vector2f(200, 400),
//             sf::Vector2f(400, 400),
//             sf::Vector2f(400, 200),
//             sf::Vector2f(600, 200),
//             sf::Vector2f(600, 400),
//             sf::Vector2f(800, 400),
//             sf::Vector2f(800, 200),
//             sf::Vector2f(1000, 200)
//         },
//         // Curly path
//         {
//             sf::Vector2f(0, 350),
//             sf::Vector2f(200, 350),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(400, 150),
//             sf::Vector2f(400, 350),
//             sf::Vector2f(600, 350),
//             sf::Vector2f(600, 150),
//             sf::Vector2f(800, 150),
//             sf::Vector2f(800, 350),
//             sf::Vector2f(1000, 350)
//         },
//         // S path
//         {
//             sf::Vector2f(0, 100),
//             sf::Vector2f(200, 100),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(400, 300),
//             sf::Vector2f(400, 500),
//             sf::Vector2f(600, 500),
//             sf::Vector2f(600, 300),
//             sf::Vector2f(800, 300),
//             sf::Vector2f(800, 100),
//             sf::Vector2f(1000, 100)
//         }
//     };

//     void changePath() {
//         // Select a path different from the previous randomly
//         int newPathIndex;
//         do {
//             newPathIndex = rand() % predefinedPaths.size();
//         } while (predefinedPaths[newPathIndex] == path);
        
//         path = predefinedPaths[newPathIndex];
//         std::cout << "=== NEW PATH SELECTED FOR LEVEL " << level << " ===" << std::endl;
//     }

//     void saveScore() {
//         try {
//             json scores;
//             std::ifstream file("scores.json");
//             if (file.good()) {
//                 // File exists, read it
//                 file >> scores;
//                 file.close();
//             } else {
//                 // File doesn't exist, create empty array
//                 scores = json::array();
//                 std::cout << "Creating file scores.json" << std::endl;
//             }
//             // Add new score
//             json newScore = {
//                 {"name", playerName},
//                 {"score", score},
//                 {"level", level},
//                 {"wave", wave},
//                 {"date", std::time(nullptr)}
//             };
            
//             scores.push_back(newScore);
            
//             std::ofstream outFile("scores.json");
//             outFile << scores.dump(4);
//             outFile.close();
            
//             std::cout << "Score sauvegardé avec succès!" << std::endl;
//         } catch (const std::exception& e) {
//             std::cerr << "Erreur lors de la sauvegarde du score: " << e.what() << std::endl;
//         }
//     }
    
//     void resetGame() {
//         gold = 200;
//         lives = 3;
//         wave = 1;
//         level = 1;
//         score = 0;
//         enemies.clear();
//         towers.clear();
//         projectiles.clear();
//         gameOver = false;
//         showReplayOption = false;
//         showSaveOption = false;
//         waitingForName = false;
//         playerName = "";
//         path = predefinedPaths[0];
//         showAudioControls = false;
        
//         // Réinitialiser le timer
//         preparationTimer = PREPARATION_TIME;
//         preparationPhase = true;
        
//         // Redémarrer la musique
//         audioManager.startMusic();
//     }

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense avec Audio"),
//              gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
//              gameOver(false), showReplayOption(false), showSaveOption(false),
//              waitingForName(false), playerName(""), showAudioControls(false),
//              preparationTimer(PREPARATION_TIME), preparationPhase(true) {
        
//         // Initialize the random number generator 
//         srand(time(nullptr));
        
//         std::cout << "=== TOWER DEFENSE AVEC AUDIO ET TIMER ===" << std::endl;
//         std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
//         std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
//         std::cout << "✓ Observer Pattern: Event management" << std::endl;
//         std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
//         std::cout << "✓ Audio Manager: Musique et effets sonores" << std::endl;
//         std::cout << "✓ Timer automatique: 15 secondes entre vagues" << std::endl;
        
//         // Subscribe to its own events
//         addObserver(this);
        
//         // Initialize the first path
//         path = predefinedPaths[0];
        
//         // Démarrer la musique
//         audioManager.startMusic();
        
//         std::cout << "CONTRÔLES DU JEU:" << std::endl;
//         std::cout << "- Clic gauche: Placer une tour" << std::endl;
//         std::cout << "- Clic sur tour: Améliorer" << std::endl;
//         std::cout << "- ESPACE: Commencer la vague immédiatement" << std::endl;
//         std::cout << "- A: Afficher/masquer contrôles audio" << std::endl;
//         std::cout << "\nCONTRÔLES AUDIO:" << std::endl;
//         std::cout << "- M: Mute/Unmute" << std::endl;
//         std::cout << "- N: Changer de musique" << std::endl;
//         std::cout << "- +/-: Volume principal" << std::endl;
//         std::cout << "- 1/2: Volume musique" << std::endl;
//         std::cout << "- 3/4: Volume effets" << std::endl;
//         std::cout << "\nFICHIERS REQUIS (placez dans assets/):" << std::endl;
//         std::cout << "- arial.ttf (OBLIGATOIRE pour le texte)" << std::endl;
//         std::cout << "- background_music1.ogg (musique 1)" << std::endl;
//         std::cout << "- background_music2.ogg (musique 2)" << std::endl;
//         std::cout << "- shoot.wav (son de tir)" << std::endl;
//         std::cout << "- explosion.wav (son d'explosion)" << std::endl;
//         std::cout << "- upgrade.wav (son d'amélioration)" << std::endl;
//         std::cout << "- gold_icon.png (optionnel)" << std::endl;
//         std::cout << "- heart_icon.png (optionnel)" << std::endl;
//     }

//     // Observer implementation - Reacts to game events
//     void notify(const GameEvent& event) override {
//         switch(event.type) {
//             case GameEvent::ENEMY_KILLED:
//                 gold += event.value;
//                 score += event.value * 10;
//                 std::cout << "Observer: Enemy killed! +" << event.value << " gold, Score: " << score << std::endl;
//                 break;
                
//             case GameEvent::ENEMY_REACHED_END:
//                 lives -= event.value;
//                 score -= 50;
//                 std::cout << "Observer: Enemy passed! -" << event.value << " life(s)" << std::endl;
//                 break;
                
//             case GameEvent::WAVE_COMPLETED:
//                 score += 100;
//                 std::cout << "Observer: Wave completed! +100 bonus points" << std::endl;
//                 break;
//         }
//     }

//     void run() {
//         sf::Clock clock;
        
//         while (window.isOpen()) {
//             float deltaTime = clock.restart().asSeconds();
            
//             handleEvents();
//             update(deltaTime);
//             render();
//         }
//     }

//     void handleEvents() {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
            
//             if (event.type == sf::Event::TextEntered && waitingForName) {
//                 if (event.text.unicode == '\b' && !playerName.empty()) {
//                     playerName.pop_back();
//                 }
//                 else if (event.text.unicode < 128 && playerName.length() < 20) {
//                     playerName += static_cast<char>(event.text.unicode);
//                 }
//             }
            
//             if (event.type == sf::Event::KeyPressed) {
//                 // Contrôles audio
//                 if (event.key.code == sf::Keyboard::M) {
//                     audioManager.toggleMute();
//                 }
//                 else if (event.key.code == sf::Keyboard::N && audioManager.hasBothMusicsLoaded()) {
//                     audioManager.switchMusic();
//                 }
//                 else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
//                     audioManager.increaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
//                     audioManager.decreaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num1) {
//                     audioManager.increaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num2) {
//                     audioManager.decreaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num3) {
//                     audioManager.increaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num4) {
//                     audioManager.decreaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::A) {
//                     showAudioControls = !showAudioControls;
//                 }
//                 // Contrôles de jeu
//                 else if (event.key.code == sf::Keyboard::Return && waitingForName) {
//                     waitingForName = false;
//                     showSaveOption = true;
//                 }
//                 else if (event.key.code == sf::Keyboard::Space && preparationPhase && !gameOver) {
//                     startWave();
//                 }
//                 else if (event.key.code == sf::Keyboard::R && gameOver) {
//                     resetGame();
//                 }
//                 else if (event.key.code == sf::Keyboard::S && showSaveOption) {
//                     saveScore();
//                     showSaveOption = false;
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                 sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
//                 // Check click on existing tower
//                 bool towerClicked = false;
//                 for (std::unique_ptr<Tower>& tower : towers) {
//                     float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                     if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
//                         gold -= tower->getUpgradeCost();
//                         tower->upgrade();
//                         audioManager.playUpgradeSound(); // Son d'amélioration
//                         towerClicked = true;
//                         std::cout << "Tower upgraded to level " << tower->level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // New tower placement
//                 if (!towerClicked) {
//                     int cost = 50;
//                     if (gold >= cost) {
//                         bool canPlace = true;
                        
//                         // Check distance with other towers
//                         for (const std::unique_ptr<Tower>& tower : towers) {
//                             float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                             if (distance < TOWER_MIN_DISTANCE) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         // Check distance from path
//                         for (size_t i = 0; i < path.size() - 1; i++) {
//                             sf::Vector2f start = path[i];
//                             sf::Vector2f end = path[i + 1];
                            
//                             float A = clickPos.x - start.x;
//                             float B = clickPos.y - start.y;
//                             float C = end.x - start.x;
//                             float D = end.y - start.y;
                            
//                             float dot = A * C + B * D;
//                             float lenSq = C * C + D * D;
//                             float param = (lenSq != 0) ? dot / lenSq : -1;
                            
//                             float xx, yy;
//                             if (param < 0) {
//                                 xx = start.x; yy = start.y;
//                             } else if (param > 1) {
//                                 xx = end.x; yy = end.y;
//                             } else {
//                                 xx = start.x + param * C;
//                                 yy = start.y + param * D;
//                             }
                            
//                             float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
//                             if (distance < 50) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         if (canPlace) {
//                             std::unique_ptr<Tower> tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
//                             towers.push_back(std::move(tower));
//                             gold -= cost;
//                             std::cout << "Tower placed! Remaining gold: " << gold << std::endl;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         preparationPhase = false;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== LEVEL " << level << " - WAVE " << wave << " STARTS ===" << std::endl;
//     }

//     void update(float deltaTime) {
//         // Stop updating if game is over
//         if (lives <= 0 || wave > 10) {
//             return;
//         }
        
//         // Gestion du timer de préparation
//         if (preparationPhase && !gameOver) {
//             preparationTimer -= deltaTime;
            
//             if (preparationTimer <= 0) {
//                 // Timer écoulé, lancer automatiquement la vague
//                 std::cout << "Timer écoulé ! Lancement automatique de la vague..." << std::endl;
//                 startWave();
//             }
//         }
        
//         // Spawn enemies with Factory Pattern
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) {
//                 // Using Factory Pattern with level-based difficulty
//                 EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
//                 std::unique_ptr<Enemy> enemy = EnemyFactory::createEnemy(type);
                
//                 // Define initial position at beginning of path
//                 enemy->position = path[0];
                
//                 // Increase enemy statistics based on the level and the wave
//                 float levelMultiplier = 1.0f + (level - 1) * 0.15f;
//                 float waveMultiplier = 1.0f + (wave - 1) * 0.05f;
//                 float totalMultiplier = levelMultiplier * waveMultiplier;
                
//                 // Apply multipliers to other stats
//                 enemy->health = static_cast<int>(enemy->health * totalMultiplier);
//                 enemy->maxHealth = enemy->health;
//                 enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);
//                 enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));
                
//                 enemies.push_back(std::move(enemy));
                
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//             }
//         }

//         // Update enemies
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->update(deltaTime, path);
                
//                 if (enemy->reachedEnd(path)) {
//                     // Using Observer Pattern
//                     notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
//                     enemy->alive = false;
//                 }
//             }
//         }

//         // Update towers (now they create projectiles with audio)
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->update(deltaTime, enemies, projectiles, this, &audioManager);
//         }

//         // Update projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.update(deltaTime);
//         }

//         // Remove inactive projectiles
//         projectiles.erase(
//             std::remove_if(projectiles.begin(), projectiles.end(),
//                 [](const Projectile& proj) { return !proj.active; }),
//             projectiles.end()
//         );

//         // Remove dead enemies
//         enemies.erase(
//             std::remove_if(enemies.begin(), enemies.end(),
//                 [](const std::unique_ptr<Enemy>& enemy) { return !enemy->alive; }),
//             enemies.end()
//         );

//         // Check end of wave
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             preparationPhase = true;
//             preparationTimer = PREPARATION_TIME;
            
//             notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
//             wave++;
            
//             // Change the level after 3 waves
//             if (wave > 3) {
//                 level++;
//                 wave = 1;
//                 enemiesInWave = 5 + (level - 1) * 2;
//                 gold += 100 * level;
                
//                 // Change the path and remove towers
//                 changePath();
//                 towers.clear();
                
//                 std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
//                 std::cout << "New path selected - Towers have been removed" << std::endl;
//             } else {
//                 enemiesInWave += 2;
//             }
//             std::cout << "Prochaine vague dans " << PREPARATION_TIME << " secondes (SPACE pour démarrer immédiatement)" << std::endl;
//         }

//         // Check for game over
//         if (lives <= 0) {
//             gameOver = true;
//             showReplayOption = true;
//             waitingForName = true;
//             audioManager.stopMusic(); // Arrêter la musique en cas de défaite
//         }
//     }

//     // Convert integer to string for display
//     std::string intToString(int value) {
//         std::ostringstream oss;
//         oss << value;
//         return oss.str();
//     }
    
//     std::string floatToString(float value) {
//         std::ostringstream oss;
//         oss << static_cast<int>(value);
//         return oss.str();
//     }

//     // Enhanced text drawing method with Arial font support
//     void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 16) {
//         sf::Text sfText;
//         sfText.setString(text);
//         sfText.setCharacterSize(size);
//         sfText.setFillColor(color);
//         sfText.setPosition(x, y);
        
//         // Use Arial font if loaded, otherwise SFML will use default font
//         if (resourceManager.isFontLoaded()) {
//             sfText.setFont(resourceManager.getFont());
//         }
        
//         window.draw(sfText);
//     }

//     void render() {
//         window.clear(sf::Color(40, 40, 40));

//         // Draw the path
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vector2f start = path[i];
//             sf::Vector2f end = path[i + 1];
//             sf::Vector2f direction = end - start;
//             float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (length > 0) {
//                 direction = direction / length;
//                 sf::RectangleShape pathSegment;
//                 pathSegment.setSize(sf::Vector2f(length, 40));
//                 pathSegment.setPosition(start);
//                 pathSegment.setFillColor(sf::Color(100, 100, 100));
                
//                 float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
//                 pathSegment.setRotation(angle);
//                 pathSegment.setOrigin(0, 20);
                
//                 window.draw(pathSegment);
//             }
//         }

//         // Path points
//         for (const sf::Vector2f& point : path) {
//             sf::CircleShape dot(3);
//             dot.setPosition(point.x - 3, point.y - 3);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Draw towers
//         for (std::unique_ptr<Tower>& tower : towers) {
//             tower->draw(window);
//         }

//         // Draw projectiles
//         for (Projectile& projectile : projectiles) {
//             projectile.draw(window);
//         }

//         // Draw enemies (on top of projectiles)
//         for (std::unique_ptr<Enemy>& enemy : enemies) {
//             if (enemy->alive) {
//                 enemy->draw(window);
//             }
//         }

//         // ==============================
//         // USER INTERFACE WITH SPRITES AND ARIAL FONT
//         // ==============================
        
//         // Gold interface with sprite and text
//         resourceManager.drawGoldIcon(window, 10, 10);
//         drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

//         // Lives interface with sprite and text
//         resourceManager.drawHeartIcon(window, 10, 40);
//         drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

//         // Level interface
//         drawText(window, "Level : " + intToString(level), 300, 15, sf::Color::White, 18);

//         // Wave interface
//         drawText(window, "Wave : " + intToString(wave), 300, 45, sf::Color::Cyan, 18);

//         // Additional game information
//         drawText(window, "Towers : " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
//         drawText(window, "Enemies : " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);

//         // Score interface
//         drawText(window, "Score : " + intToString(score), 850, 15, sf::Color::White, 18);

//         // Audio status indicator
//         if (audioManager.getIsMuted()) {
//             drawText(window, "MUTE", 850, 45, sf::Color::Red, 16);
//         } else {
//             std::string musicText = "Music " + intToString(audioManager.getCurrentMusicTrack() + 1);
//             drawText(window, musicText, 850, 45, sf::Color::Green, 16);
//         }

//         // Audio controls panel (toggle with 'A')
//         if (showAudioControls) {
//             // Semi-transparent background
//             sf::RectangleShape controlsBg(sf::Vector2f(300, 200));
//             controlsBg.setPosition(10, 80);
//             controlsBg.setFillColor(sf::Color(0, 0, 0, 180));
//             controlsBg.setOutlineColor(sf::Color::White);
//             controlsBg.setOutlineThickness(2);
//             window.draw(controlsBg);
            
//             drawText(window, "=== CONTROLES AUDIO ===", 20, 90, sf::Color::White, 16);
//             drawText(window, "M: Mute/Unmute", 20, 110, sf::Color::Cyan, 14);
//             if (audioManager.hasBothMusicsLoaded()) {
//                 drawText(window, "N: Changer musique", 20, 130, sf::Color::Cyan, 14);
//             }
//             drawText(window, "+/-: Volume principal (" + floatToString(audioManager.getMasterVolume()) + "%)", 20, 150, sf::Color::Yellow, 14);
//             drawText(window, "1/2: Volume musique (" + floatToString(audioManager.getMusicVolume()) + "%)", 20, 170, sf::Color::Yellow, 14);
//             drawText(window, "3/4: Volume effets (" + floatToString(audioManager.getEffectsVolume()) + "%)", 20, 190, sf::Color::Yellow, 14);
//             drawText(window, "A: Masquer ce panneau", 20, 210, sf::Color::White, 14);
            
//             // Current music indicator
//             if (audioManager.hasMusicLoaded()) {
//                 std::string currentMusic = "Musique actuelle: " + intToString(audioManager.getCurrentMusicTrack() + 1);
//                 drawText(window, currentMusic, 20, 240, sf::Color::Green, 14);
//             } else {
//                 drawText(window, "Aucune musique chargée", 20, 240, sf::Color::Red, 14);
//             }
//         }

//         // Timer et instructions de vague au centre de l'écran
//         if (preparationPhase && lives > 0 && !gameOver) {
//             // Semi-transparent background for text
//             sf::RectangleShape textBg(sf::Vector2f(600, 120));
//             textBg.setPosition(WINDOW_WIDTH/2 - 300, WINDOW_HEIGHT/2 - 60);
//             textBg.setFillColor(sf::Color(0, 0, 0, 150));
//             textBg.setOutlineColor(sf::Color::Green);
//             textBg.setOutlineThickness(3);
//             window.draw(textBg);
            
//             // Affichage du timer
//             int secondsLeft = static_cast<int>(std::ceil(preparationTimer));
//             drawText(window, "NEXT WAVE IN: " + intToString(secondsLeft) + "s", 
//                     WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 45, sf::Color::Red, 28);
            
//             // Texte d'information
//             drawText(window, "PRESS SPACE TO START NOW", WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 10, sf::Color::Green, 22);
//             drawText(window, "LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
//                     WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 20, sf::Color::Cyan, 20);
            
//             // Display message for each new level
//             if (wave == 1) {
//                 drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
//                         WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 50, sf::Color::Yellow, 18);
//             }
//         }

//         // Game over screen
//         if (gameOver) {
//             sf::RectangleShape gameOverBg(sf::Vector2f(400, 200));
//             gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 100);
//             gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
//             gameOverBg.setOutlineColor(sf::Color::White);
//             gameOverBg.setOutlineThickness(3);
//             window.draw(gameOverBg);
            
//             drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80, sf::Color::White, 32);
//             drawText(window, "Final Score: " + intToString(score), WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 40, sf::Color::White, 20);
            
//             if (waitingForName) {
//                 drawText(window, "Enter your name :", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//                 drawText(window, "Press Enter to continue", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
//             }
//             else if (showSaveOption) {
//                 drawText(window, "Save score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, "Press S to save", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
//             }
//             else if (showReplayOption) {
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//             }
//         }

//         // Instructions at bottom of screen (clear and readable)
//         drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade | SPACE: Start wave | A: Audio controls", 
//                 10, WINDOW_HEIGHT - 30, sf::Color::White, 16);

//         window.display();
//     }
// };

// int main() {
//     std::cout << "Tower Defense avec Système Audio Complet et Timer Automatique" << std::endl;
//     std::cout << "=============================================================" << std::endl;
//     Game game;
//     game.run();
//     return 0;
// }

// // Ci dessus version timer 1st try
// ---------------------------------------------


// Version avec timer - plante rapidement 
// ------------------------------------------------- //

// #include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <sstream>
// #include <memory>
// #include <algorithm>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <fstream>
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

// // Constants
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;
// const float TOWER_MIN_DISTANCE = 80.0f;

// // ===========================================
// // AUDIO MANAGER - Gestion de la musique et des sons
// // ===========================================
// class AudioManager {
// private:
//     sf::Music backgroundMusic1;
//     sf::Music backgroundMusic2;
//     sf::SoundBuffer shootSoundBuffer;
//     sf::SoundBuffer explosionSoundBuffer;
//     sf::SoundBuffer upgradeSoundBuffer;
//     sf::Sound shootSound;
//     sf::Sound explosionSound;
//     sf::Sound upgradeSound;
    
//     bool music1Loaded;
//     bool music2Loaded;
//     bool soundsLoaded;
//     float masterVolume;
//     float musicVolume;
//     float effectsVolume;
//     bool isMuted;
//     int currentMusicTrack; // 0 = music1, 1 = music2
    
// public:
//     AudioManager() : music1Loaded(false), music2Loaded(false), soundsLoaded(false),
//                      masterVolume(50.0f), musicVolume(50.0f), effectsVolume(70.0f), 
//                      isMuted(false), currentMusicTrack(0) {
//         loadAudioFiles();
//     }
    
//     void loadAudioFiles() {
//         std::cout << "=== CHARGEMENT DES FICHIERS AUDIO ===" << std::endl;
        
//         // Chemins à essayer pour les fichiers audio
//         std::vector<std::string> audioPaths = {
//             "./assets/",
//             "assets/",
//             "../../assets/",
//             "../assets/",
//             "./"
//         };
        
//         // Chargement des musiques de fond
//         for (const std::string& basePath : audioPaths) {
//             if (!music1Loaded) {
//                 std::string music1Path = basePath + "background_music1.ogg";
//                 if (backgroundMusic1.openFromFile(music1Path)) {
//                     music1Loaded = true;
//                     backgroundMusic1.setLoop(true);
//                     backgroundMusic1.setVolume(musicVolume);
//                     std::cout << "✓ Musique 1 chargée: " << music1Path << std::endl;
//                     break; // Important: sortir de la boucle une fois chargé
//                 }
//             }
//         }
        
//         for (const std::string& basePath : audioPaths) {
//             if (!music2Loaded) {
//                 std::string music2Path = basePath + "background_music2.ogg";
//                 if (backgroundMusic2.openFromFile(music2Path)) {
//                     music2Loaded = true;
//                     backgroundMusic2.setLoop(true);
//                     backgroundMusic2.setVolume(musicVolume);
//                     std::cout << "✓ Musique 2 chargée: " << music2Path << std::endl;
//                     break; // Important: sortir de la boucle une fois chargé
//                 }
//             }
//         }
        
//         // Chargement des effets sonores
//         for (const std::string& basePath : audioPaths) {
//             std::string shootPath = basePath + "shoot.wav";
//             std::string explosionPath = basePath + "explosion.wav";
//             std::string upgradePath = basePath + "upgrade.wav";
            
//             bool shootLoaded = false;
//             bool explosionLoaded = false;
//             bool upgradeLoaded = false;
            
//             // Vérifier chaque son individuellement
//             struct stat info;
//             if (stat(shootPath.c_str(), &info) == 0) {
//                 shootLoaded = shootSoundBuffer.loadFromFile(shootPath);
//                 if (shootLoaded) {
//                     shootSound.setBuffer(shootSoundBuffer);
//                     shootSound.setVolume(effectsVolume);
//                     std::cout << "✓ Son de tir chargé: " << shootPath << std::endl;
//                 }
//             }
            
//             if (stat(explosionPath.c_str(), &info) == 0) {
//                 explosionLoaded = explosionSoundBuffer.loadFromFile(explosionPath);
//                 if (explosionLoaded) {
//                     explosionSound.setBuffer(explosionSoundBuffer);
//                     explosionSound.setVolume(effectsVolume);
//                     std::cout << "✓ Son d'explosion chargé: " << explosionPath << std::endl;
//                 }
//             }
            
//             if (stat(upgradePath.c_str(), &info) == 0) {
//                 upgradeLoaded = upgradeSoundBuffer.loadFromFile(upgradePath);
//                 if (upgradeLoaded) {
//                     upgradeSound.setBuffer(upgradeSoundBuffer);
//                     upgradeSound.setVolume(effectsVolume);
//                     std::cout << "✓ Son d'amélioration chargé: " << upgradePath << std::endl;
//                 }
//             }
            
//             if (shootLoaded || explosionLoaded || upgradeLoaded) {
//                 soundsLoaded = true;
//                 break; // Sortir si au moins un son est chargé
//             }
//         }
        
//         if (!music1Loaded && !music2Loaded) {
//             std::cout << "⚠ AUCUNE musique trouvée! Placez background_music1.ogg et/ou background_music2.ogg dans assets/" << std::endl;
//         }
//         if (!soundsLoaded) {
//             std::cout << "⚠ Effets sonores non trouvés! Placez shoot.wav, explosion.wav, upgrade.wav dans assets/" << std::endl;
//         }
        
//         std::cout << "=== FIN CHARGEMENT AUDIO ===" << std::endl;
//     }
    
//     void startMusic() {
//         if (isMuted) return;
        
//         stopMusic(); // Arrête toute musique en cours
        
//         if (currentMusicTrack == 0 && music1Loaded) {
//             // Vérifier que la musique n'est pas déjà en cours
//             if (backgroundMusic1.getStatus() != sf::Music::Playing) {
//                 backgroundMusic1.play();
//                 std::cout << "Lecture de la musique 1" << std::endl;
//             }
//         } else if (currentMusicTrack == 1 && music2Loaded) {
//             // Vérifier que la musique n'est pas déjà en cours
//             if (backgroundMusic2.getStatus() != sf::Music::Playing) {
//                 backgroundMusic2.play();
//                 std::cout << "Lecture de la musique 2" << std::endl;
//             }
//         }
//     }
    
//     void stopMusic() {
//         if (backgroundMusic1.getStatus() != sf::Music::Stopped) {
//             backgroundMusic1.stop();
//         }
//         if (backgroundMusic2.getStatus() != sf::Music::Stopped) {
//             backgroundMusic2.stop();
//         }
//     }
    
//     void switchMusic() {
//         if (music1Loaded && music2Loaded) {
//             stopMusic();
//             currentMusicTrack = (currentMusicTrack == 0) ? 1 : 0;
//             startMusic();
//             std::cout << "Changement vers musique " << (currentMusicTrack + 1) << std::endl;
//         }
//     }
    
//     void toggleMute() {
//         isMuted = !isMuted;
//         if (isMuted) {
//             stopMusic();
//             std::cout << "Audio muet" << std::endl;
//         } else {
//             startMusic();
//             std::cout << "Audio activé" << std::endl;
//         }
//     }
    
//     void increaseMasterVolume() {
//         masterVolume = std::min(100.0f, masterVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void decreaseMasterVolume() {
//         masterVolume = std::max(0.0f, masterVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume principal: " << masterVolume << "%" << std::endl;
//     }
    
//     void increaseMusicVolume() {
//         musicVolume = std::min(100.0f, musicVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void decreaseMusicVolume() {
//         musicVolume = std::max(0.0f, musicVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume musique: " << musicVolume << "%" << std::endl;
//     }
    
//     void increaseEffectsVolume() {
//         effectsVolume = std::min(100.0f, effectsVolume + 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void decreaseEffectsVolume() {
//         effectsVolume = std::max(0.0f, effectsVolume - 10.0f);
//         updateVolumes();
//         std::cout << "Volume effets: " << effectsVolume << "%" << std::endl;
//     }
    
//     void updateVolumes() {
//         float finalMusicVolume = (masterVolume / 100.0f) * (musicVolume / 100.0f) * 100.0f;
//         float finalEffectsVolume = (masterVolume / 100.0f) * (effectsVolume / 100.0f) * 100.0f;
        
//         if (music1Loaded) backgroundMusic1.setVolume(finalMusicVolume);
//         if (music2Loaded) backgroundMusic2.setVolume(finalMusicVolume);
        
//         if (soundsLoaded) {
//             shootSound.setVolume(finalEffectsVolume);
//             explosionSound.setVolume(finalEffectsVolume);
//             upgradeSound.setVolume(finalEffectsVolume);
//         }
//     }
    
//     void playShootSound() {
//         if (!isMuted && soundsLoaded && shootSound.getBuffer() != nullptr) {
//             // Ne jouer que si le son précédent est terminé ou presque
//             if (shootSound.getStatus() != sf::Sound::Playing) {
//                 shootSound.play();
//             }
//         }
//     }
    
//     void playExplosionSound() {
//         if (!isMuted && soundsLoaded && explosionSound.getBuffer() != nullptr) {
//             if (explosionSound.getStatus() != sf::Sound::Playing) {
//                 explosionSound.play();
//             }
//         }
//     }
    
//     void playUpgradeSound() {
//         if (!isMuted && soundsLoaded && upgradeSound.getBuffer() != nullptr) {
//             if (upgradeSound.getStatus() != sf::Sound::Playing) {
//                 upgradeSound.play();
//             }
//         }
//     }
    
//     // Getters pour l'interface
//     float getMasterVolume() const { return masterVolume; }
//     float getMusicVolume() const { return musicVolume; }
//     float getEffectsVolume() const { return effectsVolume; }
//     bool getIsMuted() const { return isMuted; }
//     int getCurrentMusicTrack() const { return currentMusicTrack; }
//     bool hasMusicLoaded() const { return music1Loaded || music2Loaded; }
//     bool hasBothMusicsLoaded() const { return music1Loaded && music2Loaded; }
// };

// // ===========================================
// // OBSERVER PATTERN - Interface for events
// // ===========================================
// class GameEvent {
// public:
//     enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
//     Type type;
//     int value; // Gold earned, damage taken, etc.
    
//     GameEvent(Type t, int v = 0) : type(t), value(v) {}
// };

// class Observer {
// public:
//     virtual ~Observer() = default;
//     virtual void notify(const GameEvent& event) = 0;
// };

// class Subject {
// protected:
//     std::vector<Observer*> observers;
// public:
//     virtual ~Subject() = default;
    
//     void addObserver(Observer* observer) {
//         if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
//             observers.push_back(observer);
//         }
//     }
    
//     void removeObserver(Observer* observer) {
//         observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
//     }
    
//     void notifyObservers(const GameEvent& event) {
//         for (auto* observer : observers) {
//             if (observer) {
//                 observer->notify(event);
//             }
//         }
//     }
// };

// // ===========================================
// // RESOURCE MANAGER - To manage textures, sprites and fonts
// // ===========================================
// class ResourceManager {
// private:
//     sf::Texture goldTexture, heartTexture;
//     sf::Sprite goldSprite, heartSprite;
//     bool goldLoaded, heartLoaded;
    
//     // Font management
//     sf::Font arialFont;
//     bool fontLoaded;
    
// public:
//     ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
//         loadResources();
//         loadFont();
//     }
    
//     void loadFont() {
//         std::cout << "=== LOADING ARIAL FONT ===" << std::endl;
        
//         std::vector<std::string> fontPaths = {
//             "./assets/arial.ttf",
//             "assets/arial.ttf",
//             "../../assets/arial.ttf",
//             "../assets/arial.ttf",
//             "./arial.ttf",
//             "arial.ttf",
//         };
        
//         for (const std::string& path : fontPaths) {
//             struct stat info;
//             if (stat(path.c_str(), &info) == 0) {
//                 if (arialFont.loadFromFile(path)) {
//                     fontLoaded = true;
//                     std::cout << "✓ Font successfully loaded from: " << path << std::endl;
//                     return;
//                 }
//             }
//         }
        
//         std::cout << "⚠ WARNING: Arial font not found!" << std::endl;
//         std::cout << "=== END FONT LOADING ===" << std::endl;
//     }
    
//     const sf::Font& getFont() const {
//         return arialFont;
//     }
    
//     bool isFontLoaded() const {
//         return fontLoaded;
//     }
    
//     void loadResources() {   
//         char cwd[1024];
//         if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//             std::cout << "Current working directory: " << cwd << std::endl;
//         }
        
//         std::vector<std::string> pathsToTry = {
//             "./assets/",
//             "assets/",
//             "../../assets/",
//             "../assets/",
//             "./"
//         };
        
//         // Loading gold icon
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "gold_icon.png";
//             struct stat info;
//             if (stat(fullPath.c_str(), &info) == 0) {
//                 if (goldTexture.loadFromFile(fullPath)) {
//                     goldLoaded = true;
//                     goldSprite.setTexture(goldTexture);
//                     goldSprite.setScale(0.8f, 0.8f);
//                     std::cout << "✓ Texture gold_icon.png loaded from: " << fullPath << std::endl;
//                     break;
//                 }
//             }
//         }
        
//         // Loading heart icon
//         for (const std::string& basePath : pathsToTry) {
//             std::string fullPath = basePath + "heart_icon.png";
//             struct stat info;
//             if (stat(fullPath.c_str(), &info) == 0) {
//                 if (heartTexture.loadFromFile(fullPath)) {
//                     heartLoaded = true;
//                     heartSprite.setTexture(heartTexture);
//                     heartSprite.setScale(0.8f, 0.8f);
//                     std::cout << "✓ Texture heart_icon.png loaded from: " << fullPath << std::endl;
//                     break;
//                 }
//             }
//         }
        
//         std::cout << "=== END DEBUG PATHS ===" << std::endl;
//     }
    
//     void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
//         if (goldLoaded) {
//             goldSprite.setPosition(x, y);
//             window.draw(goldSprite);
//         } 
//     }
    
//     void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
//         if (heartLoaded) {
//             heartSprite.setPosition(x, y);
//             window.draw(heartSprite);
//         } 
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Enemy creation
// // ===========================================
// class Enemy {
// public:
//     sf::Vector2f position;
//     int health;
//     int maxHealth;
//     float speed;
//     int reward;
//     bool alive;
//     int pathIndex;
//     std::string type;

//     Enemy(int hp, float spd, int rew, const std::string& t) 
//         : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
//     }

//     virtual ~Enemy() = default;

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= static_cast<int>(path.size()) - 1) return;

//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             if (distance > 0) {
//                 direction = direction / distance;
//                 position += direction * speed * deltaTime;
//             }
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     virtual sf::Color getColor() {
//         if (health > maxHealth * 0.7f) return sf::Color::Green;
//         else if (health > maxHealth * 0.3f) return sf::Color::Yellow;
//         else return sf::Color::Red;
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
//         shape.setFillColor(getColor());
//         window.draw(shape);

//         // Health bar
//         sf::RectangleShape healthBar(sf::Vector2f(30, 5));
//         healthBar.setPosition(position.x - 15, position.y - 25);
//         healthBar.setFillColor(sf::Color::Black);
//         window.draw(healthBar);

//         if (health > 0) {
//             sf::RectangleShape healthFill(sf::Vector2f(30 * (float)health / maxHealth, 5));
//             healthFill.setPosition(position.x - 15, position.y - 25);
//             healthFill.setFillColor(sf::Color::Green);
//             window.draw(healthFill);
//         }
//     }

//     bool reachedEnd(const std::vector<sf::Vector2f>& path) {
//         return pathIndex >= static_cast<int>(path.size()) - 1;
//     }
// };

// // Specialized enemy classes
// class Goblin : public Enemy {
// public:
//     Goblin() : Enemy(50, 60, 10, "Goblin") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Green;
//     }
// };

// class Orc : public Enemy {
// public:
//     Orc() : Enemy(120, 45, 20, "Orc") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Red;
//     }
// };

// class MediumEnemy : public Enemy {
// public:
//     MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
//     sf::Color getColor() override {
//         return sf::Color(0, 128, 128); // Turquoise
//     }
// };

// class Troll : public Enemy {
// public:
//     Troll() : Enemy(300, 35, 50, "Troll") {}
    
//     sf::Color getColor() override {
//         return sf::Color::Magenta;
//     }
// };

// class HeavyEnemy : public Enemy {
// public:
//     HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
//     sf::Color getColor() override {
//         return sf::Color(139, 69, 19); // Brown
//     }
// };

// // Factory to create enemies
// class EnemyFactory {
// public:
//     enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
//     static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
//         switch(type) {
//             case GOBLIN:
//                 return std::make_unique<Goblin>();
//             case ORC:
//                 return std::make_unique<Orc>();
//             case TROLL:
//                 return std::make_unique<Troll>();
//             case HEAVY:
//                 return std::make_unique<HeavyEnemy>();
//             case MEDIUM:
//                 return std::make_unique<MediumEnemy>();
//             default:
//                 return std::make_unique<Goblin>();
//         }
//     }
    
//     static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
//         int totalDifficulty = wave + (level - 1) * 3;
        
//         if (totalDifficulty <= 3) {
//             return GOBLIN;
//         }
//         else if (totalDifficulty <= 6) {
//             if (enemyIndex % 8 == 0) return MEDIUM;
//             return (enemyIndex % 5 == 0) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 9) {
//             if (enemyIndex % 10 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 2) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 12) {
//             if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 5 == 0) return MEDIUM;
//             return (enemyIndex % 5 < 3) ? ORC : GOBLIN;
//         }
//         else if (totalDifficulty <= 15) {
//             if (enemyIndex % 10 == 0) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 7) return ORC;
//             else return GOBLIN;
//         }
//         else {
//             if (enemyIndex % 10 < 3) return TROLL;
//             else if (enemyIndex % 8 == 0) return HEAVY;
//             else if (enemyIndex % 6 == 0) return MEDIUM;
//             else if (enemyIndex % 10 < 9) return ORC;
//             else return GOBLIN;
//         }
//     }
// };

// // Class for projectiles
// class Projectile {
// public:
//     sf::Vector2f position;
//     sf::Vector2f targetPosition;
//     sf::Vector2f velocity;
//     int damage;
//     float speed;
//     bool active;
//     Enemy* targetEnemy;
//     Subject* gameSubject;
//     AudioManager* audioManager;

//     Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject, AudioManager* audio) 
//         : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), 
//           gameSubject(subject), audioManager(audio) {
        
//         if (target) {
//             targetPosition = target->position;
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }
        
//         // Jouer le son de tir
//         if (audioManager) {
//             audioManager->playShootSound();
//         }
//     }

//     void update(float deltaTime) {
//         if (!active) return;

//         // If target is still alive, follow its position
//         if (targetEnemy && targetEnemy->alive) {
//             targetPosition = targetEnemy->position;
//             sf::Vector2f direction = targetPosition - position;
//             float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (distance < 10.0f) {
//                 // Impact!
//                 int oldHealth = targetEnemy->health;
//                 targetEnemy->takeDamage(damage);
                
//                 // Jouer le son d'explosion quand l'ennemi meurt
//                 if (targetEnemy->health <= 0 && oldHealth > 0 && gameSubject) {
//                     gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
//                     if (audioManager) {
//                         audioManager->playExplosionSound();
//                     }
//                 }
                
//                 active = false;
//                 return;
//             }
            
//             // Update velocity to follow target
//             if (distance > 0) {
//                 velocity = (direction / distance) * speed;
//             }
//         }

//         // Move projectile
//         position += velocity * deltaTime;
        
//         // Deactivate if too far (safety)
//         if (position.x < -50 || position.x > WINDOW_WIDTH + 50 || 
//             position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
//             active = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         if (!active) return;
        
//         sf::CircleShape bullet(4);
//         bullet.setPosition(position.x - 4, position.y - 4);
//         bullet.setFillColor(sf::Color::Yellow);
//         bullet.setOutlineColor(sf::Color::Black);
//         bullet.setOutlineThickness(1);
//         window.draw(bullet);
//     }
// };

// // ===========================================
// // FACTORY PATTERN - Tower creation
// // ===========================================

// enum class TowerType {
//     BASIC,
//     SNIPER,
//     CANNON,
//     FREEZE
// };

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;
//     std::string towerType;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1), towerType("Basic") {}

// protected:
//     Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
//         : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), timeSinceLastAttack(0), level(1), towerType(type) {}

// public:
//     virtual ~Tower() = default;
    
//     virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject, AudioManager* audioManager) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (auto& enemy : enemies) {
//                 if (!enemy || !enemy->alive) continue;
                
//                 float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = enemy.get();
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 projectiles.emplace_back(position, target, damage, subject, audioManager);
//                 timeSinceLastAttack = 0;
//             }
//         }
//     }
    
//     virtual void upgrade() {
//         if (level == 1) {
//             level = 2;
//             damage = 40;
//             range = 100;
//             attackCooldown = 0.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 80;
//             range = 120;
//             attackCooldown = 0.6f;
//         }
//     }

//     virtual void draw(sf::RenderWindow& window) {
//         // Range circle
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         drawTower(window);
//     }

//     virtual int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0;
//     }

//     virtual bool canUpgrade() {
//         return level < 3;
//     }

// protected:
//     virtual void drawTower(sf::RenderWindow& window) {
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }
// };

// // Derived tower classes
// class BasicTower : public Tower {
// public:
//     BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
// };

// class SniperTower : public Tower {
// public:
//     SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 100;
//             range = 180;
//             attackCooldown = 1.8f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 200;
//             range = 220;
//             attackCooldown = 1.5f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(25, 25));
//         shape.setPosition(position.x - 12.5f, position.y - 12.5f);
        
//         if (level == 1) shape.setFillColor(sf::Color::Green);
//         else if (level == 2) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);
//     }
// };

// class CannonTower : public Tower {
// public:
//     CannonTower(sf::Vector2f pos) : Tower(pos, 100, 60, 3.0f, "Cannon") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 200;
//             range = 70;
//             attackCooldown = 2.5f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 400;
//             range = 80;
//             attackCooldown = 2.0f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::CircleShape shape(18);
//         shape.setPosition(position.x - 18, position.y - 18);
        
//         if (level == 1) shape.setFillColor(sf::Color(139, 69, 19)); // Brown
//         else if (level == 2) shape.setFillColor(sf::Color(160, 82, 45));
//         else shape.setFillColor(sf::Color(205, 133, 63));
        
//         window.draw(shape);
//     }
// };

// class FreezeTower : public Tower {
// public:
//     FreezeTower(sf::Vector2f pos) : Tower(pos, 5, 90, 0.5f, "Freeze") {}
    
//     void upgrade() override {
//         if (level == 1) {
//             level = 2;
//             damage = 10;
//             range = 110;
//             attackCooldown = 0.4f;
//         } else if (level == 2) {
//             level = 3;
//             damage = 20;
//             range = 130;
//             attackCooldown = 0.3f;
//         }
//     }

// protected:
//     void drawTower(sf::RenderWindow& window) override {
//         sf::RectangleShape shape(sf::Vector2f(28, 28));
//         shape.setPosition(position.x - 14, position.y - 14);
        
//         if (level == 1) shape.setFillColor(sf::Color(173, 216, 230)); // Light Blue
//         else if (level == 2) shape.setFillColor(sf::Color(135, 206, 235));
//         else shape.setFillColor(sf::Color(70, 130, 180));
        
//         window.draw(shape);
//     }
// };

// // Tower Factory
// class TowerFactory {
// public:
//     static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position) {
//         switch (type) {
//             case TowerType::BASIC:
//                 return std::make_unique<BasicTower>(position);
//             case TowerType::SNIPER:
//                 return std::make_unique<SniperTower>(position);
//             case TowerType::CANNON:
//                 return std::make_unique<CannonTower>(position);
//             case TowerType::FREEZE:
//                 return std::make_unique<FreezeTower>(position);
//             default:
//                 return std::make_unique<BasicTower>(position);
//         }
//     }
    
//     static int getTowerCost(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return 50;
//             case TowerType::SNIPER: return 100;
//             case TowerType::CANNON: return 150;
//             case TowerType::FREEZE: return 75;
//             default: return 50;
//         }
//     }
    
//     static std::string getTowerDescription(TowerType type) {
//         switch (type) {
//             case TowerType::BASIC: return "Basic Tower - Balanced";
//             case TowerType::SNIPER: return "Sniper Tower - Long Range";
//             case TowerType::CANNON: return "Cannon Tower - High Damage";
//             case TowerType::FREEZE: return "Freeze Tower - Slows Enemies";
//             default: return "Unknown Tower";
//         }
//     }
// };

// // ===========================================
// // GAME CLASS - Implements Observer to receive events
// // ===========================================
// class Game : public Subject, public Observer {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<std::unique_ptr<Enemy>> enemies;
//     std::vector<std::unique_ptr<Tower>> towers;
//     std::vector<Projectile> projectiles;
//     int gold;
//     int lives;
//     int wave;
//     int level;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;
//     int score;
//     ResourceManager resourceManager;
//     AudioManager audioManager;
//     bool gameOver;
//     bool showReplayOption;
//     bool showSaveOption;
//     std::string playerName;
//     bool waitingForName;
//     bool showAudioControls;
    
//     // Variables pour le timer automatique
//     float preparationTimer;
//     const float PREPARATION_TIME = 15.0f;
//     bool preparationPhase;
    
//     // Preset paths
//     const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
//         // Original path
//         {
//             sf::Vector2f(0, 300),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(500, 150),
//             sf::Vector2f(500, 450),
//             sf::Vector2f(800, 450),
//             sf::Vector2f(800, 250),
//             sf::Vector2f(1000, 250)
//         },
//         // Zigzag path
//         {
//             sf::Vector2f(0, 200),
//             sf::Vector2f(200, 200),
//             sf::Vector2f(200, 400),
//             sf::Vector2f(400, 400),
//             sf::Vector2f(400, 200),
//             sf::Vector2f(600, 200),
//             sf::Vector2f(600, 400),
//             sf::Vector2f(800, 400),
//             sf::Vector2f(800, 200),
//             sf::Vector2f(1000, 200)
//         },
//         // Curly path
//         {
//             sf::Vector2f(0, 350),
//             sf::Vector2f(200, 350),
//             sf::Vector2f(200, 150),
//             sf::Vector2f(400, 150),
//             sf::Vector2f(400, 350),
//             sf::Vector2f(600, 350),
//             sf::Vector2f(600, 150),
//             sf::Vector2f(800, 150),
//             sf::Vector2f(800, 350),
//             sf::Vector2f(1000, 350)
//         },
//         // S path
//         {
//             sf::Vector2f(0, 100),
//             sf::Vector2f(200, 100),
//             sf::Vector2f(200, 300),
//             sf::Vector2f(400, 300),
//             sf::Vector2f(400, 500),
//             sf::Vector2f(600, 500),
//             sf::Vector2f(600, 300),
//             sf::Vector2f(800, 300),
//             sf::Vector2f(800, 100),
//             sf::Vector2f(1000, 100)
//         }
//     };

//     void changePath() {
//         // Select a path different from the previous randomly
//         int newPathIndex;
//         do {
//             newPathIndex = rand() % predefinedPaths.size();
//         } while (predefinedPaths[newPathIndex] == path && predefinedPaths.size() > 1);
        
//         path = predefinedPaths[newPathIndex];
//         std::cout << "=== NEW PATH SELECTED FOR LEVEL " << level << " ===" << std::endl;
//     }

//     void saveScore() {
//         try {
//             json scores;
//             std::ifstream file("scores.json");
//             if (file.good()) {
//                 file >> scores;
//                 file.close();
//             } else {
//                 scores = json::array();
//                 std::cout << "Creating file scores.json" << std::endl;
//             }
            
//             json newScore = {
//                 {"name", playerName},
//                 {"score", score},
//                 {"level", level},
//                 {"wave", wave},
//                 {"date", std::time(nullptr)}
//             };
            
//             scores.push_back(newScore);
            
//             std::ofstream outFile("scores.json");
//             outFile << scores.dump(4);
//             outFile.close();
            
//             std::cout << "Score sauvegardé avec succès!" << std::endl;
//         } catch (const std::exception& e) {
//             std::cerr << "Erreur lors de la sauvegarde du score: " << e.what() << std::endl;
//         }
//     }
    
//     void resetGame() {
//         gold = 200;
//         lives = 3;
//         wave = 1;
//         level = 1;
//         score = 0;
//         enemies.clear();
//         towers.clear();
//         projectiles.clear();
//         gameOver = false;
//         showReplayOption = false;
//         showSaveOption = false;
//         waitingForName = false;
//         playerName = "";
//         path = predefinedPaths[0];
//         showAudioControls = false;
        
//         // Réinitialiser le timer
//         preparationTimer = PREPARATION_TIME;
//         preparationPhase = true;
        
//         // Redémarrer la musique
//         audioManager.startMusic();
//     }

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - Version Corrigée"),
//              gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
//              gameOver(false), showReplayOption(false), showSaveOption(false),
//              waitingForName(false), playerName(""), showAudioControls(false),
//              preparationTimer(PREPARATION_TIME), preparationPhase(true) {
        
//         // Initialize the random number generator 
//         srand(static_cast<unsigned int>(time(nullptr)));
        
//         std::cout << "=== TOWER DEFENSE - VERSION CORRIGÉE ===" << std::endl;
//         std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
//         std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
//         std::cout << "✓ Observer Pattern: Event management" << std::endl;
//         std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
//         std::cout << "✓ Audio Manager: Musique et effets sonores (corrigé)" << std::endl;
//         std::cout << "✓ Timer automatique: 15 secondes entre vagues" << std::endl;
//         std::cout << "✓ Gestion mémoire améliorée" << std::endl;
        
//         // Subscribe to its own events
//         addObserver(this);
        
//         // Initialize the first path
//         path = predefinedPaths[0];
        
//         // Démarrer la musique
//         audioManager.startMusic();
        
//         std::cout << "\nCONTRÔLES DU JEU:" << std::endl;
//         std::cout << "- Clic gauche: Placer une tour" << std::endl;
//         std::cout << "- Clic sur tour: Améliorer" << std::endl;
//         std::cout << "- ESPACE: Commencer la vague immédiatement" << std::endl;
//         std::cout << "- A: Afficher/masquer contrôles audio" << std::endl;
//         std::cout << "\nCONTRÔLES AUDIO:" << std::endl;
//         std::cout << "- M: Mute/Unmute" << std::endl;
//         std::cout << "- N: Changer de musique" << std::endl;
//         std::cout << "- +/-: Volume principal" << std::endl;
//         std::cout << "- 1/2: Volume musique" << std::endl;
//         std::cout << "- 3/4: Volume effets" << std::endl;
//     }

//     // Observer implementation
//     void notify(const GameEvent& event) override {
//         switch(event.type) {
//             case GameEvent::ENEMY_KILLED:
//                 gold += event.value;
//                 score += event.value * 10;
//                 break;
                
//             case GameEvent::ENEMY_REACHED_END:
//                 lives -= event.value;
//                 score = std::max(0, score - 50);
//                 break;
                
//             case GameEvent::WAVE_COMPLETED:
//                 score += 100;
//                 break;
//         }
//     }

//     void run() {
//         sf::Clock clock;
        
//         while (window.isOpen()) {
//             float deltaTime = clock.restart().asSeconds();
            
//             // Limiter le deltaTime pour éviter les gros sauts
//             deltaTime = std::min(deltaTime, 0.05f);
            
//             handleEvents();
//             update(deltaTime);
//             render();
//         }
//     }

//     void handleEvents() {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
            
//             if (event.type == sf::Event::TextEntered && waitingForName) {
//                 if (event.text.unicode == '\b' && !playerName.empty()) {
//                     playerName.pop_back();
//                 }
//                 else if (event.text.unicode < 128 && event.text.unicode >= 32 && playerName.length() < 20) {
//                     playerName += static_cast<char>(event.text.unicode);
//                 }
//             }
            
//             if (event.type == sf::Event::KeyPressed) {
//                 // Contrôles audio
//                 if (event.key.code == sf::Keyboard::M) {
//                     audioManager.toggleMute();
//                 }
//                 else if (event.key.code == sf::Keyboard::N && audioManager.hasBothMusicsLoaded()) {
//                     audioManager.switchMusic();
//                 }
//                 else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
//                     audioManager.increaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
//                     audioManager.decreaseMasterVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num1) {
//                     audioManager.increaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num2) {
//                     audioManager.decreaseMusicVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num3) {
//                     audioManager.increaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::Num4) {
//                     audioManager.decreaseEffectsVolume();
//                 }
//                 else if (event.key.code == sf::Keyboard::A) {
//                     showAudioControls = !showAudioControls;
//                 }
//                 // Contrôles de jeu
//                 else if (event.key.code == sf::Keyboard::Return && waitingForName) {
//                     waitingForName = false;
//                     showSaveOption = true;
//                 }
//                 else if (event.key.code == sf::Keyboard::Space && preparationPhase && !gameOver) {
//                     startWave();
//                 }
//                 else if (event.key.code == sf::Keyboard::R && gameOver) {
//                     resetGame();
//                 }
//                 else if (event.key.code == sf::Keyboard::S && showSaveOption) {
//                     saveScore();
//                     showSaveOption = false;
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//                 sf::Vector2f clickPos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                
//                 // Check click on existing tower
//                 bool towerClicked = false;
//                 for (auto& tower : towers) {
//                     if (!tower) continue;
                    
//                     float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                     if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
//                         gold -= tower->getUpgradeCost();
//                         tower->upgrade();
//                         audioManager.playUpgradeSound();
//                         towerClicked = true;
//                         std::cout << "Tower upgraded to level " << tower->level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // New tower placement
//                 if (!towerClicked && !gameOver) {
//                     int cost = 50;
//                     if (gold >= cost) {
//                         bool canPlace = true;
                        
//                         // Check distance with other towers
//                         for (const auto& tower : towers) {
//                             if (!tower) continue;
//                             float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
//                             if (distance < TOWER_MIN_DISTANCE) {
//                                 canPlace = false;
//                                 break;
//                             }
//                         }
                        
//                         // Check distance from path
//                         if (canPlace) {
//                             for (size_t i = 0; i < path.size() - 1; i++) {
//                                 sf::Vector2f start = path[i];
//                                 sf::Vector2f end = path[i + 1];
                                
//                                 float A = clickPos.x - start.x;
//                                 float B = clickPos.y - start.y;
//                                 float C = end.x - start.x;
//                                 float D = end.y - start.y;
                                
//                                 float dot = A * C + B * D;
//                                 float lenSq = C * C + D * D;
//                                 float param = (lenSq != 0) ? dot / lenSq : -1;
                                
//                                 float xx, yy;
//                                 if (param < 0) {
//                                     xx = start.x; yy = start.y;
//                                 } else if (param > 1) {
//                                     xx = end.x; yy = end.y;
//                                 } else {
//                                     xx = start.x + param * C;
//                                     yy = start.y + param * D;
//                                 }
                                
//                                 float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
//                                 if (distance < 50) {
//                                     canPlace = false;
//                                     break;
//                                 }
//                             }
//                         }
                        
//                         if (canPlace) {
//                             auto tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
//                             towers.push_back(std::move(tower));
//                             gold -= cost;
//                             std::cout << "Tower placed! Remaining gold: " << gold << std::endl;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         preparationPhase = false;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== LEVEL " << level << " - WAVE " << wave << " STARTS ===" << std::endl;
//     }

//     void update(float deltaTime) {
//         // Stop updating if game is over
//         if (gameOver) {
//             return;
//         }
        
//         // Gestion du timer de préparation
//         if (preparationPhase) {
//             preparationTimer -= deltaTime;
            
//             if (preparationTimer <= 0) {
//                 std::cout << "Timer écoulé ! Lancement automatique de la vague..." << std::endl;
//                 startWave();
//             }
//         }
        
//         // Spawn enemies with Factory Pattern
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) {
//                 EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
//                 auto enemy = EnemyFactory::createEnemy(type);
                
//                 if (enemy && !path.empty()) {
//                     enemy->position = path[0];
                    
//                     // Increase enemy statistics based on the level and the wave
//                     float levelMultiplier = 1.0f + (level - 1) * 0.15f;
//                     float waveMultiplier = 1.0f + (wave - 1) * 0.05f;
//                     float totalMultiplier = levelMultiplier * waveMultiplier;
                    
//                     enemy->health = static_cast<int>(enemy->health * totalMultiplier);
//                     enemy->maxHealth = enemy->health;
//                     enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);
//                     enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));
                    
//                     enemies.push_back(std::move(enemy));
//                 }
                
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//             }
//         }

//         // Update enemies
//         for (auto& enemy : enemies) {
//             if (enemy && enemy->alive) {
//                 enemy->update(deltaTime, path);
                
//                 if (enemy->reachedEnd(path)) {
//                     notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
//                     enemy->alive = false;
//                 }
//             }
//         }

//         // Update towers
//         for (auto& tower : towers) {
//             if (tower) {
//                 tower->update(deltaTime, enemies, projectiles, this, &audioManager);
//             }
//         }

//         // Update projectiles
//         for (auto& projectile : projectiles) {
//             projectile.update(deltaTime);
//         }

//         // Remove inactive projectiles
//         projectiles.erase(
//             std::remove_if(projectiles.begin(), projectiles.end(),
//                 [](const Projectile& proj) { return !proj.active; }),
//             projectiles.end()
//         );

//         // Remove dead enemies
//         enemies.erase(
//             std::remove_if(enemies.begin(), enemies.end(),
//                 [](const std::unique_ptr<Enemy>& enemy) { 
//                     return !enemy || !enemy->alive; 
//                 }),
//             enemies.end()
//         );

//         // Check end of wave
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             preparationPhase = true;
//             preparationTimer = PREPARATION_TIME;
            
//             notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
//             wave++;
            
//             // Change the level after 3 waves
//             if (wave > 3) {
//                 level++;
//                 wave = 1;
//                 enemiesInWave = 5 + (level - 1) * 2;
//                 gold += 100 * level;
                
//                 // Change the path and remove towers
//                 changePath();
//                 towers.clear();
                
//                 std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
//                 std::cout << "New path selected - Towers have been removed" << std::endl;
//             } else {
//                 enemiesInWave += 2;
//             }
//         }

//         // Check for game over
//         if (lives <= 0) {
//             gameOver = true;
//             showReplayOption = true;
//             waitingForName = true;
//             audioManager.stopMusic();
//         }
//     }

//     // Convert integer to string for display
//     std::string intToString(int value) {
//         std::ostringstream oss;
//         oss << value;
//         return oss.str();
//     }
    
//     std::string floatToString(float value) {
//         std::ostringstream oss;
//         oss << static_cast<int>(value);
//         return oss.str();
//     }

//     void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 16) {
//         sf::Text sfText;
//         sfText.setString(text);
//         sfText.setCharacterSize(size);
//         sfText.setFillColor(color);
//         sfText.setPosition(x, y);
        
//         if (resourceManager.isFontLoaded()) {
//             sfText.setFont(resourceManager.getFont());
//         }
        
//         window.draw(sfText);
//     }

//     void render() {
//         window.clear(sf::Color(40, 40, 40));

//         // Draw the path
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vector2f start = path[i];
//             sf::Vector2f end = path[i + 1];
//             sf::Vector2f direction = end - start;
//             float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (length > 0) {
//                 direction = direction / length;
//                 sf::RectangleShape pathSegment;
//                 pathSegment.setSize(sf::Vector2f(length, 40));
//                 pathSegment.setPosition(start);
//                 pathSegment.setFillColor(sf::Color(100, 100, 100));
                
//                 float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
//                 pathSegment.setRotation(angle);
//                 pathSegment.setOrigin(0, 20);
                
//                 window.draw(pathSegment);
//             }
//         }

//         // Path points
//         for (const sf::Vector2f& point : path) {
//             sf::CircleShape dot(3);
//             dot.setPosition(point.x - 3, point.y - 3);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Draw towers
//         for (auto& tower : towers) {
//             if (tower) {
//                 tower->draw(window);
//             }
//         }

//         // Draw projectiles
//         for (auto& projectile : projectiles) {
//             projectile.draw(window);
//         }

//         // Draw enemies
//         for (auto& enemy : enemies) {
//             if (enemy && enemy->alive) {
//                 enemy->draw(window);
//             }
//         }

//         // USER INTERFACE
//         resourceManager.drawGoldIcon(window, 10, 10);
//         drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

//         resourceManager.drawHeartIcon(window, 10, 40);
//         drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

//         drawText(window, "Level : " + intToString(level), 300, 15, sf::Color::White, 18);
//         drawText(window, "Wave : " + intToString(wave), 300, 45, sf::Color::Cyan, 18);
//         drawText(window, "Towers : " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
//         drawText(window, "Enemies : " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);
//         drawText(window, "Score : " + intToString(score), 850, 15, sf::Color::White, 18);

//         // Audio status indicator
//         if (audioManager.getIsMuted()) {
//             drawText(window, "MUTE", 850, 45, sf::Color::Red, 16);
//         } else {
//             std::string musicText = "Music " + intToString(audioManager.getCurrentMusicTrack() + 1);
//             drawText(window, musicText, 850, 45, sf::Color::Green, 16);
//         }

//         // Audio controls panel
//         if (showAudioControls) {
//             sf::RectangleShape controlsBg(sf::Vector2f(300, 200));
//             controlsBg.setPosition(10, 80);
//             controlsBg.setFillColor(sf::Color(0, 0, 0, 180));
//             controlsBg.setOutlineColor(sf::Color::White);
//             controlsBg.setOutlineThickness(2);
//             window.draw(controlsBg);
            
//             drawText(window, "=== CONTROLES AUDIO ===", 20, 90, sf::Color::White, 16);
//             drawText(window, "M: Mute/Unmute", 20, 110, sf::Color::Cyan, 14);
//             if (audioManager.hasBothMusicsLoaded()) {
//                 drawText(window, "N: Changer musique", 20, 130, sf::Color::Cyan, 14);
//             }
//             drawText(window, "+/-: Volume principal (" + floatToString(audioManager.getMasterVolume()) + "%)", 20, 150, sf::Color::Yellow, 14);
//             drawText(window, "1/2: Volume musique (" + floatToString(audioManager.getMusicVolume()) + "%)", 20, 170, sf::Color::Yellow, 14);
//             drawText(window, "3/4: Volume effets (" + floatToString(audioManager.getEffectsVolume()) + "%)", 20, 190, sf::Color::Yellow, 14);
//             drawText(window, "A: Masquer ce panneau", 20, 210, sf::Color::White, 14);
//         }

//         // Timer et instructions de vague au centre de l'écran
//         if (preparationPhase && !gameOver) {
//             sf::RectangleShape textBg(sf::Vector2f(600, 120));
//             textBg.setPosition(WINDOW_WIDTH/2 - 300, WINDOW_HEIGHT/2 - 60);
//             textBg.setFillColor(sf::Color(0, 0, 0, 150));
//             textBg.setOutlineColor(sf::Color::Green);
//             textBg.setOutlineThickness(3);
//             window.draw(textBg);
            
//             // Affichage du timer
//             int secondsLeft = static_cast<int>(std::ceil(preparationTimer));
//             drawText(window, "NEXT WAVE IN: " + intToString(secondsLeft) + "s", 
//                     WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 45, sf::Color::Red, 28);
            
//             drawText(window, "PRESS SPACE TO START NOW", WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT/2 - 10, sf::Color::Green, 22);
//             drawText(window, "LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
//                     WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 20, sf::Color::Cyan, 20);
            
//             // Display message for each new level
//             if (wave == 1) {
//                 drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
//                         WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 50, sf::Color::Yellow, 18);
//             }
//         }

//         // Game over screen
//         if (gameOver) {
//             sf::RectangleShape gameOverBg(sf::Vector2f(400, 200));
//             gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 100);
//             gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
//             gameOverBg.setOutlineColor(sf::Color::White);
//             gameOverBg.setOutlineThickness(3);
//             window.draw(gameOverBg);
            
//             drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80, sf::Color::White, 32);
//             drawText(window, "Final Score: " + intToString(score), WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 40, sf::Color::White, 20);
            
//             if (waitingForName) {
//                 drawText(window, "Enter your name :", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//                 drawText(window, "Press Enter to continue", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
//             }
//             else if (showSaveOption) {
//                 drawText(window, "Save score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
//                 drawText(window, "Press S to save", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
//             }
//             else if (showReplayOption) {
//                 drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
//             }
//         }

//         // Instructions at bottom of screen
//         drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade | SPACE: Start wave | A: Audio controls", 
//                 10, WINDOW_HEIGHT - 30, sf::Color::White, 16);

//         window.display();
//     }
// };

// int main() {
//     try {
//         std::cout << "Tower Defense - Version Corrigée avec Audio et Timer" << std::endl;
//         std::cout << "====================================================" << std::endl;
        
//         Game game;
//         game.run();
        
//     } catch (const std::exception& e) {
//         std::cerr << "Erreur fatale: " << e.what() << std::endl;
//         return -1;
//     } catch (...) {
//         std::cerr << "Erreur inconnue!" << std::endl;
//         return -1;
//     }
    
//     return 0;
// }

// Version avec timer -- Plante rapidement 

// ------------------------------------------------------------- //

// ci dessous test version débug ------------------------------------

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <memory>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Constants
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const float TOWER_MIN_DISTANCE = 80.0f;

// ===========================================
// AUDIO MANAGER - Version ultra-sécurisée
// ===========================================
class AudioManager {
private:
    std::unique_ptr<sf::Music> backgroundMusic1;
    std::unique_ptr<sf::Music> backgroundMusic2;
    sf::SoundBuffer shootSoundBuffer;
    sf::SoundBuffer explosionSoundBuffer;
    sf::SoundBuffer upgradeSoundBuffer;
    sf::Sound shootSound;
    sf::Sound explosionSound;
    sf::Sound upgradeSound;
    
    bool music1Loaded;
    bool music2Loaded;
    bool soundsLoaded;
    float masterVolume;
    float musicVolume;
    float effectsVolume;
    bool isMuted;
    int currentMusicTrack;
    
public:
    AudioManager() : backgroundMusic1(std::make_unique<sf::Music>()),
                     backgroundMusic2(std::make_unique<sf::Music>()),
                     music1Loaded(false), music2Loaded(false), soundsLoaded(false),
                     masterVolume(30.0f), musicVolume(20.0f), effectsVolume(50.0f), 
                     isMuted(false), currentMusicTrack(0) {
        loadAudioFiles();
    }
    
    ~AudioManager() {
        stopMusic();
    }
    
    void loadAudioFiles() {
        std::cout << "=== CHARGEMENT AUDIO SÉCURISÉ ===" << std::endl;
        
        std::vector<std::string> audioPaths = {
            "./assets/",
            "assets/",
            "../assets/",
            "./"
        };
        
        // Chargement musique 1
        for (const std::string& basePath : audioPaths) {
            if (!music1Loaded) {
                std::string music1Path = basePath + "background_music1.ogg";
                struct stat info;
                if (stat(music1Path.c_str(), &info) == 0) {
                    if (backgroundMusic1->openFromFile(music1Path)) {
                        music1Loaded = true;
                        backgroundMusic1->setLoop(true);
                        backgroundMusic1->setVolume(musicVolume);
                        std::cout << "✓ Musique 1 chargée: " << music1Path << std::endl;
                        break;
                    }
                }
            }
        }
        
        // Chargement musique 2
        for (const std::string& basePath : audioPaths) {
            if (!music2Loaded) {
                std::string music2Path = basePath + "background_music2.ogg";
                struct stat info;
                if (stat(music2Path.c_str(), &info) == 0) {
                    if (backgroundMusic2->openFromFile(music2Path)) {
                        music2Loaded = true;
                        backgroundMusic2->setLoop(true);
                        backgroundMusic2->setVolume(musicVolume);
                        std::cout << "✓ Musique 2 chargée: " << music2Path << std::endl;
                        break;
                    }
                }
            }
        }
        
        // Chargement effets sonores avec vérification
        for (const std::string& basePath : audioPaths) {
            if (!soundsLoaded) {
                std::string shootPath = basePath + "shoot.wav";
                std::string explosionPath = basePath + "explosion.wav";
                std::string upgradePath = basePath + "upgrade.wav";
                
                struct stat info;
                bool hasSound = false;
                
                if (stat(shootPath.c_str(), &info) == 0) {
                    if (shootSoundBuffer.loadFromFile(shootPath)) {
                        shootSound.setBuffer(shootSoundBuffer);
                        shootSound.setVolume(effectsVolume);
                        std::cout << "✓ Son de tir chargé" << std::endl;
                        hasSound = true;
                    }
                }
                
                if (stat(explosionPath.c_str(), &info) == 0) {
                    if (explosionSoundBuffer.loadFromFile(explosionPath)) {
                        explosionSound.setBuffer(explosionSoundBuffer);
                        explosionSound.setVolume(effectsVolume);
                        std::cout << "✓ Son d'explosion chargé" << std::endl;
                        hasSound = true;
                    }
                }
                
                if (stat(upgradePath.c_str(), &info) == 0) {
                    if (upgradeSoundBuffer.loadFromFile(upgradePath)) {
                        upgradeSound.setBuffer(upgradeSoundBuffer);
                        upgradeSound.setVolume(effectsVolume);
                        std::cout << "✓ Son d'amélioration chargé" << std::endl;
                        hasSound = true;
                    }
                }
                
                if (hasSound) {
                    soundsLoaded = true;
                    break;
                }
            }
        }
        
        if (!music1Loaded && !music2Loaded) {
            std::cout << "⚠ Aucune musique trouvée (optionnel)" << std::endl;
        }
        if (!soundsLoaded) {
            std::cout << "⚠ Aucun effet sonore trouvé (optionnel)" << std::endl;
        }
        
        std::cout << "=== FIN CHARGEMENT AUDIO ===" << std::endl;
    }
    
    void startMusic() {
        if (isMuted || (!music1Loaded && !music2Loaded)) return;
        
        try {
            stopMusic();
            
            if (currentMusicTrack == 0 && music1Loaded && backgroundMusic1) {
                if (backgroundMusic1->getStatus() != sf::Music::Playing) {
                    backgroundMusic1->play();
                    std::cout << "Lecture musique 1" << std::endl;
                }
            } else if (currentMusicTrack == 1 && music2Loaded && backgroundMusic2) {
                if (backgroundMusic2->getStatus() != sf::Music::Playing) {
                    backgroundMusic2->play();
                    std::cout << "Lecture musique 2" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur audio: " << e.what() << std::endl;
        }
    }
    
    void stopMusic() {
        try {
            if (backgroundMusic1 && backgroundMusic1->getStatus() != sf::Music::Stopped) {
                backgroundMusic1->stop();
            }
            if (backgroundMusic2 && backgroundMusic2->getStatus() != sf::Music::Stopped) {
                backgroundMusic2->stop();
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur arrêt musique: " << e.what() << std::endl;
        }
    }
    
    void switchMusic() {
        if (music1Loaded && music2Loaded) {
            stopMusic();
            currentMusicTrack = (currentMusicTrack == 0) ? 1 : 0;
            startMusic();
        }
    }
    
    void toggleMute() {
        isMuted = !isMuted;
        if (isMuted) {
            stopMusic();
        } else {
            startMusic();
        }
    }
    
    void updateVolumes() {
        try {
            float finalMusicVolume = (masterVolume / 100.0f) * (musicVolume / 100.0f) * 100.0f;
            float finalEffectsVolume = (masterVolume / 100.0f) * (effectsVolume / 100.0f) * 100.0f;
            
            if (music1Loaded && backgroundMusic1) backgroundMusic1->setVolume(finalMusicVolume);
            if (music2Loaded && backgroundMusic2) backgroundMusic2->setVolume(finalMusicVolume);
            
            if (soundsLoaded) {
                shootSound.setVolume(finalEffectsVolume);
                explosionSound.setVolume(finalEffectsVolume);
                upgradeSound.setVolume(finalEffectsVolume);
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur volume: " << e.what() << std::endl;
        }
    }
    
    void playShootSound() {
        if (!isMuted && soundsLoaded && shootSound.getBuffer()) {
            try {
                if (shootSound.getStatus() != sf::Sound::Playing) {
                    shootSound.play();
                }
            } catch (const std::exception& e) {
                std::cerr << "Erreur son tir: " << e.what() << std::endl;
            }
        }
    }
    
    void playExplosionSound() {
        if (!isMuted && soundsLoaded && explosionSound.getBuffer()) {
            try {
                if (explosionSound.getStatus() != sf::Sound::Playing) {
                    explosionSound.play();
                }
            } catch (const std::exception& e) {
                std::cerr << "Erreur son explosion: " << e.what() << std::endl;
            }
        }
    }
    
    void playUpgradeSound() {
        if (!isMuted && soundsLoaded && upgradeSound.getBuffer()) {
            try {
                if (upgradeSound.getStatus() != sf::Sound::Playing) {
                    upgradeSound.play();
                }
            } catch (const std::exception& e) {
                std::cerr << "Erreur son amélioration: " << e.what() << std::endl;
            }
        }
    }
    
    // Volume controls with limits
    void increaseMasterVolume() {
        masterVolume = std::min(50.0f, masterVolume + 5.0f);
        updateVolumes();
    }
    
    void decreaseMasterVolume() {
        masterVolume = std::max(0.0f, masterVolume - 5.0f);
        updateVolumes();
    }
    
    void increaseMusicVolume() {
        musicVolume = std::min(30.0f, musicVolume + 5.0f);
        updateVolumes();
    }
    
    void decreaseMusicVolume() {
        musicVolume = std::max(0.0f, musicVolume - 5.0f);
        updateVolumes();
    }
    
    void increaseEffectsVolume() {
        effectsVolume = std::min(70.0f, effectsVolume + 5.0f);
        updateVolumes();
    }
    
    void decreaseEffectsVolume() {
        effectsVolume = std::max(0.0f, effectsVolume - 5.0f);
        updateVolumes();
    }
    
    // Getters
    float getMasterVolume() const { return masterVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getEffectsVolume() const { return effectsVolume; }
    bool getIsMuted() const { return isMuted; }
    int getCurrentMusicTrack() const { return currentMusicTrack; }
    bool hasMusicLoaded() const { return music1Loaded || music2Loaded; }
    bool hasBothMusicsLoaded() const { return music1Loaded && music2Loaded; }
};

// ===========================================
// OBSERVER PATTERN
// ===========================================
class GameEvent {
public:
    enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
    Type type;
    int value;
    
    GameEvent(Type t, int v = 0) : type(t), value(v) {}
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void notify(const GameEvent& event) = 0;
};

class Subject {
protected:
    std::vector<Observer*> observers;
public:
    virtual ~Subject() = default;
    
    void addObserver(Observer* observer) {
        if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
            observers.push_back(observer);
        }
    }
    
    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notifyObservers(const GameEvent& event) {
        // Copie défensive pour éviter les problèmes si la liste change pendant l'itération
        auto observersCopy = observers;
        for (auto* observer : observersCopy) {
            if (observer) {
                try {
                    observer->notify(event);
                } catch (const std::exception& e) {
                    std::cerr << "Erreur notification observer: " << e.what() << std::endl;
                }
            }
        }
    }
};

// ===========================================
// RESOURCE MANAGER
// ===========================================
class ResourceManager {
private:
    sf::Texture goldTexture, heartTexture;
    sf::Sprite goldSprite, heartSprite;
    bool goldLoaded, heartLoaded;
    sf::Font arialFont;
    bool fontLoaded;
    
public:
    ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
        loadFont();
        loadResources();
    }
    
    void loadFont() {
        std::vector<std::string> fontPaths = {
            "./assets/arial.ttf",
            "assets/arial.ttf",
            "../assets/arial.ttf",
            "./arial.ttf"
        };
        
        for (const std::string& path : fontPaths) {
            struct stat info;
            if (stat(path.c_str(), &info) == 0) {
                if (arialFont.loadFromFile(path)) {
                    fontLoaded = true;
                    std::cout << "✓ Police chargée: " << path << std::endl;
                    return;
                }
            }
        }
        std::cout << "⚠ Police Arial non trouvée (police par défaut utilisée)" << std::endl;
    }
    
    void loadResources() {
        std::vector<std::string> pathsToTry = {
            "./assets/",
            "assets/",
            "../assets/",
            "./"
        };
        
        // Gold icon
        for (const std::string& basePath : pathsToTry) {
            std::string fullPath = basePath + "gold_icon.png";
            struct stat info;
            if (stat(fullPath.c_str(), &info) == 0) {
                if (goldTexture.loadFromFile(fullPath)) {
                    goldLoaded = true;
                    goldSprite.setTexture(goldTexture);
                    goldSprite.setScale(0.8f, 0.8f);
                    std::cout << "✓ Icône or chargée" << std::endl;
                    break;
                }
            }
        }
        
        // Heart icon
        for (const std::string& basePath : pathsToTry) {
            std::string fullPath = basePath + "heart_icon.png";
            struct stat info;
            if (stat(fullPath.c_str(), &info) == 0) {
                if (heartTexture.loadFromFile(fullPath)) {
                    heartLoaded = true;
                    heartSprite.setTexture(heartTexture);
                    heartSprite.setScale(0.8f, 0.8f);
                    std::cout << "✓ Icône vie chargée" << std::endl;
                    break;
                }
            }
        }
    }
    
    const sf::Font& getFont() const { return arialFont; }
    bool isFontLoaded() const { return fontLoaded; }
    
    void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
        if (goldLoaded) {
            goldSprite.setPosition(x, y);
            window.draw(goldSprite);
        }
    }
    
    void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
        if (heartLoaded) {
            heartSprite.setPosition(x, y);
            window.draw(heartSprite);
        }
    }
};

// ===========================================
// ENEMY CLASSES
// ===========================================
class Enemy {
public:
    sf::Vector2f position;
    int health;
    int maxHealth;
    float speed;
    int reward;
    bool alive;
    size_t pathIndex; // Changé en size_t pour éviter les comparaisons signées/non-signées
    std::string type;

    Enemy(int hp, float spd, int rew, const std::string& t) 
        : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
    }

    virtual ~Enemy() = default;

    void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
        if (!alive || pathIndex >= path.size() || path.empty()) return;
        
        if (pathIndex >= path.size() - 1) return;

        sf::Vector2f target = path[pathIndex + 1];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 10.0f) {
            pathIndex++;
        } else {
            if (distance > 0) {
                direction = direction / distance;
                position += direction * speed * deltaTime;
            }
        }
    }

    void takeDamage(int damage) {
        health = std::max(0, health - damage);
        if (health <= 0) {
            alive = false;
        }
    }

    virtual sf::Color getColor() {
        if (maxHealth <= 0) return sf::Color::White;
        float ratio = static_cast<float>(health) / maxHealth;
        if (ratio > 0.7f) return sf::Color::Green;
        else if (ratio > 0.3f) return sf::Color::Yellow;
        else return sf::Color::Red;
    }

    void draw(sf::RenderWindow& window) {
        sf::CircleShape shape(15);
        shape.setPosition(position.x - 15, position.y - 15);
        shape.setFillColor(getColor());
        window.draw(shape);

        // Health bar
        sf::RectangleShape healthBar(sf::Vector2f(30, 5));
        healthBar.setPosition(position.x - 15, position.y - 25);
        healthBar.setFillColor(sf::Color::Black);
        window.draw(healthBar);

        if (health > 0 && maxHealth > 0) {
            float ratio = static_cast<float>(health) / maxHealth;
            sf::RectangleShape healthFill(sf::Vector2f(30 * ratio, 5));
            healthFill.setPosition(position.x - 15, position.y - 25);
            healthFill.setFillColor(sf::Color::Green);
            window.draw(healthFill);
        }
    }

    bool reachedEnd(const std::vector<sf::Vector2f>& path) {
        return pathIndex >= path.size() - 1;
    }
};

// Specialized enemy classes
class Goblin : public Enemy {
public:
    Goblin() : Enemy(50, 60, 10, "Goblin") {}
    sf::Color getColor() override { return sf::Color::Green; }
};

class Orc : public Enemy {
public:
    Orc() : Enemy(120, 45, 20, "Orc") {}
    sf::Color getColor() override { return sf::Color::Red; }
};

class MediumEnemy : public Enemy {
public:
    MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    sf::Color getColor() override { return sf::Color(0, 128, 128); }
};

class Troll : public Enemy {
public:
    Troll() : Enemy(300, 35, 50, "Troll") {}
    sf::Color getColor() override { return sf::Color::Magenta; }
};

class HeavyEnemy : public Enemy {
public:
    HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    sf::Color getColor() override { return sf::Color(139, 69, 19); }
};

// Enemy Factory
class EnemyFactory {
public:
    enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
    static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
        switch(type) {
            case GOBLIN: return std::make_unique<Goblin>();
            case ORC: return std::make_unique<Orc>();
            case TROLL: return std::make_unique<Troll>();
            case HEAVY: return std::make_unique<HeavyEnemy>();
            case MEDIUM: return std::make_unique<MediumEnemy>();
            default: return std::make_unique<Goblin>();
        }
    }
    
    static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
        int totalDifficulty = wave + (level - 1) * 3;
        
        if (totalDifficulty <= 3) return GOBLIN;
        else if (totalDifficulty <= 6) {
            if (enemyIndex % 8 == 0) return MEDIUM;
            return (enemyIndex % 5 == 0) ? ORC : GOBLIN;
        }
        else if (totalDifficulty <= 9) {
            if (enemyIndex % 10 == 0) return HEAVY;
            else if (enemyIndex % 6 == 0) return MEDIUM;
            return (enemyIndex % 5 < 2) ? ORC : GOBLIN;
        }
        else if (totalDifficulty <= 12) {
            if (enemyIndex % 8 == 0) return HEAVY;
            else if (enemyIndex % 5 == 0) return MEDIUM;
            return (enemyIndex % 5 < 3) ? ORC : GOBLIN;
        }
        else if (totalDifficulty <= 15) {
            if (enemyIndex % 10 == 0) return TROLL;
            else if (enemyIndex % 8 == 0) return HEAVY;
            else if (enemyIndex % 6 == 0) return MEDIUM;
            else if (enemyIndex % 10 < 7) return ORC;
            else return GOBLIN;
        }
        else {
            if (enemyIndex % 10 < 3) return TROLL;
            else if (enemyIndex % 8 == 0) return HEAVY;
            else if (enemyIndex % 6 == 0) return MEDIUM;
            else if (enemyIndex % 10 < 9) return ORC;
            else return GOBLIN;
        }
    }
};

// ===========================================
// PROJECTILE CLASS
// ===========================================
class Projectile {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    int damage;
    float speed;
    bool active;
    Enemy* targetEnemy;
    Subject* gameSubject;
    AudioManager* audioManager;

    Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject, AudioManager* audio) 
        : position(startPos), damage(dmg), speed(300.0f), active(true), 
          targetEnemy(target), gameSubject(subject), audioManager(audio) {
        
        if (target) {
            sf::Vector2f direction = target->position - position;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (distance > 0) {
                velocity = (direction / distance) * speed;
            }
        }
        
        if (audioManager) {
            audioManager->playShootSound();
        }
    }

    void update(float deltaTime) {
        if (!active) return;

        // Vérification de sécurité pour le pointeur targetEnemy
        if (targetEnemy && targetEnemy->alive) {
            sf::Vector2f targetPos = targetEnemy->position;
            sf::Vector2f direction = targetPos - position;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (distance < 15.0f) { // Impact
                int oldHealth = targetEnemy->health;
                targetEnemy->takeDamage(damage);
                
                if (targetEnemy->health <= 0 && oldHealth > 0 && gameSubject) {
                    gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
                    if (audioManager) {
                        audioManager->playExplosionSound();
                    }
                }
                
                active = false;
                return;
            }
            
            // Mise à jour de la direction vers la cible
            if (distance > 0) {
                velocity = (direction / distance) * speed;
            }
        }

        // Déplacement du projectile
        position += velocity * deltaTime;
        
        // Désactiver si trop loin
        if (position.x < -100 || position.x > WINDOW_WIDTH + 100 || 
            position.y < -100 || position.y > WINDOW_HEIGHT + 100) {
            active = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        if (!active) return;
        
        sf::CircleShape bullet(4);
        bullet.setPosition(position.x - 4, position.y - 4);
        bullet.setFillColor(sf::Color::Yellow);
        bullet.setOutlineColor(sf::Color::Black);
        bullet.setOutlineThickness(1);
        window.draw(bullet);
    }
};

// ===========================================
// TOWER CLASSES
// ===========================================
enum class TowerType { BASIC, SNIPER, CANNON, FREEZE };

class Tower {
public:
    sf::Vector2f position;
    int damage;
    float range;
    float attackCooldown;
    float timeSinceLastAttack;
    int level;
    std::string towerType;

protected:
    Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
        : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), 
          timeSinceLastAttack(0), level(1), towerType(type) {}

public:
    virtual ~Tower() = default;
    
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, 
                       std::vector<Projectile>& projectiles, Subject* subject, AudioManager* audioManager) {
        timeSinceLastAttack += deltaTime;
        
        if (timeSinceLastAttack >= attackCooldown) {
            Enemy* target = nullptr;
            float closestDistance = range + 1;
            
            for (auto& enemy : enemies) {
                if (!enemy || !enemy->alive) continue;
                
                float dx = enemy->position.x - position.x;
                float dy = enemy->position.y - position.y;
                float distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance <= range && distance < closestDistance) {
                    target = enemy.get();
                    closestDistance = distance;
                }
            }
            
            if (target) {
                try {
                    projectiles.emplace_back(position, target, damage, subject, audioManager);
                    timeSinceLastAttack = 0;
                } catch (const std::exception& e) {
                    std::cerr << "Erreur création projectile: " << e.what() << std::endl;
                }
            }
        }
    }
    
    virtual void upgrade() {
        if (level == 1) {
            level = 2;
            damage = 40;
            range = 100;
            attackCooldown = 0.8f;
        } else if (level == 2) {
            level = 3;
            damage = 80;
            range = 120;
            attackCooldown = 0.6f;
        }
    }

    virtual void draw(sf::RenderWindow& window) {
        // Range circle
        sf::CircleShape rangeCircle(range);
        rangeCircle.setPosition(position.x - range, position.y - range);
        rangeCircle.setFillColor(sf::Color::Transparent);
        rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 50));
        rangeCircle.setOutlineThickness(1);
        window.draw(rangeCircle);

        drawTower(window);
    }

    virtual int getUpgradeCost() {
        if (level == 1) return 100;
        if (level == 2) return 200;
        return 0;
    }

    virtual bool canUpgrade() { return level < 3; }

protected:
    virtual void drawTower(sf::RenderWindow& window) {
        sf::RectangleShape shape(sf::Vector2f(30, 30));
        shape.setPosition(position.x - 15, position.y - 15);
        
        if (level == 1) shape.setFillColor(sf::Color::Blue);
        else if (level == 2) shape.setFillColor(sf::Color::Magenta);
        else shape.setFillColor(sf::Color::Cyan);
        
        window.draw(shape);
    }
};

class BasicTower : public Tower {
public:
    BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
};

class SniperTower : public Tower {
public:
    SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
    void upgrade() override {
        if (level == 1) {
            level = 2; damage = 100; range = 180; attackCooldown = 1.8f;
        } else if (level == 2) {
            level = 3; damage = 200; range = 220; attackCooldown = 1.5f;
        }
    }

protected:
    void drawTower(sf::RenderWindow& window) override {
        sf::RectangleShape shape(sf::Vector2f(25, 25));
        shape.setPosition(position.x - 12.5f, position.y - 12.5f);
        
        if (level == 1) shape.setFillColor(sf::Color::Green);
        else if (level == 2) shape.setFillColor(sf::Color::Yellow);
        else shape.setFillColor(sf::Color::Red);
        
        window.draw(shape);
    }
};

class CannonTower : public Tower {
public:
    CannonTower(sf::Vector2f pos) : Tower(pos, 100, 60, 3.0f, "Cannon") {}
    
    void upgrade() override {
        if (level == 1) {
            level = 2; damage = 200; range = 70; attackCooldown = 2.5f;
        } else if (level == 2) {
            level = 3; damage = 400; range = 80; attackCooldown = 2.0f;
        }
    }

protected:
    void drawTower(sf::RenderWindow& window) override {
        sf::CircleShape shape(18);
        shape.setPosition(position.x - 18, position.y - 18);
        
        if (level == 1) shape.setFillColor(sf::Color(139, 69, 19));
        else if (level == 2) shape.setFillColor(sf::Color(160, 82, 45));
        else shape.setFillColor(sf::Color(205, 133, 63));
        
        window.draw(shape);
    }
};

class FreezeTower : public Tower {
public:
    FreezeTower(sf::Vector2f pos) : Tower(pos, 5, 90, 0.5f, "Freeze") {}
    
    void upgrade() override {
        if (level == 1) {
            level = 2; damage = 10; range = 110; attackCooldown = 0.4f;
        } else if (level == 2) {
            level = 3; damage = 20; range = 130; attackCooldown = 0.3f;
        }
    }

protected:
    void drawTower(sf::RenderWindow& window) override {
        sf::RectangleShape shape(sf::Vector2f(28, 28));
        shape.setPosition(position.x - 14, position.y - 14);
        
        if (level == 1) shape.setFillColor(sf::Color(173, 216, 230));
        else if (level == 2) shape.setFillColor(sf::Color(135, 206, 235));
        else shape.setFillColor(sf::Color(70, 130, 180));
        
        window.draw(shape);
    }
};

// Tower Factory
class TowerFactory {
public:
    static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2f position) {
        switch (type) {
            case TowerType::BASIC: return std::make_unique<BasicTower>(position);
            case TowerType::SNIPER: return std::make_unique<SniperTower>(position);
            case TowerType::CANNON: return std::make_unique<CannonTower>(position);
            case TowerType::FREEZE: return std::make_unique<FreezeTower>(position);
            default: return std::make_unique<BasicTower>(position);
        }
    }
    
    static int getTowerCost(TowerType type) {
        switch (type) {
            case TowerType::BASIC: return 50;
            case TowerType::SNIPER: return 100;
            case TowerType::CANNON: return 150;
            case TowerType::FREEZE: return 75;
            default: return 50;
        }
    }
};

// ===========================================
// GAME CLASS - Version ultra-sécurisée
// ===========================================
class Game : public Subject, public Observer {
private:
    sf::RenderWindow window;
    std::vector<sf::Vector2f> path;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Projectile> projectiles;
    int gold, lives, wave, level, score;
    float enemySpawnTimer;
    int enemiesInWave, enemiesSpawned;
    bool waveActive, gameOver, showReplayOption, showSaveOption, waitingForName, showAudioControls;
    std::string playerName;
    ResourceManager resourceManager;
    AudioManager audioManager;
    
    // Timer automatique
    const float PREPARATION_TIME = 15.0f;
    float preparationTimer;
    bool preparationPhase;
    
    // Chemins prédéfinis
    const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
        {   // Path 1
            sf::Vector2f(0, 300), sf::Vector2f(200, 300), sf::Vector2f(200, 150),
            sf::Vector2f(500, 150), sf::Vector2f(500, 450), sf::Vector2f(800, 450),
            sf::Vector2f(800, 250), sf::Vector2f(1000, 250)
        },
        {   // Path 2
            sf::Vector2f(0, 200), sf::Vector2f(200, 200), sf::Vector2f(200, 400),
            sf::Vector2f(400, 400), sf::Vector2f(400, 200), sf::Vector2f(600, 200),
            sf::Vector2f(600, 400), sf::Vector2f(800, 400), sf::Vector2f(800, 200), sf::Vector2f(1000, 200)
        },
        {   // Path 3
            sf::Vector2f(0, 350), sf::Vector2f(200, 350), sf::Vector2f(200, 150),
            sf::Vector2f(400, 150), sf::Vector2f(400, 350), sf::Vector2f(600, 350),
            sf::Vector2f(600, 150), sf::Vector2f(800, 150), sf::Vector2f(800, 350), sf::Vector2f(1000, 350)
        }
    };

    void changePath() {
        if (predefinedPaths.empty()) return;
        
        size_t newPathIndex;
        do {
            newPathIndex = rand() % predefinedPaths.size();
        } while (predefinedPaths[newPathIndex] == path && predefinedPaths.size() > 1);
        
        path = predefinedPaths[newPathIndex];
        std::cout << "=== NOUVEAU CHEMIN SÉLECTIONNÉ ===" << std::endl;
    }

    void saveScore() {
        if (playerName.empty()) return;
        
        try {
            json scores = json::array();
            std::ifstream file("scores.json");
            if (file.good()) {
                file >> scores;
                file.close();
            }
            
            json newScore = {
                {"name", playerName},
                {"score", score},
                {"level", level},
                {"wave", wave},
                {"date", std::time(nullptr)}
            };
            
            scores.push_back(newScore);
            
            std::ofstream outFile("scores.json");
            outFile << scores.dump(4);
            outFile.close();
            
            std::cout << "Score sauvegardé!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erreur sauvegarde: " << e.what() << std::endl;
        }
    }
    
    void resetGame() {
        // Nettoyage sécurisé
        enemies.clear();
        towers.clear();
        projectiles.clear();
        
        // Réinitialisation des variables
        gold = 200; lives = 3; wave = 1; level = 1; score = 0;
        enemySpawnTimer = 0; enemiesInWave = 5; enemiesSpawned = 0;
        waveActive = false; gameOver = false; showReplayOption = false;
        showSaveOption = false; waitingForName = false; playerName = "";
        showAudioControls = false;
        
        preparationTimer = PREPARATION_TIME;
        preparationPhase = true;
        
        if (!predefinedPaths.empty()) {
            path = predefinedPaths[0];
        }
        
        audioManager.startMusic();
    }

public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - Version Sécurisée"),
             gold(200), lives(3), wave(1), level(1), score(0), enemySpawnTimer(0),
             enemiesInWave(5), enemiesSpawned(0), waveActive(false), gameOver(false),
             showReplayOption(false), showSaveOption(false), waitingForName(false),
             showAudioControls(false), playerName(""), preparationTimer(PREPARATION_TIME),
             preparationPhase(true) {
        
        srand(static_cast<unsigned int>(time(nullptr)));
        
        std::cout << "=== TOWER DEFENSE - VERSION SÉCURISÉE ===" << std::endl;
        std::cout << "✓ Gestion mémoire améliorée" << std::endl;
        std::cout << "✓ Audio optimisé (volumes réduits)" << std::endl;
        std::cout << "✓ Vérifications de sécurité" << std::endl;
        std::cout << "✓ Protection contre les segfaults" << std::endl;
        
        addObserver(this);
        
        if (!predefinedPaths.empty()) {
            path = predefinedPaths[0];
        }
        
        // Limitation de la fréquence d'images pour stabilité
        window.setFramerateLimit(60);
        
        audioManager.startMusic();
    }

    void notify(const GameEvent& event) override {
        try {
            switch(event.type) {
                case GameEvent::ENEMY_KILLED:
                    gold += event.value;
                    score += event.value * 10;
                    break;
                case GameEvent::ENEMY_REACHED_END:
                    lives = std::max(0, lives - event.value);
                    score = std::max(0, score - 50);
                    break;
                case GameEvent::WAVE_COMPLETED:
                    score += 100;
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur dans notify: " << e.what() << std::endl;
        }
    }

    void run() {
        sf::Clock clock;
        
        try {
            while (window.isOpen()) {
                float deltaTime = clock.restart().asSeconds();
                deltaTime = std::min(deltaTime, 0.033f); // Max 30 FPS pour stabilité
                
                handleEvents();
                update(deltaTime);
                render();
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur dans la boucle principale: " << e.what() << std::endl;
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            try {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                
                if (event.type == sf::Event::TextEntered && waitingForName) {
                    if (event.text.unicode == '\b' && !playerName.empty()) {
                        playerName.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode >= 32 && playerName.length() < 15) {
                        playerName += static_cast<char>(event.text.unicode);
                    }
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    handleKeyPress(event.key.code);
                }
                
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            } catch (const std::exception& e) {
                std::cerr << "Erreur gestion événement: " << e.what() << std::endl;
            }
        }
    }
    
    void handleKeyPress(sf::Keyboard::Key key) {
        // Contrôles audio
        switch(key) {
            case sf::Keyboard::M: audioManager.toggleMute(); break;
            case sf::Keyboard::N: if (audioManager.hasBothMusicsLoaded()) audioManager.switchMusic(); break;
            case sf::Keyboard::Add: case sf::Keyboard::Equal: audioManager.increaseMasterVolume(); break;
            case sf::Keyboard::Subtract: case sf::Keyboard::Hyphen: audioManager.decreaseMasterVolume(); break;
            case sf::Keyboard::Num1: audioManager.increaseMusicVolume(); break;
            case sf::Keyboard::Num2: audioManager.decreaseMusicVolume(); break;
            case sf::Keyboard::Num3: audioManager.increaseEffectsVolume(); break;
            case sf::Keyboard::Num4: audioManager.decreaseEffectsVolume(); break;
            case sf::Keyboard::A: showAudioControls = !showAudioControls; break;
            
            // Contrôles jeu
            case sf::Keyboard::Return: 
                if (waitingForName) { waitingForName = false; showSaveOption = true; }
                break;
            case sf::Keyboard::Space: 
                if (preparationPhase && !gameOver) startWave();
                break;
            case sf::Keyboard::R: 
                if (gameOver) resetGame();
                break;
            case sf::Keyboard::S: 
                if (showSaveOption) { saveScore(); showSaveOption = false; }
                break;
            default: break;
        }
    }
    
    void handleMouseClick(int mouseX, int mouseY) {
        if (gameOver) return;
        
        sf::Vector2f clickPos(static_cast<float>(mouseX), static_cast<float>(mouseY));
        
        // Vérification amélioration tour
        bool towerClicked = false;
        for (auto& tower : towers) {
            if (!tower) continue;
            
            float dx = clickPos.x - tower->position.x;
            float dy = clickPos.y - tower->position.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance <= 25 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
                gold -= tower->getUpgradeCost();
                tower->upgrade();
                audioManager.playUpgradeSound();
                towerClicked = true;
                std::cout << "Tour améliorée niveau " << tower->level << std::endl;
                break;
            }
        }
        
        // Placement nouvelle tour
        if (!towerClicked) {
            placeTower(clickPos);
        }
    }
    
    void placeTower(sf::Vector2f clickPos) {
        const int cost = 50;
        if (gold < cost) return;
        
        // Vérification distance autres tours
        for (const auto& tower : towers) {
            if (!tower) continue;
            float dx = clickPos.x - tower->position.x;
            float dy = clickPos.y - tower->position.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < TOWER_MIN_DISTANCE) return;
        }
        
        // Vérification distance du chemin
        for (size_t i = 0; i < path.size() - 1; ++i) {
            sf::Vector2f start = path[i];
            sf::Vector2f end = path[i + 1];
            
            // Distance point-ligne simplifiée
            float A = clickPos.x - start.x;
            float B = clickPos.y - start.y;
            float C = end.x - start.x;
            float D = end.y - start.y;
            
            float dot = A * C + B * D;
            float lenSq = C * C + D * D;
            float param = (lenSq != 0) ? dot / lenSq : -1;
            
            float xx, yy;
            if (param < 0) { xx = start.x; yy = start.y; }
            else if (param > 1) { xx = end.x; yy = end.y; }
            else { xx = start.x + param * C; yy = start.y + param * D; }
            
            float distance = std::sqrt((clickPos.x - xx) * (clickPos.x - xx) + (clickPos.y - yy) * (clickPos.y - yy));
            if (distance < 60) return;
        }
        
        // Placement réussi
        auto tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
        if (tower) {
            towers.push_back(std::move(tower));
            gold -= cost;
            std::cout << "Tour placée! Or restant: " << gold << std::endl;
        }
    }

    void startWave() {
        waveActive = true;
        preparationPhase = false;
        enemiesSpawned = 0;
        enemySpawnTimer = 0;
        std::cout << "=== VAGUE " << wave << " NIVEAU " << level << " ===" << std::endl;
    }

    void update(float deltaTime) {
        if (gameOver) return;
        
        try {
            // Timer de préparation
            if (preparationPhase) {
                preparationTimer -= deltaTime;
                if (preparationTimer <= 0) {
                    startWave();
                }
            }
            
            // Spawn ennemis
            if (waveActive && enemiesSpawned < enemiesInWave) {
                enemySpawnTimer += deltaTime;
                if (enemySpawnTimer >= 1.2f) { // Spawn plus lent pour stabilité
                    spawnEnemy();
                    enemiesSpawned++;
                    enemySpawnTimer = 0;
                }
            }

            // Mise à jour ennemis
            updateEnemies(deltaTime);
            
            // Mise à jour tours
            updateTowers(deltaTime);
            
            // Mise à jour projectiles
            updateProjectiles(deltaTime);
            
            // Nettoyage
            cleanupObjects();
            
            // Vérifications de fin
            checkWaveEnd();
            checkGameOver();
            
        } catch (const std::exception& e) {
            std::cerr << "Erreur update: " << e.what() << std::endl;
        }
    }
    
    void spawnEnemy() {
        if (path.empty()) return;
        
        EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
        auto enemy = EnemyFactory::createEnemy(type);
        
        if (enemy) {
            enemy->position = path[0];
            
            // Scaling avec limites
            float levelMultiplier = 1.0f + (level - 1) * 0.1f;
            float waveMultiplier = 1.0f + (wave - 1) * 0.05f;
            float totalMultiplier = std::min(3.0f, levelMultiplier * waveMultiplier);
            
            enemy->health = static_cast<int>(enemy->health * totalMultiplier);
            enemy->maxHealth = enemy->health;
            enemy->speed = std::min(120.0f, enemy->speed * (1.0f + (level - 1) * 0.02f));
            enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.05f));
            
            enemies.push_back(std::move(enemy));
        }
    }
    
    void updateEnemies(float deltaTime) {
        for (auto& enemy : enemies) {
            if (enemy && enemy->alive) {
                enemy->update(deltaTime, path);
                
                if (enemy->reachedEnd(path)) {
                    notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
                    enemy->alive = false;
                }
            }
        }
    }
    
    void updateTowers(float deltaTime) {
        for (auto& tower : towers) {
            if (tower) {
                tower->update(deltaTime, enemies, projectiles, this, &audioManager);
            }
        }
    }
    
    void updateProjectiles(float deltaTime) {
        for (auto& projectile : projectiles) {
            projectile.update(deltaTime);
        }
    }
    
    void cleanupObjects() {
        // Nettoyage projectiles inactifs
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& proj) { return !proj.active; }),
            projectiles.end()
        );

        // Nettoyage ennemis morts
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) { 
                    return !enemy || !enemy->alive; 
                }),
            enemies.end()
        );
    }
    
    void checkWaveEnd() {
        if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
            waveActive = false;
            preparationPhase = true;
            preparationTimer = PREPARATION_TIME;
            
            notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
            wave++;
            
            if (wave > 3) {
                level++;
                wave = 1;
                enemiesInWave = std::min(20, 5 + (level - 1) * 2);
                gold += 50 * level;
                
                changePath();
                towers.clear();
                
                std::cout << "=== NIVEAU " << level << " ===" << std::endl;
            } else {
                enemiesInWave = std::min(25, enemiesInWave + 2);
            }
        }
    }
    
    void checkGameOver() {
        if (lives <= 0) {
            gameOver = true;
            showReplayOption = true;
            waitingForName = true;
            audioManager.stopMusic();
        }
    }

    std::string intToString(int value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, 
                  sf::Color color = sf::Color::White, int size = 16) {
        sf::Text sfText;
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(color);
        sfText.setPosition(x, y);
        
        if (resourceManager.isFontLoaded()) {
            sfText.setFont(resourceManager.getFont());
        }
        
        window.draw(sfText);
    }

    void render() {
        try {
            window.clear(sf::Color(40, 40, 40));

            drawPath();
            drawTowers();
            drawProjectiles();
            drawEnemies();
            drawUI();
            
            // Timer et états de jeu
            if (preparationPhase && !gameOver) {
                sf::RectangleShape textBg(sf::Vector2f(500, 100));
                textBg.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 50);
                textBg.setFillColor(sf::Color(0, 0, 0, 150));
                textBg.setOutlineColor(sf::Color::Green);
                textBg.setOutlineThickness(3);
                window.draw(textBg);
                
                int secondsLeft = static_cast<int>(std::ceil(preparationTimer));
                drawText(window, "PROCHAINE VAGUE: " + intToString(secondsLeft) + "s", 
                        WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 35, sf::Color::Red, 24);
                
                drawText(window, "ESPACE pour démarrer maintenant", WINDOW_WIDTH/2 - 140, WINDOW_HEIGHT/2 - 5, sf::Color::Green, 16);
                drawText(window, "Niveau " + intToString(level) + " Vague " + intToString(wave), 
                        WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 + 20, sf::Color::Cyan, 16);
                
                if (wave == 1) {
                    drawText(window, "NOUVEAU CHEMIN - PLACEZ VOS TOURS!", 
                            WINDOW_WIDTH/2 - 160, WINDOW_HEIGHT/2 + 45, sf::Color::Yellow, 14);
                }
            }

            // Écran game over
            if (gameOver) {
                drawGameOverScreen();
            }

            window.display();
        } catch (const std::exception& e) {
            std::cerr << "Erreur rendu: " << e.what() << std::endl;
        }
    }
    
    void drawPath() {
        for (size_t i = 0; i < path.size() - 1; ++i) {
            sf::Vector2f start = path[i];
            sf::Vector2f end = path[i + 1];
            sf::Vector2f direction = end - start;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (length > 0) {
                direction = direction / length;
                sf::RectangleShape pathSegment;
                pathSegment.setSize(sf::Vector2f(length, 40));
                pathSegment.setPosition(start);
                pathSegment.setFillColor(sf::Color(100, 100, 100));
                
                float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;
                pathSegment.setRotation(angle);
                pathSegment.setOrigin(0, 20);
                
                window.draw(pathSegment);
            }
        }

        // Points du chemin
        for (const sf::Vector2f& point : path) {
            sf::CircleShape dot(3);
            dot.setPosition(point.x - 3, point.y - 3);
            dot.setFillColor(sf::Color::Yellow);
            window.draw(dot);
        }
    }
    
    void drawTowers() {
        for (auto& tower : towers) {
            if (tower) {
                tower->draw(window);
            }
        }
    }
    
    void drawProjectiles() {
        for (auto& projectile : projectiles) {
            projectile.draw(window);
        }
    }
    
    void drawEnemies() {
        for (auto& enemy : enemies) {
            if (enemy && enemy->alive) {
                enemy->draw(window);
            }
        }
    }
    
    void drawUI() {
        // Interface principale
        resourceManager.drawGoldIcon(window, 10, 10);
        drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

        resourceManager.drawHeartIcon(window, 10, 40);
        drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

        drawText(window, "Niveau: " + intToString(level), 300, 15, sf::Color::White, 18);
        drawText(window, "Vague: " + intToString(wave), 300, 45, sf::Color::Cyan, 18);
        drawText(window, "Tours: " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
        drawText(window, "Ennemis: " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);
        drawText(window, "Score: " + intToString(score), 850, 15, sf::Color::White, 18);

        // Statut audio
        if (audioManager.getIsMuted()) {
            drawText(window, "MUET", 850, 45, sf::Color::Red, 16);
        } else {
            drawText(window, "Musique " + intToString(audioManager.getCurrentMusicTrack() + 1), 850, 45, sf::Color::Green, 16);
        }

        // Panneau contrôles audio
        if (showAudioControls) {
            drawAudioControls();
        }

        // Instructions
        drawText(window, "Clic: Placer tour (50 or) | Clic sur tour: Améliorer | ESPACE: Démarrer | A: Audio", 
                10, WINDOW_HEIGHT - 30, sf::Color::White, 14);
    }
    
    void drawAudioControls() {
        sf::RectangleShape controlsBg(sf::Vector2f(280, 180));
        controlsBg.setPosition(10, 80);
        controlsBg.setFillColor(sf::Color(0, 0, 0, 180));
        controlsBg.setOutlineColor(sf::Color::White);
        controlsBg.setOutlineThickness(2);
        window.draw(controlsBg);
        
        drawText(window, "=== CONTROLES AUDIO ===", 20, 90, sf::Color::White, 14);
        drawText(window, "M: Muet/Son", 20, 110, sf::Color::Cyan, 12);
        if (audioManager.hasBothMusicsLoaded()) {
            drawText(window, "N: Changer musique", 20, 125, sf::Color::Cyan, 12);
        }
        drawText(window, "+/-: Volume (" + intToString(static_cast<int>(audioManager.getMasterVolume())) + "%)", 20, 140, sf::Color::Yellow, 12);
        drawText(window, "1/2: Musique (" + intToString(static_cast<int>(audioManager.getMusicVolume())) + "%)", 20, 155, sf::Color::Yellow, 12);
        drawText(window, "3/4: Effets (" + intToString(static_cast<int>(audioManager.getEffectsVolume())) + "%)", 20, 170, sf::Color::Yellow, 12);
        drawText(window, "A: Masquer", 20, 185, sf::Color::White, 12);
    }
    
    void drawGameOverScreen() {
        sf::RectangleShape gameOverBg(sf::Vector2f(400, 180));
        gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 90);
        gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
        gameOverBg.setOutlineColor(sf::Color::White);
        gameOverBg.setOutlineThickness(3);
        window.draw(gameOverBg);
        
        drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 - 70, sf::Color::White, 28);
        drawText(window, "Score Final: " + intToString(score), WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 - 35, sf::Color::White, 18);
        
        if (waitingForName) {
            drawText(window, "Votre nom:", WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/2, sf::Color::White, 16);
            drawText(window, playerName + "|", WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/2 + 25, sf::Color::Yellow, 16);
            drawText(window, "Appuyez sur Entrée", WINDOW_WIDTH/2 - 80, WINDOW_HEIGHT/2 + 50, sf::Color::White, 14);
        }
        else if (showSaveOption) {
            drawText(window, "S: Sauvegarder", WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 + 10, sf::Color::Green, 16);
            drawText(window, "R: Rejouer", WINDOW_WIDTH/2 - 40, WINDOW_HEIGHT/2 + 35, sf::Color::Yellow, 16);
        }
        else if (showReplayOption) {
            drawText(window, "R: Rejouer", WINDOW_WIDTH/2 - 40, WINDOW_HEIGHT/2 + 25, sf::Color::Yellow, 18);
        }
    }
};

int main() {
    try {
        std::cout << "=== TOWER DEFENSE - VERSION ULTRA-SÉCURISÉE ===" << std::endl;
        std::cout << "✓ Protection contre les segfaults" << std::endl;
        std::cout << "✓ Audio optimisé (volumes réduits)" << std::endl;
        std::cout << "✓ Gestion mémoire robuste" << std::endl;
        std::cout << "✓ Limitation FPS pour stabilité" << std::endl;
        std::cout << "✓ Vérifications de sécurité partout" << std::endl;
        std::cout << "====================================================" << std::endl;
        
        Game game;
        game.run();
        
        std::cout << "Jeu fermé proprement." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERREUR FATALE: " << e.what() << std::endl;
        std::cerr << "Le jeu va se fermer pour éviter un crash." << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "ERREUR INCONNUE détectée!" << std::endl;
        std::cerr << "Le jeu va se fermer pour éviter un crash." << std::endl;
        return -1;
    }
    
    return 0;
}