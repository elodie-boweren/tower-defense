#include <SFML/Graphics.hpp>
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
// OBSERVER PATTERN - Interface for events
// ===========================================
class GameEvent {
public:
    enum Type { ENEMY_KILLED, ENEMY_REACHED_END, WAVE_COMPLETED };
    Type type;
    int value; // Gold earned, damage taken, etc.
    
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
        observers.push_back(observer);
    }
    
    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notifyObservers(const GameEvent& event) {
        for (auto* observer : observers) {
            observer->notify(event);
        }
    }
};

// ===========================================
// RESOURCE MANAGER - To manage textures, sprites and fonts
// ===========================================
class ResourceManager {
private:
    sf::Texture goldTexture, heartTexture;
    sf::Sprite goldSprite, heartSprite;
    bool goldLoaded, heartLoaded;
    
    // Font management
    sf::Font arialFont;
    bool fontLoaded;
    
public:
    ResourceManager() : goldLoaded(false), heartLoaded(false), fontLoaded(false) {
        loadResources();
        loadFont();
    }
    
    // Font loading method with comprehensive path search
    void loadFont() {
        std::cout << "=== LOADING ARIAL FONT ===" << std::endl;
        
        // List of paths to try for arial.ttf font
        std::vector<std::string> fontPaths = {
            "./assets/arial.ttf",                    // In assets directory
            "assets/arial.ttf",                      // Without ./
            "../../assets/arial.ttf",                // Two levels up
            "../../../assets/arial.ttf",             // Three levels up
            "../../../../assets/arial.ttf",          // Four levels up
            "../../src/assets/arial.ttf",            // If assets is in src
            "../assets/arial.ttf",                   // One level up
            "./arial.ttf",                          // In current directory
            "arial.ttf",                            // Direct filename
        };
        
        // Try to load font from each path
        for (const std::string& path : fontPaths) {
            std::cout << "Trying font path: " << path << std::endl;
            
            // Check if file exists first
            struct stat info;
            if (stat(path.c_str(), &info) == 0) {
                std::cout << "  -> Font file found, attempting to load..." << std::endl;
                if (arialFont.loadFromFile(path)) {
                    fontLoaded = true;
                    std::cout << "✓ Font successfully loaded from: " << path << std::endl;
                    return;
                } else {
                    std::cout << "  -> SFML font loading failed for: " << path << std::endl;
                }
            } else {
                std::cout << "  -> Font file not found at: " << path << std::endl;
            }
        }
        
        std::cout << "⚠ WARNING: Arial font not found in any location!" << std::endl;
        std::cout << "  Please place arial.ttf in the assets/ folder" << std::endl;
        std::cout << "=== END FONT LOADING ===" << std::endl;
    }
    
    // Get font reference for text rendering
    const sf::Font& getFont() const {
        return arialFont;
    }
    
    // Check if font is available
    bool isFontLoaded() const {
        return fontLoaded;
    }
    
    void loadResources() {
        // Display current working directory for debug
        std::cout << "=== DEBUG WSL PATHS ===" << std::endl;
        
        // Display current working directory
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        }
        
        // List of paths to try (adapted for your structure)
        std::vector<std::string> pathsToTry = {
            "./assets/",                    // In executable directory
            "assets/",                      // Without ./
            "../../assets/",                // Two levels up (from build/bin to root)
            "../../../assets/",             // Three levels up
            "../../../../assets/",          // Four levels up
            "../../src/assets/",            // If assets is in src
            "../assets/",                   // One level up
            "./"                           // In current directory
        };
        
        // Test existence of assets folder
        struct stat info;
        if (stat("./assets", &info) == 0 && S_ISDIR(info.st_mode)) {
            std::cout << "✓ Folder './assets' found" << std::endl;
        } else {
            std::cout << "⚠ Folder './assets' not found" << std::endl;
        }
        
        // Loading gold icon
        bool goldFound = false;
        for (const auto& basePath : pathsToTry) {
            std::string fullPath = basePath + "gold_icon.png";
            std::cout << "Attempt: " << fullPath << std::endl;
            
            // Check if file exists
            if (stat(fullPath.c_str(), &info) == 0) {
                std::cout << "  -> File found, attempting to load..." << std::endl;
                if (goldTexture.loadFromFile(fullPath)) {
                    goldLoaded = true;
                    goldSprite.setTexture(goldTexture);
                    goldSprite.setScale(0.8f, 0.8f);
                    std::cout << "✓ Texture gold_icon.png loaded from: " << fullPath << std::endl;
                    goldFound = true;
                    break;
                } else {
                    std::cout << "  -> SFML loading failed" << std::endl;
                }
            } else {
                std::cout << "  -> File not found" << std::endl;
            }
        }
        if (!goldFound) {
            std::cout << "⚠ Texture gold_icon.png not found in all paths" << std::endl;
        }
        
        // Loading heart icon
        bool heartFound = false;
        for (const auto& basePath : pathsToTry) {
            std::string fullPath = basePath + "heart_icon.png";
        if (stat(fullPath.c_str(), &info) == 0) {
            std::cout << "  -> File found, attempting to load..." << std::endl;
            if (heartTexture.loadFromFile(fullPath)) {
                heartLoaded = true;
                heartSprite.setTexture(heartTexture);
                heartSprite.setScale(0.8f, 0.8f);
                std::cout << "✓ Texture heart_icon.png loaded from: " << fullPath << std::endl;
                heartFound = true;
                break;
            } else {
                std::cout << "  -> SFML loading failed" << std::endl;
            }
        } else {
            std::cout << "  -> File not found" << std::endl;
        }
        }
        if (!heartFound) {
            std::cout << "⚠ Texture heart_icon.png not found" << std::endl;
        }
        
        std::cout << "=== END DEBUG PATHS ===" << std::endl;
    }
    
    void drawGoldIcon(sf::RenderWindow& window, float x, float y) {
        if (goldLoaded) {
            goldSprite.setPosition(x, y);
            window.draw(goldSprite);
        } else {
            // Fallback: more visible yellow circle
            sf::CircleShape fallback(12);
            fallback.setPosition(x, y);
            fallback.setFillColor(sf::Color::Yellow);
            fallback.setOutlineColor(sf::Color::Black);
            fallback.setOutlineThickness(1);
            window.draw(fallback);
        }
    }
    
    void drawHeartIcon(sf::RenderWindow& window, float x, float y) {
        if (heartLoaded) {
            heartSprite.setPosition(x, y);
            window.draw(heartSprite);
        } else {
            // Fallback: simplified heart
            sf::CircleShape fallback(12);
            fallback.setPosition(x, y);
            fallback.setFillColor(sf::Color::Red);
            fallback.setOutlineColor(sf::Color::White);
            fallback.setOutlineThickness(1);
            window.draw(fallback);
        }
    }
};

// ===========================================
// FACTORY PATTERN - Enemy creation
// ===========================================
class Enemy {
public:
    sf::Vector2f position;
    int health;
    int maxHealth;
    float speed;
    int reward;
    bool alive;
    int pathIndex;
    std::string type;

    Enemy(int hp, float spd, int rew, const std::string& t) 
        : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
        // La position initiale sera définie par le Game
    }

    virtual ~Enemy() = default;

    void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
        if (!alive || pathIndex >= path.size() - 1) return;

        sf::Vector2f target = path[pathIndex + 1];
        sf::Vector2f direction = target - position;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 10.0f) {
            pathIndex++;
        } else {
            direction = direction / distance;
            position += direction * speed * deltaTime;
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            alive = false;
        }
    }

    virtual sf::Color getColor() {
        if (health > maxHealth * 0.7f) return sf::Color::Green;
        else if (health > maxHealth * 0.3f) return sf::Color::Yellow;
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

        if (health > 0) {
            sf::RectangleShape healthFill(sf::Vector2f(30 * (float)health / maxHealth, 5));
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
    
    sf::Color getColor() override {
        return sf::Color::Green;
    }
};

class Orc : public Enemy {
public:
    Orc() : Enemy(120, 45, 20, "Orc") {}
    
    sf::Color getColor() override {
        return sf::Color::Red;
    }
};

class Troll : public Enemy {
public:
    Troll() : Enemy(300, 35, 50, "Troll") {}
    
    sf::Color getColor() override {
        return sf::Color::Magenta;
    }
};

// Nouveaux types d'ennemis
class HeavyEnemy : public Enemy {
public:
    HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
    sf::Color getColor() override {
        return sf::Color(139, 69, 19); // Marron
    }
};

class MediumEnemy : public Enemy {
public:
    MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
    sf::Color getColor() override {
        return sf::Color(0, 128, 128); // Turquoise
    }
};

// Factory to create enemies
class EnemyFactory {
public:
    enum EnemyType { GOBLIN, ORC, TROLL, HEAVY, MEDIUM };
    
    static std::unique_ptr<Enemy> createEnemy(EnemyType type) {
        switch(type) {
            case GOBLIN:
                std::cout << "Factory: Creating a Goblin" << std::endl;
                return std::make_unique<Goblin>();
            case ORC:
                std::cout << "Factory: Creating an Orc" << std::endl;
                return std::make_unique<Orc>();
            case TROLL:
                std::cout << "Factory: Creating a Troll" << std::endl;
                return std::make_unique<Troll>();
            case HEAVY:
                std::cout << "Factory: Creating a Heavy Enemy" << std::endl;
                return std::make_unique<HeavyEnemy>();
            case MEDIUM:
                std::cout << "Factory: Creating a Medium Enemy" << std::endl;
                return std::make_unique<MediumEnemy>();
            default:
                return std::make_unique<Goblin>();
        }
    }
    
    static EnemyType getEnemyTypeForWave(int wave, int level, int enemyIndex) {
        int totalDifficulty = wave + (level - 1) * 3;
        
        if (totalDifficulty <= 3) {
            return GOBLIN;
        }
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

// Class for projectiles
class Projectile {
public:
    sf::Vector2f position;
    sf::Vector2f targetPosition;
    sf::Vector2f velocity;
    int damage;
    float speed;
    bool active;
    Enemy* targetEnemy;
    Subject* gameSubject;

    Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject) 
        : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), gameSubject(subject) {
        
        if (target) {
            targetPosition = target->position;
            // Calculate direction towards target
            sf::Vector2f direction = targetPosition - position;
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (distance > 0) {
                velocity = (direction / distance) * speed;
            }
        }
    }

    void update(float deltaTime) {
        if (!active) return;

        // If target is still alive, follow its position
        if (targetEnemy && targetEnemy->alive) {
            targetPosition = targetEnemy->position;
            sf::Vector2f direction = targetPosition - position;
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (distance < 10.0f) {
                // Impact!
                int oldHealth = targetEnemy->health;
                targetEnemy->takeDamage(damage);
                
                // Notify if enemy is killed
                if (targetEnemy->health <= 0 && oldHealth > 0) {
                    gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
                }
                
                std::cout << "Projectile hits " << targetEnemy->type << "! Damage: " << damage << std::endl;
                active = false;
                return;
            }
            
            // Update velocity to follow target
            if (distance > 0) {
                velocity = (direction / distance) * speed;
            }
        }

        // Move projectile
        position += velocity * deltaTime;
        
        // Deactivate if too far (safety)
        if (position.x < -50 || position.x > WINDOW_WIDTH + 50 || 
            position.y < -50 || position.y > WINDOW_HEIGHT + 50) {
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
// FACTORY PATTERN - Tower creation
// ===========================================

// Enumeration of tower types
enum class TowerType {
    BASIC,
    SNIPER,
    CANNON,
    FREEZE
};

class Tower {
public:
    sf::Vector2f position;
    int damage;
    float range;
    float attackCooldown;
    float timeSinceLastAttack;
    int level;
    std::string towerType;

    Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1), towerType("Basic") {}

    // Protected constructor for derived classes
protected:
    Tower(sf::Vector2f pos, int dmg, float rng, float cooldown, std::string type) 
        : position(pos), damage(dmg), range(rng), attackCooldown(cooldown), timeSinceLastAttack(0), level(1), towerType(type) {}

public:
    virtual ~Tower() = default;

    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject) {
        timeSinceLastAttack += deltaTime;
        
        if (timeSinceLastAttack >= attackCooldown) {
            Enemy* target = nullptr;
            float closestDistance = range + 1;
            
            for (auto& enemy : enemies) {
                if (!enemy->alive) continue;
                
                float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
                if (distance <= range && distance < closestDistance) {
                    target = enemy.get();
                    closestDistance = distance;
                }
            }
            
            if (target) {
                // Create a projectile instead of attacking directly
                projectiles.push_back(Projectile(position, target, damage, subject));
                timeSinceLastAttack = 0;
                std::cout << towerType << " Tower shoots at " << target->type << "!" << std::endl;
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
        rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
        rangeCircle.setOutlineThickness(1);
        window.draw(rangeCircle);

        // Tower (delegate to specific implementation)
        drawTower(window);
    }

    virtual int getUpgradeCost() {
        if (level == 1) return 100;
        if (level == 2) return 200;
        return 0;
    }

    virtual bool canUpgrade() {
        return level < 3;
    }

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

// Derived tower classes
class BasicTower : public Tower {
public:
    BasicTower(sf::Vector2f pos) : Tower(pos, 20, 80, 1.0f, "Basic") {}
};

class SniperTower : public Tower {
public:
    SniperTower(sf::Vector2f pos) : Tower(pos, 50, 150, 2.0f, "Sniper") {}
    
    void upgrade() override {
        if (level == 1) {
            level = 2;
            damage = 100;
            range = 180;
            attackCooldown = 1.8f;
        } else if (level == 2) {
            level = 3;
            damage = 200;
            range = 220;
            attackCooldown = 1.5f;
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
            level = 2;
            damage = 200;
            range = 70;
            attackCooldown = 2.5f;
        } else if (level == 2) {
            level = 3;
            damage = 400;
            range = 80;
            attackCooldown = 2.0f;
        }
    }

protected:
    void drawTower(sf::RenderWindow& window) override {
        sf::CircleShape shape(18);
        shape.setPosition(position.x - 18, position.y - 18);
        
        if (level == 1) shape.setFillColor(sf::Color(139, 69, 19)); // Brown
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
            level = 2;
            damage = 10;
            range = 110;
            attackCooldown = 0.4f;
        } else if (level == 2) {
            level = 3;
            damage = 20;
            range = 130;
            attackCooldown = 0.3f;
        }
    }

protected:
    void drawTower(sf::RenderWindow& window) override {
        sf::RectangleShape shape(sf::Vector2f(28, 28));
        shape.setPosition(position.x - 14, position.y - 14);
        
        if (level == 1) shape.setFillColor(sf::Color(173, 216, 230)); // Light Blue
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
            case TowerType::BASIC:
                std::cout << "Factory: Creating Basic Tower" << std::endl;
                return std::make_unique<BasicTower>(position);
            case TowerType::SNIPER:
                std::cout << "Factory: Creating Sniper Tower" << std::endl;
                return std::make_unique<SniperTower>(position);
            case TowerType::CANNON:
                std::cout << "Factory: Creating Cannon Tower" << std::endl;
                return std::make_unique<CannonTower>(position);
            case TowerType::FREEZE:
                std::cout << "Factory: Creating Freeze Tower" << std::endl;
                return std::make_unique<FreezeTower>(position);
            default:
                return std::make_unique<BasicTower>(position);
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
    
    static std::string getTowerDescription(TowerType type) {
        switch (type) {
            case TowerType::BASIC: return "Basic Tower - Balanced";
            case TowerType::SNIPER: return "Sniper Tower - Long Range";
            case TowerType::CANNON: return "Cannon Tower - High Damage";
            case TowerType::FREEZE: return "Freeze Tower - Slows Enemies";
            default: return "Unknown Tower";
        }
    }
};

// ===========================================
// GAME CLASS - Implements Observer to receive events
// ===========================================
class Game : public Subject, public Observer {
private:
    sf::RenderWindow window;
    std::vector<sf::Vector2f> path;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers; // Changed from std::vector<Tower>
    std::vector<Projectile> projectiles;
    int gold;
    int lives;
    int wave;
    int level;
    float enemySpawnTimer;
    int enemiesInWave;
    int enemiesSpawned;
    bool waveActive;
    int score;
    ResourceManager resourceManager;
    bool gameOver;
    bool showReplayOption;
    bool showSaveOption;
    std::string playerName;
    bool waitingForName;
    
    // Chemins prédéfinis
    const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
        // Chemin original
        {
            sf::Vector2f(0, 300),
            sf::Vector2f(200, 300),
            sf::Vector2f(200, 150),
            sf::Vector2f(500, 150),
            sf::Vector2f(500, 450),
            sf::Vector2f(800, 450),
            sf::Vector2f(800, 250),
            sf::Vector2f(1000, 250)
        },
        // Chemin en zigzag
        {
            sf::Vector2f(0, 200),
            sf::Vector2f(200, 200),
            sf::Vector2f(200, 400),
            sf::Vector2f(400, 400),
            sf::Vector2f(400, 200),
            sf::Vector2f(600, 200),
            sf::Vector2f(600, 400),
            sf::Vector2f(800, 400),
            sf::Vector2f(800, 200),
            sf::Vector2f(1000, 200)
        },
        // Chemin en spirale
        {
            sf::Vector2f(0, 350),
            sf::Vector2f(200, 350),
            sf::Vector2f(200, 150),
            sf::Vector2f(400, 150),
            sf::Vector2f(400, 350),
            sf::Vector2f(600, 350),
            sf::Vector2f(600, 150),
            sf::Vector2f(800, 150),
            sf::Vector2f(800, 350),
            sf::Vector2f(1000, 350)
        },
        // Chemin en S
        {
            sf::Vector2f(0, 100),
            sf::Vector2f(200, 100),
            sf::Vector2f(200, 300),
            sf::Vector2f(400, 300),
            sf::Vector2f(400, 500),
            sf::Vector2f(600, 500),
            sf::Vector2f(600, 300),
            sf::Vector2f(800, 300),
            sf::Vector2f(800, 100),
            sf::Vector2f(1000, 100)
        }
    };

    void changePath() {
        // Sélectionner un chemin aléatoire différent du précédent
        int newPathIndex;
        do {
            newPathIndex = rand() % predefinedPaths.size();
        } while (predefinedPaths[newPathIndex] == path);
        
        path = predefinedPaths[newPathIndex];
        std::cout << "=== NEW PATH SELECTED FOR LEVEL " << level << " ===" << std::endl;
    }

    void saveScore() {
        try {
            json scores;
            std::ifstream file("scores.json");
        if (file.good()) {
            // file exists, read it
            file >> scores;
            file.close();
        } else {
            // File doesn't exist, create empty array
            scores = json::array();
            std::cout << "Creating file scores.json" << std::endl;
        }
            // Add new score
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
            
            std::cout << "Score sauvegardé avec succès!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors de la sauvegarde du score: " << e.what() << std::endl;
        }
    }
    
    void resetGame() {
        gold = 200;
        lives = 3;
        wave = 1;
        level = 1;
        score = 0;
        enemies.clear();
        towers.clear();
        projectiles.clear();
        gameOver = false;
        showReplayOption = false;
        showSaveOption = false;
        waitingForName = false;
        playerName = "";
        path = predefinedPaths[0];
    }

public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense "),
             gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
             enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
             gameOver(false), showReplayOption(false), showSaveOption(false),
             waitingForName(false), playerName("") {
        
        // Initialiser le générateur de nombres aléatoires
        srand(time(nullptr));
        
        std::cout << "=== TOWER DEFENSE ===" << std::endl;
        std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
        std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
        std::cout << "✓ Observer Pattern: Event management" << std::endl;
        std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
        
        // Subscribe to its own events
        addObserver(this);
        
        // Initialiser le premier chemin
        path = predefinedPaths[0];
        
        std::cout << "CONTROLS:" << std::endl;
        std::cout << "- Keys 1-4: Select tower type" << std::endl;
        std::cout << "- Left click: Place tower" << std::endl;
        std::cout << "- Click on tower: Upgrade" << std::endl;
        std::cout << "- Press SPACE to start wave" << std::endl;
        std::cout << "\nREQUIRED FILES (place in assets/ folder):" << std::endl;
        std::cout << "- assets/arial.ttf (REQUIRED for text display)" << std::endl;
        std::cout << "- assets/gold_icon.png (optional)" << std::endl;
        std::cout << "- assets/heart_icon.png (optional)" << std::endl;
    }

    // Observer implementation - Reacts to game events
    void notify(const GameEvent& event) override {
        switch(event.type) {
            case GameEvent::ENEMY_KILLED:
                gold += event.value;
                score += event.value * 10;
                std::cout << "Observer: Enemy killed! +" << event.value << " gold, Score: " << score << std::endl;
                break;
                
            case GameEvent::ENEMY_REACHED_END:
                lives -= event.value;
                score -= 50;
                std::cout << "Observer: Enemy passed! -" << event.value << " life(s)" << std::endl;
                break;
                
            case GameEvent::WAVE_COMPLETED:
                score += 100;
                std::cout << "Observer: Wave completed! +100 bonus points" << std::endl;
                break;
        }
    }

    void run() {
        sf::Clock clock;
        
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            handleEvents();
            update(deltaTime);
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::TextEntered && waitingForName) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back();
                }
                else if (event.text.unicode < 128 && playerName.length() < 20) {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return && waitingForName) {
                    waitingForName = false;
                    showSaveOption = true;
                }
                else if (event.key.code == sf::Keyboard::Space && !waveActive && !gameOver) {
                    startWave();
                }
                else if (event.key.code == sf::Keyboard::R && gameOver) {
                    resetGame();
                }
                else if (event.key.code == sf::Keyboard::S && showSaveOption) {
                    saveScore();
                    showSaveOption = false;
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
                // Check click on existing tower
                bool towerClicked = false;
                for (auto& tower : towers) {
                    float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
                    if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
                        gold -= tower->getUpgradeCost();
                        tower->upgrade();
                        towerClicked = true;
                        std::cout << "Tower upgraded to level " << tower->level << std::endl;
                        break;
                    }
                }
                
                // New tower placement
                if (!towerClicked) {
                    int cost = TowerFactory::getTowerCost(selectedTowerType);
                    if (gold >= cost) {
                        bool canPlace = true;
                        
                        // Check distance with other towers
                        for (const auto& tower : towers) {
                            float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
                            if (distance < TOWER_MIN_DISTANCE) {
                                canPlace = false;
                                break;
                            }
                        }
                        
                        // Check distance from path
                        for (size_t i = 0; i < path.size() - 1; i++) {
                            sf::Vector2f start = path[i];
                            sf::Vector2f end = path[i + 1];
                            
                            float A = clickPos.x - start.x;
                            float B = clickPos.y - start.y;
                            float C = end.x - start.x;
                            float D = end.y - start.y;
                            
                            float dot = A * C + B * D;
                            float lenSq = C * C + D * D;
                            float param = (lenSq != 0) ? dot / lenSq : -1;
                            
                            float xx, yy;
                            if (param < 0) {
                                xx = start.x; yy = start.y;
                            } else if (param > 1) {
                                xx = end.x; yy = end.y;
                            } else {
                                xx = start.x + param * C;
                                yy = start.y + param * D;
                            }
                            
                            float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
                            if (distance < 50) {
                                canPlace = false;
                                break;
                            }
                        }
                        
                        if (canPlace) {
                            auto tower = TowerFactory::createTower(selectedTowerType, clickPos);
                            towers.push_back(std::move(tower));
                            gold -= cost;
                            std::cout << "Tower placed! Remaining gold: " << gold << std::endl;
                        }
                    }
                }
            }
        }
    }

    void startWave() {
        waveActive = true;
        enemiesSpawned = 0;
        enemySpawnTimer = 0;
        std::cout << "=== LEVEL " << level << " - WAVE " << wave << " STARTS ===" << std::endl;
    }

    void update(float deltaTime) {
        // Stop updating if game is over
        if (lives <= 0 || wave > 10) {
            return;
        }
        
        // Spawn enemies with Factory Pattern
        if (waveActive && enemiesSpawned < enemiesInWave) {
            enemySpawnTimer += deltaTime;
            if (enemySpawnTimer >= 1.0f) {
                // Using Factory Pattern with level-based difficulty
                EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
                auto enemy = EnemyFactory::createEnemy(type);
                
                // Définir la position initiale au début du chemin
                enemy->position = path[0];
                
                // Augmenter les statistiques des ennemis en fonction du niveau et de la vague
                float levelMultiplier = 1.0f + (level - 1) * 0.15f;  // Réduit à +15% par niveau
                float waveMultiplier = 1.0f + (wave - 1) * 0.05f;    // Réduit à +5% par vague
                float totalMultiplier = levelMultiplier * waveMultiplier;
                
                // Appliquer les multiplicateurs aux statistiques
                enemy->health = static_cast<int>(enemy->health * totalMultiplier);
                enemy->maxHealth = enemy->health;
                enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);  // Réduit les bonus de vitesse
                enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));  // Réduit les bonus de récompense
                
                enemies.push_back(std::move(enemy));
                
                enemiesSpawned++;
                enemySpawnTimer = 0;
            }
        }

        // Update enemies
        for (auto& enemy : enemies) {
            if (enemy->alive) {
                enemy->update(deltaTime, path);
                
                if (enemy->reachedEnd(path)) {
                    // Using Observer Pattern
                    notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
                    enemy->alive = false;
                }
            }
        }

        // Update towers (now they create projectiles)
        for (auto& tower : towers) {
            tower->update(deltaTime, enemies, projectiles, this);
        }

        // Update projectiles
        for (auto& projectile : projectiles) {
            projectile.update(deltaTime);
        }

        // Remove inactive projectiles
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& proj) { return !proj.active; }),
            projectiles.end()
        );

        // Remove dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) { return !enemy->alive; }),
            enemies.end()
        );

        // Check end of wave
        if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
            waveActive = false;
            notifyObservers(GameEvent(GameEvent::WAVE_COMPLETED));
            wave++;
            
            // Changer de niveau après 3 vagues
            if (wave > 3) {
                level++;
                wave = 1;
                enemiesInWave = 5 + (level - 1) * 2;  // Plus d'ennemis par vague à chaque niveau
                gold += 100 * level;  // Bonus d'or à chaque niveau
                
                // Changer le chemin et supprimer les tours
                changePath();
                towers.clear();
                
                std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
                std::cout << "New path selected - Towers have been removed" << std::endl;
            } else {
                enemiesInWave += 2;
            }
            std::cout << "Press SPACE for level " << level << " wave " << wave << std::endl;
        }

        // Check for game over
        if (lives <= 0) {
            gameOver = true;
            showReplayOption = true;
            waitingForName = true;
        }
    }

    // Convert integer to string for display
    std::string intToString(int value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    // Enhanced text drawing method with Arial font support
    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 16) {
        sf::Text sfText;
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(color);
        sfText.setPosition(x, y);
        
        // Use Arial font if loaded, otherwise SFML will use default font
        if (resourceManager.isFontLoaded()) {
            sfText.setFont(resourceManager.getFont());
        }
        
        window.draw(sfText);
    }

    void render() {
        window.clear(sf::Color(40, 40, 40));

        // Draw the path
        for (size_t i = 0; i < path.size() - 1; i++) {
            sf::Vector2f start = path[i];
            sf::Vector2f end = path[i + 1];
            sf::Vector2f direction = end - start;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (length > 0) {
                direction = direction / length;
                sf::RectangleShape pathSegment;
                pathSegment.setSize(sf::Vector2f(length, 40));
                pathSegment.setPosition(start);
                pathSegment.setFillColor(sf::Color(100, 100, 100));
                
                float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
                pathSegment.setRotation(angle);
                pathSegment.setOrigin(0, 20);
                
                window.draw(pathSegment);
            }
        }

        // Path points
        for (const auto& point : path) {
            sf::CircleShape dot(3);
            dot.setPosition(point.x - 3, point.y - 3);
            dot.setFillColor(sf::Color::Yellow);
            window.draw(dot);
        }

        // Draw towers
        for (auto& tower : towers) {
            tower->draw(window);
        }

        // Draw projectiles
        for (auto& projectile : projectiles) {
            projectile.draw(window);
        }

        // Draw enemies (on top of projectiles)
        for (auto& enemy : enemies) {
            if (enemy->alive) {
                enemy->draw(window);
            }
        }

        // ==============================
        // USER INTERFACE WITH SPRITES AND ARIAL FONT
        // ==============================
        
        // Gold interface with sprite and text (using Arial font)
        resourceManager.drawGoldIcon(window, 10, 10);
        drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

        // Lives interface with sprite and text (using Arial font)
        resourceManager.drawHeartIcon(window, 10, 40);
        drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

        // Wave interface with text (using Arial font)
        drawText(window, "Wave: " + intToString(wave), 10, 75, sf::Color::Cyan, 16);

        // Score interface with text (using Arial font)
        drawText(window, "Score: " + intToString(score), 10, 105, sf::Color::White, 16);

        // Additional game information
        drawText(window, "Towers: " + intToString(towers.size()), 10, 135, sf::Color::Blue, 16);
        drawText(window, "Enemies: " + intToString(enemies.size()), 10, 165, sf::Color::Red, 16);

        // "PRESS SPACE TO START" text at center of screen (large, visible text)
        if (!waveActive && lives > 0) {
            // Semi-transparent background for text
            sf::RectangleShape textBg(sf::Vector2f(500, 80));
            textBg.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 40);
            textBg.setFillColor(sf::Color(0, 0, 0, 150));
            textBg.setOutlineColor(sf::Color::Green);
            textBg.setOutlineThickness(3);
            window.draw(textBg);
            
            // Large, bold text for better visibility
            drawText(window, "PRESS SPACE", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 25, sf::Color::Green, 32);
            drawText(window, "TO START LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
                    WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 10, sf::Color::Green, 22);
            
            // Afficher un message si c'est un nouveau niveau
            if (wave == 1) {
                drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
                        WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 40, sf::Color::Yellow, 20);
            }
        }

        // Game over screen
        if (gameOver) {
            sf::RectangleShape gameOverBg(sf::Vector2f(400, 200));
            gameOverBg.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 100);
            gameOverBg.setFillColor(sf::Color(100, 0, 0, 180));
            gameOverBg.setOutlineColor(sf::Color::White);
            gameOverBg.setOutlineThickness(3);
            window.draw(gameOverBg);
            
            drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 80, sf::Color::White, 32);
            drawText(window, "Final Score: " + intToString(score), WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 40, sf::Color::White, 20);
            
            if (waitingForName) {
                drawText(window, "Entrez votre nom:", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
                drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
                drawText(window, "Appuyez sur Entrée pour continuer", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
            }
            else if (showSaveOption) {
                drawText(window, "Voulez-vous sauvegarder votre score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
                drawText(window, "Appuyez sur S pour sauvegarder", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
                drawText(window, "Appuyez sur R pour rejouer", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
            }
            else if (showReplayOption) {
                drawText(window, "Appuyez sur R pour rejouer", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
            }
        }

        // Instructions at bottom of screen (clear and readable)
        drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade (100 and 200 gold) | SPACE: Start wave", 10, WINDOW_HEIGHT - 30, sf::Color::White, 16);

        window.display();
    }
};

int main() {
    std::cout << "Tower defense" << std::endl;
    Game game;
    game.run();
    return 0;
}