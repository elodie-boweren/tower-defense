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
#include <ctime>
#include <iomanip>

using json = nlohmann::json;

// Constants
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const float TOWER_MIN_DISTANCE = 80.0f;
const float AUTO_START_TIMER = 15.0f; // New timer for automatic start

// Structure to represent a score in the Hall of Fame
struct ScoreEntry {
    std::string name;
    int score;
    int level;
    int wave;
    std::time_t date;
    
    ScoreEntry(const std::string& n, int s, int l, int w, std::time_t d)
        : name(n), score(s), level(l), wave(w), date(d) {}
};

// ===========================================
// AUDIO MANAGER - Music and sound management
// ===========================================
class AudioManager {
private:
    sf::Music backgroundMusic1;
    sf::Music backgroundMusic2;
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
    int currentMusicTrack; // 0 = music1, 1 = music2
    
public:
    AudioManager() : music1Loaded(false), music2Loaded(false), soundsLoaded(false),
                     masterVolume(50.0f), musicVolume(50.0f), effectsVolume(70.0f), 
                     isMuted(false), currentMusicTrack(0) {
        loadAudioFiles();
    }
    
    void loadAudioFiles() {
        std::cout << "=== LOADING AUDIO FILES ===" << std::endl;
        
        // Paths to try for audio files
        std::vector<std::string> audioPaths = {
            "./assets/",
            "assets/",
            "../../assets/",
            "../assets/",
            "./"
        };
        
        // Loading background music
        for (const std::string& basePath : audioPaths) {
            if (!music1Loaded) {
                std::string music1Path = basePath + "background_music1.ogg";
                if (backgroundMusic1.openFromFile(music1Path)) {
                    music1Loaded = true;
                    backgroundMusic1.setLoop(true);
                    backgroundMusic1.setVolume(musicVolume);
                    std::cout << "✓ Music 1 loaded: " << music1Path << std::endl;
                }
            }
            
            if (!music2Loaded) {
                std::string music2Path = basePath + "background_music2.ogg";
                if (backgroundMusic2.openFromFile(music2Path)) {
                    music2Loaded = true;
                    backgroundMusic2.setLoop(true);
                    backgroundMusic2.setVolume(musicVolume);
                    std::cout << "✓ Music 2 loaded: " << music2Path << std::endl;
                }
            }
            
            // Loading sound effects
            if (!soundsLoaded) {
                std::string shootPath = basePath + "shoot.wav";
                std::string explosionPath = basePath + "explosion.wav";
                std::string upgradePath = basePath + "upgrade.wav";
                
                bool shootLoaded = shootSoundBuffer.loadFromFile(shootPath);
                bool explosionLoaded = explosionSoundBuffer.loadFromFile(explosionPath);
                bool upgradeLoaded = upgradeSoundBuffer.loadFromFile(upgradePath);
                
                if (shootLoaded || explosionLoaded || upgradeLoaded) {
                    soundsLoaded = true;
                    if (shootLoaded) {
                        shootSound.setBuffer(shootSoundBuffer);
                        shootSound.setVolume(effectsVolume);
                        std::cout << "✓ Shoot sound loaded: " << shootPath << std::endl;
                    }
                    if (explosionLoaded) {
                        explosionSound.setBuffer(explosionSoundBuffer);
                        explosionSound.setVolume(effectsVolume);
                        std::cout << "✓ Explosion sound loaded: " << explosionPath << std::endl;
                    }
                    if (upgradeLoaded) {
                        upgradeSound.setBuffer(upgradeSoundBuffer);
                        upgradeSound.setVolume(effectsVolume);
                        std::cout << "✓ Upgrade sound loaded: " << upgradePath << std::endl;
                    }
                }
            }
        }
        
        if (!music1Loaded && !music2Loaded) {
            std::cout << "⚠ NO music found! Place background_music1.ogg and/or background_music2.ogg in assets/" << std::endl;
        }
        if (!soundsLoaded) {
            std::cout << "⚠ Sound effects not found! Place shoot.wav, explosion.wav, upgrade.wav in assets/" << std::endl;
        }
        
        std::cout << "=== END AUDIO LOADING ===" << std::endl;
    }
    
    void startMusic() {
        if (isMuted) return;
        
        stopMusic(); // Stop any current music
        
        if (currentMusicTrack == 0 && music1Loaded) {
            backgroundMusic1.play();
            std::cout << "Playing music 1" << std::endl;
        } else if (currentMusicTrack == 1 && music2Loaded) {
            backgroundMusic2.play();
            std::cout << "Playing music 2" << std::endl;
        }
    }
    
    void stopMusic() {
        backgroundMusic1.stop();
        backgroundMusic2.stop();
    }
    
    void switchMusic() {
        if (music1Loaded && music2Loaded) {
            currentMusicTrack = (currentMusicTrack == 0) ? 1 : 0;
            startMusic();
            std::cout << "Switching to music " << (currentMusicTrack + 1) << std::endl;
        }
    }
    
    void toggleMute() {
        isMuted = !isMuted;
        if (isMuted) {
            stopMusic();
            std::cout << "Audio muted" << std::endl;
        } else {
            startMusic();
            std::cout << "Audio enabled" << std::endl;
        }
    }
    
    void increaseMasterVolume() {
        masterVolume = std::min(100.0f, masterVolume + 10.0f);
        updateVolumes();
        std::cout << "Master volume: " << masterVolume << "%" << std::endl;
    }
    
    void decreaseMasterVolume() {
        masterVolume = std::max(0.0f, masterVolume - 10.0f);
        updateVolumes();
        std::cout << "Master volume: " << masterVolume << "%" << std::endl;
    }
    
    void increaseMusicVolume() {
        musicVolume = std::min(100.0f, musicVolume + 10.0f);
        updateVolumes();
        std::cout << "Music volume: " << musicVolume << "%" << std::endl;
    }
    
    void decreaseMusicVolume() {
        musicVolume = std::max(0.0f, musicVolume - 10.0f);
        updateVolumes();
        std::cout << "Music volume: " << musicVolume << "%" << std::endl;
    }
    
    void increaseEffectsVolume() {
        effectsVolume = std::min(100.0f, effectsVolume + 10.0f);
        updateVolumes();
        std::cout << "Effects volume: " << effectsVolume << "%" << std::endl;
    }
    
    void decreaseEffectsVolume() {
        effectsVolume = std::max(0.0f, effectsVolume - 10.0f);
        updateVolumes();
        std::cout << "Effects volume: " << effectsVolume << "%" << std::endl;
    }
    
    void updateVolumes() {
        float finalMusicVolume = (masterVolume / 100.0f) * (musicVolume / 100.0f) * 100.0f;
        float finalEffectsVolume = (masterVolume / 100.0f) * (effectsVolume / 100.0f) * 100.0f;
        
        backgroundMusic1.setVolume(finalMusicVolume);
        backgroundMusic2.setVolume(finalMusicVolume);
        shootSound.setVolume(finalEffectsVolume);
        explosionSound.setVolume(finalEffectsVolume);
        upgradeSound.setVolume(finalEffectsVolume);
    }
    
    void playShootSound() {
        if (!isMuted && soundsLoaded) {
            shootSound.play();
        }
    }
    
    void playExplosionSound() {
        if (!isMuted && soundsLoaded) {
            explosionSound.play();
        }
    }
    
    void playUpgradeSound() {
        if (!isMuted && soundsLoaded) {
            upgradeSound.play();
        }
    }
    
    // Getters for interface
    float getMasterVolume() const { return masterVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getEffectsVolume() const { return effectsVolume; }
    bool getIsMuted() const { return isMuted; }
    int getCurrentMusicTrack() const { return currentMusicTrack; }
    bool hasMusicLoaded() const { return music1Loaded || music2Loaded; }
    bool hasBothMusicsLoaded() const { return music1Loaded && music2Loaded; }
};

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
        for (const std::string& basePath : pathsToTry) {
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
        for (const std::string& basePath : pathsToTry) {
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
// HALL OF FAME MANAGER - Score management
// ===========================================
class HallOfFameManager {
private:
    std::vector<ScoreEntry> scores;
    bool scoresLoaded;
    
public:
    HallOfFameManager() : scoresLoaded(false) {
        loadScores();
    }
    
    void loadScores() {
        scores.clear();
        scoresLoaded = false;
        
        try {
            std::ifstream file("scores.json");
            if (file.good()) {
                json scoresJson;
                file >> scoresJson;
                file.close();
                
                // Convert JSON to ScoreEntry
                for (const auto& scoreData : scoresJson) {
                    std::string name = scoreData.value("name", "Anonymous");
                    int score = scoreData.value("score", 0);
                    int level = scoreData.value("level", 1);
                    int wave = scoreData.value("wave", 1);
                    std::time_t date = scoreData.value("date", std::time(nullptr));
                    
                    scores.emplace_back(name, score, level, wave, date);
                }
                
                // Sort by descending score
                std::sort(scores.begin(), scores.end(), 
                    [](const ScoreEntry& a, const ScoreEntry& b) {
                        return a.score > b.score;
                    });
                
                scoresLoaded = true;
                std::cout << "✓ Hall of Fame loaded: " << scores.size() << " scores found" << std::endl;
            } else {
                std::cout << "ℹ No score file found (scores.json)" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "⚠ Error loading scores: " << e.what() << std::endl;
        }
    }
    
    std::vector<ScoreEntry> getTopScores(int count = 10) const {
        std::vector<ScoreEntry> topScores;
        int maxCount = std::min(count, static_cast<int>(scores.size()));
        
        for (int i = 0; i < maxCount; i++) {
            topScores.push_back(scores[i]);
        }
        
        return topScores;
    }
    
    bool hasScores() const {
        return scoresLoaded && !scores.empty();
    }
    
    int getScoreCount() const {
        return scores.size();
    }
    
    // Format a date for display
    std::string formatDate(std::time_t timestamp) const {
        std::tm* timeInfo = std::localtime(&timestamp);
        std::ostringstream oss;
        oss << std::setfill('0') 
            << std::setw(2) << timeInfo->tm_mday << "/"
            << std::setw(2) << (timeInfo->tm_mon + 1) << "/"
            << (timeInfo->tm_year + 1900);
        return oss.str();
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
    size_t pathIndex;
    std::string type;

    Enemy(int hp, float spd, int rew, const std::string& t) 
        : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0), type(t) {
        // Initial position will be set by the Game
    }

    virtual ~Enemy() = default;

    void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
        if (!alive || pathIndex >= static_cast<size_t>(path.size() - 1)) return;

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
        return pathIndex >= static_cast<size_t>(path.size() - 1);
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

class MediumEnemy : public Enemy {
public:
    MediumEnemy() : Enemy(200, 45, 35, "Medium") {}
    
    sf::Color getColor() override {
        return sf::Color(0, 128, 128); // Turquoise
    }
};

class Troll : public Enemy {
public:
    Troll() : Enemy(300, 35, 50, "Troll") {}
    
    sf::Color getColor() override {
        return sf::Color::Magenta;
    }
};

class HeavyEnemy : public Enemy {
public:
    HeavyEnemy() : Enemy(400, 30, 60, "Heavy") {}
    
    sf::Color getColor() override {
        return sf::Color(139, 69, 19); // Brown
    }
};

// Factory to create enemies
class EnemyFactory {
public:
    enum EnemyType { GOBLIN, ORC, MEDIUM, TROLL, HEAVY };
    
    // Creation of enemies per type
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
    
    // Create assortment of enemies based with difficulty increasing per level
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
    AudioManager* audioManager;

    Projectile(sf::Vector2f startPos, Enemy* target, int dmg, Subject* subject, AudioManager* audio) 
        : position(startPos), damage(dmg), speed(300.0f), active(true), targetEnemy(target), 
          gameSubject(subject), audioManager(audio) {
        
        if (target) {
            targetPosition = target->position;
            // Calculate direction towards target
            sf::Vector2f direction = targetPosition - position;
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (distance > 0) {
                velocity = (direction / distance) * speed;
            }
        }
        
        // Play shoot sound
        if (audioManager) {
            audioManager->playShootSound();
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
                
                // Play explosion sound when enemy dies
                if (targetEnemy->health <= 0 && oldHealth > 0) {
                    gameSubject->notifyObservers(GameEvent(GameEvent::ENEMY_KILLED, targetEnemy->reward));
                    if (audioManager) {
                        audioManager->playExplosionSound();
                    }
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
    virtual void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, Subject* subject, AudioManager* audioManager) {
        timeSinceLastAttack += deltaTime;
        
        if (timeSinceLastAttack >= attackCooldown) {
            Enemy* target = nullptr;
            float closestDistance = range + 1;
            
            for (std::unique_ptr<Enemy>& enemy : enemies) {
                if (!enemy->alive) continue;
                
                float distance = sqrt(pow(enemy->position.x - position.x, 2) + pow(enemy->position.y - position.y, 2));
                if (distance <= range && distance < closestDistance) {
                    target = enemy.get();
                    closestDistance = distance;
                }
            }
            
            if (target) {
                // Create a projectile to attack enemy
                projectiles.push_back(Projectile(position, target, damage, subject, audioManager));
                timeSinceLastAttack = 0;
            }
        }
    }
    // Tower attack increases with the levels
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
    std::vector<std::unique_ptr<Tower>> towers;
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
    AudioManager audioManager;
    HallOfFameManager hallOfFameManager;
    bool gameOver;
    bool showReplayOption;
    bool showSaveOption;
    std::string playerName;
    bool waitingForName;
    bool showAudioControls;
    bool showHallOfFame;
    bool isPaused;
    float autoStartTimer;  // New timer for automatic start
    
    // Preset paths
    const std::vector<std::vector<sf::Vector2f>> predefinedPaths = {
        // Original path
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
        // Zigzag path
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
        // Curly path
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
        // S path
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
        // Select a path different from the previous randomly
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
                // File exists, read it
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
            
            // Reload Hall of Fame after saving
            hallOfFameManager.loadScores();
            
            std::cout << "Score saved successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error saving score: " << e.what() << std::endl;
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
        showAudioControls = false;
        showHallOfFame = false;
        autoStartTimer = AUTO_START_TIMER;  // Reset timer
        
        // Restart music
        audioManager.startMusic();
    }

public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Laura, Louis and Elodie's Tower Defense"),
             gold(200), lives(3), wave(1), level(1), enemySpawnTimer(0), 
             enemiesInWave(5), enemiesSpawned(0), waveActive(false), score(0),
             gameOver(false), showReplayOption(false), showSaveOption(false),
             playerName(""), waitingForName(false), showAudioControls(false), 
             showHallOfFame(false), isPaused(false), autoStartTimer(AUTO_START_TIMER) {
        
        // Initialize the random number generator 
        srand(time(nullptr));
        
        std::cout << "=== TOWER DEFENSE GAME ===" << std::endl;
        std::cout << "✓ Factory Pattern: Enemy creation" << std::endl;
        std::cout << "✓ Factory Pattern: Tower creation" << std::endl;
        std::cout << "✓ Observer Pattern: Event management" << std::endl;
        std::cout << "✓ Resource Manager: Sprites and font management" << std::endl;
        std::cout << "✓ Audio Manager: Music and sound effects" << std::endl;
        std::cout << "✓ Hall of Fame Manager: Score management" << std::endl;
        std::cout << "✓ Auto-Wave Timer: Automatic start after 15s" << std::endl;
        
        // Subscribe to its own events
        addObserver(this);
        
        // Initialize the first path
        path = predefinedPaths[0];
        
        // Start music
        audioManager.startMusic();
        
        std::cout << "GAME CONTROLS:" << std::endl;
        std::cout << "- Left click: Place a tower" << std::endl;
        std::cout << "- Click on tower: Upgrade" << std::endl;
        std::cout << "- SPACE: Start wave (or wait 15s)" << std::endl;
        std::cout << "- A: Show/hide audio controls" << std::endl;
        std::cout << "- H: Show/hide Hall of Fame" << std::endl;
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
                autoStartTimer = AUTO_START_TIMER;  // Reset timer for next wave
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
                // Audio controls
                if (event.key.code == sf::Keyboard::M) {
                    audioManager.toggleMute();
                }
                else if (event.key.code == sf::Keyboard::N && audioManager.hasBothMusicsLoaded()) {
                    audioManager.switchMusic();
                }
                else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                    audioManager.increaseMasterVolume();
                }
                else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
                    audioManager.decreaseMasterVolume();
                }
                else if (event.key.code == sf::Keyboard::Num1) {
                    audioManager.increaseMusicVolume();
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    audioManager.decreaseMusicVolume();
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    audioManager.increaseEffectsVolume();
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    audioManager.decreaseEffectsVolume();
                }
                else if (event.key.code == sf::Keyboard::A) {
                    showAudioControls = !showAudioControls;
                }
                else if (event.key.code == sf::Keyboard::H) {
                    showHallOfFame = !showHallOfFame;
                    if (showHallOfFame) {
                        hallOfFameManager.loadScores(); // Reload scores
                    }
                }
                // Game controls
                else if (event.key.code == sf::Keyboard::Return && waitingForName) {
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
                else if (event.key.code == sf::Keyboard::P && !gameOver) {
                    isPaused = !isPaused;
                    if (isPaused) {
                        audioManager.stopMusic();
                    } else {
                        audioManager.startMusic();
                    }
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
                // Check click on existing tower
                bool towerClicked = false;
                for (std::unique_ptr<Tower>& tower : towers) {
                    float distance = sqrt(pow(clickPos.x - tower->position.x, 2) + pow(clickPos.y - tower->position.y, 2));
                    if (distance <= 20 && tower->canUpgrade() && gold >= tower->getUpgradeCost()) {
                        gold -= tower->getUpgradeCost();
                        tower->upgrade();
                        audioManager.playUpgradeSound(); // Uograde sound
                        towerClicked = true;
                        std::cout << "Tower upgraded to level " << tower->level << std::endl;
                        break;
                    }
                }
                
                // New tower placement
                if (!towerClicked) {
                    int cost = 50;
                    if (gold >= cost) {
                        bool canPlace = true;
                        
                        // Check distance with other towers
                        for (const std::unique_ptr<Tower>& tower : towers) {
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
                            std::unique_ptr<Tower> tower = TowerFactory::createTower(TowerType::BASIC, clickPos);
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
        // Stop updating if game is over or paused
        if (isPaused || lives <= 0 || wave > 10) {
            return;
        }
        
        // Timer to automatically start the wave
        if (!waveActive && !gameOver && lives > 0) {
            autoStartTimer -= deltaTime;
            if (autoStartTimer <= 0) {
                startWave();
            }
        }
        
        // Spawn enemies with Factory Pattern
        if (waveActive && enemiesSpawned < enemiesInWave) {
            enemySpawnTimer += deltaTime;
            if (enemySpawnTimer >= 1.0f) {
                // Using Factory Pattern with level-based difficulty
                EnemyFactory::EnemyType type = EnemyFactory::getEnemyTypeForWave(wave, level, enemiesSpawned);
                std::unique_ptr<Enemy> enemy = EnemyFactory::createEnemy(type);
                
                // Define initial position at beginning of path
                enemy->position = path[0];
                
                // Increase enemy statistics based on the level and the wave
                float levelMultiplier = 1.0f + (level - 1) * 0.15f;
                float waveMultiplier = 1.0f + (wave - 1) * 0.05f;
                float totalMultiplier = levelMultiplier * waveMultiplier;
                
                // Apply multipliers to other stats
                enemy->health = static_cast<int>(enemy->health * totalMultiplier);
                enemy->maxHealth = enemy->health;
                enemy->speed = enemy->speed * (1.0f + (level - 1) * 0.03f + (wave - 1) * 0.01f);
                enemy->reward = static_cast<int>(enemy->reward * (1.0f + (level - 1) * 0.1f + (wave - 1) * 0.05f));
                
                enemies.push_back(std::move(enemy));
                
                enemiesSpawned++;
                enemySpawnTimer = 0;
            }
        }

        // Update enemies
        for (std::unique_ptr<Enemy>& enemy : enemies) {
            if (enemy->alive) {
                enemy->update(deltaTime, path);
                
                if (enemy->reachedEnd(path)) {
                    // Using Observer Pattern
                    notifyObservers(GameEvent(GameEvent::ENEMY_REACHED_END, 1));
                    enemy->alive = false;
                }
            }
        }

        // Update towers (now they create projectiles with audio)
        for (std::unique_ptr<Tower>& tower : towers) {
            tower->update(deltaTime, enemies, projectiles, this, &audioManager);
        }

        // Update projectiles
        for (Projectile& projectile : projectiles) {
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
            
            // Change the level after 3 waves
            if (wave > 3) {
                level++;
                wave = 1;
                enemiesInWave = 5 + (level - 1) * 2;
                gold += 100 * level;
                
                // Change the path and remove towers
                changePath();
                towers.clear();
                
                std::cout << "=== LEVEL " << level << " STARTED ===" << std::endl;
                std::cout << "New path selected - Towers have been removed" << std::endl;
            } else {
                enemiesInWave += 2;
            }
            std::cout << "Press SPACE for level " << level << " wave " << wave << " (or wait " << static_cast<int>(autoStartTimer) << "s)" << std::endl;
        }

        // Check for game over
        if (lives <= 0) {
            gameOver = true;
            showReplayOption = true;
            waitingForName = true;
            audioManager.stopMusic(); // Stop music in case of game over
        }
    }

    // Convert integer to string for display
    std::string intToString(int value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    std::string floatToString(float value) {
        std::ostringstream oss;
        oss << static_cast<int>(value);
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
        for (const sf::Vector2f& point : path) {
            sf::CircleShape dot(3);
            dot.setPosition(point.x - 3, point.y - 3);
            dot.setFillColor(sf::Color::Yellow);
            window.draw(dot);
        }

        // Draw towers
        for (std::unique_ptr<Tower>& tower : towers) {
            tower->draw(window);
        }

        // Draw projectiles
        for (Projectile& projectile : projectiles) {
            projectile.draw(window);
        }

        // Draw enemies (on top of projectiles)
        for (std::unique_ptr<Enemy>& enemy : enemies) {
            if (enemy->alive) {
                enemy->draw(window);
            }
        }

        // ==============================
        // USER INTERFACE WITH SPRITES AND ARIAL FONT
        // ==============================
        
        // Gold interface with sprite and text
        resourceManager.drawGoldIcon(window, 10, 10);
        drawText(window, intToString(gold), 40, 15, sf::Color::Yellow, 18);

        // Lives interface with sprite and text
        resourceManager.drawHeartIcon(window, 10, 40);
        drawText(window, intToString(lives), 40, 45, sf::Color::Red, 18);

        // Level interface
        drawText(window, "Level : " + intToString(level), 300, 15, sf::Color::White, 18);

        // Wave interface
        drawText(window, "Wave : " + intToString(wave), 300, 45, sf::Color::Cyan, 18);

        // Additional game information
        drawText(window, "Towers : " + intToString(towers.size()), 600, 15, sf::Color::Blue, 18);
        drawText(window, "Enemies : " + intToString(enemies.size()), 600, 45, sf::Color::Red, 18);

        // Score interface
        drawText(window, "Score : " + intToString(score), 850, 15, sf::Color::White, 18);

        // Audio status indicator
        if (audioManager.getIsMuted()) {
            drawText(window, "MUTE", 850, 45, sf::Color::Red, 16);
        } else {
            std::string musicText = "Music " + intToString(audioManager.getCurrentMusicTrack() + 1);
            drawText(window, musicText, 850, 45, sf::Color::Green, 16);
        }

        // Audio controls panel (toggle with 'A')
        if (showAudioControls) {
            // Semi-transparent background 
            sf::RectangleShape controlsBg(sf::Vector2f(300, 280));
            controlsBg.setPosition(10, WINDOW_HEIGHT - 320); 
            controlsBg.setFillColor(sf::Color(0, 0, 0, 180));
            controlsBg.setOutlineColor(sf::Color::White);
            controlsBg.setOutlineThickness(2);
            window.draw(controlsBg);
            
           float baseY = WINDOW_HEIGHT - 310; // To align the text
            drawText(window, "=== AUDIO CONTROLS ===", 20, baseY, sf::Color::White, 16);
            drawText(window, "M: Mute/Unmute", 20, baseY + 20, sf::Color::Cyan, 14);
            if (audioManager.hasBothMusicsLoaded()) {
                drawText(window, "N: Change music", 20, baseY + 40, sf::Color::Cyan, 14);
            }
            drawText(window, "+/-: Main volume (" + floatToString(audioManager.getMasterVolume()) + "%)", 20, baseY + 60, sf::Color::Yellow, 14);
            drawText(window, "1/2: Music volume (" + floatToString(audioManager.getMusicVolume()) + "%)", 20, baseY + 80, sf::Color::Yellow, 14);
            drawText(window, "3/4: Sound effect volume (" + floatToString(audioManager.getEffectsVolume()) + "%)", 20, baseY + 100, sf::Color::Yellow, 14);
            drawText(window, "A: Hide this panel", 20, baseY + 120, sf::Color::White, 14);
            
            if (audioManager.hasMusicLoaded()) {
                std::string currentMusic = "Current music: " + intToString(audioManager.getCurrentMusicTrack() + 1);
                drawText(window, currentMusic, 20, baseY + 150, sf::Color::Green, 14);
            } else {
                drawText(window, "No music loaded", 20, baseY + 150, sf::Color::Red, 14);
            }
            
            drawText(window, "A: Hide this panel", 20, baseY + 190, sf::Color::Magenta, 14);
        }

        // Hall of Fame panel (toggle with 'H')
        if (showHallOfFame) {
            // Semi-transparent panel for the Hall of Fame
            sf::RectangleShape hallOfFameBg(sf::Vector2f(450, 400));
            hallOfFameBg.setPosition(530, 80);
            hallOfFameBg.setFillColor(sf::Color(0, 0, 0, 200));
            hallOfFameBg.setOutlineColor(sf::Color(255, 215, 0)); 
            hallOfFameBg.setOutlineThickness(3);
            window.draw(hallOfFameBg);
            
            // Hall of Fame title
            drawText(window, "=== HALL OF FAME ===", 550, 95, sf::Color(255, 215, 0), 20); // Gold color
            
            if (hallOfFameManager.hasScores()) {
                std::vector<ScoreEntry> topScores = hallOfFameManager.getTopScores(15);

                // Headers of columns with fixed sizes
                drawText(window, "Rank", 560, 125, sf::Color::White, 14);
                drawText(window, "Name", 600, 125, sf::Color::White, 14);
                drawText(window, "Score", 750, 125, sf::Color::White, 14);
                drawText(window, "Level", 820, 125, sf::Color::White, 14);
                drawText(window, "Date", 880, 125, sf::Color::White, 14);
                drawText(window, "------------------------------------------------", 550, 145, sf::Color(128, 128, 128), 14);
              
                // Display scores
                for (size_t i = 0; i < topScores.size(); i++) {
                    const ScoreEntry& entry = topScores[i];
                    float yPos = 165 + i * 18;
                    
                    // Color based on the rank
                    sf::Color rankColor = sf::Color::White;
                    if (i == 0) rankColor = sf::Color(255, 215, 0);       // 1st place (gold)
                    else if (i == 1) rankColor = sf::Color(192, 192, 192); // 2nd place (silver)
                    else if (i == 2) rankColor = sf::Color(205, 127, 50);  // 3rd place (bronze)
                    
                    // Format data with fixed alignment
                    std::string rank = intToString(i + 1);
                    
                    std::string name = entry.name;
                    if (name.length() > 15) name = name.substr(0, 15); // Cut name if too long
                    
                    std::string scoreStr = intToString(entry.score);
                    
                    std::string levelStr = "L" + intToString(entry.level) + "W" + intToString(entry.wave);
                    
                    std::string date = hallOfFameManager.formatDate(entry.date);
                    
                    // Display of each column with fixed size
                    drawText(window, rank, 560, yPos, rankColor, 12);        // Rank column
                    drawText(window, name, 600, yPos, rankColor, 12);        // Name column
                    drawText(window, scoreStr, 750, yPos, rankColor, 12);    // Score column
                    drawText(window, levelStr, 820, yPos, rankColor, 12);    // Level column
                    drawText(window, date, 880, yPos, rankColor, 12);       // Date column
                }
                
                // Information at the bottoù
                std::string totalScoresText = "Total scores saved: " + intToString(hallOfFameManager.getScoreCount());
                drawText(window, totalScoresText, 550, 430, sf::Color::Cyan, 12);
                
            } else {
                // No score available
                drawText(window, "No score is available", 550, 150, sf::Color(128, 128, 128), 16); // Couleur grise
                drawText(window, "", 550, 180, sf::Color::White, 14);
                drawText(window, "Play and save your score", 550, 200, sf::Color::White, 14);
                drawText(window, "to have it show here!", 550, 220, sf::Color::White, 14);
            }
            
            drawText(window, "H: Hide this pannel", 550, 455, sf::Color::White, 14);
        }

        // "PRESS SPACE TO START" text at center of screen 
        if (!waveActive && lives > 0) {
            // Semi-transparent background for text
            sf::RectangleShape textBg(sf::Vector2f(500, 120)); 
            textBg.setPosition(WINDOW_WIDTH/2 - 250, WINDOW_HEIGHT/2 - 60);
            textBg.setFillColor(sf::Color(0, 0, 0, 150));
            textBg.setOutlineColor(sf::Color::Green);
            textBg.setOutlineThickness(3);
            window.draw(textBg);
            
            // Large, bold text for better visibility
            drawText(window, "PRESS SPACE", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 50, sf::Color::Green, 32);
            drawText(window, "TO START LEVEL " + intToString(level) + " WAVE " + intToString(wave), 
                    WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 10, sf::Color::Green, 22);
            
            // Display next wave timer 
            int secondsLeft = static_cast<int>(autoStartTimer) + 1;
            if (secondsLeft > 0) {
                std::string timerText = "Auto-start in " + intToString(secondsLeft) + "s";
                drawText(window, timerText, WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 15, sf::Color::Yellow, 18);
            }
            
            // Display message for each new level
            if (wave == 1) {
                drawText(window, "NEW PATH - PLACE YOUR TOWERS!", 
                        WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 + 35, sf::Color::Yellow, 16);
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
                drawText(window, "Enter your name :", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2, sf::Color::White, 20);
                drawText(window, playerName + "|", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
                drawText(window, "Press Enter to continue", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 + 60, sf::Color::White, 16);
            }
            else if (showSaveOption) {
                drawText(window, "Save score?", WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2, sf::Color::White, 20);
                drawText(window, "Press S to save", WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 + 30, sf::Color::Green, 16);
                drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 60, sf::Color::Yellow, 16);
            }
            else if (showReplayOption) {
                drawText(window, "Press R to play again", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 30, sf::Color::Yellow, 20);
            }
        }

        // Display pause state
        if (isPaused) {
            sf::RectangleShape pauseBg(sf::Vector2f(300, 100));
            pauseBg.setPosition(WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 50);
            pauseBg.setFillColor(sf::Color(0, 0, 0, 150));
            pauseBg.setOutlineColor(sf::Color::Yellow);
            pauseBg.setOutlineThickness(3);
            window.draw(pauseBg);
            
            drawText(window, "PAUSE", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 - 30, sf::Color::Yellow, 32);
            drawText(window, "Press P to resume", WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 + 10, sf::Color::White, 20);
        }

        // Instructions at bottom of screen (clear and readable)
        drawText(window, "Click: Place tower (50 gold) | Click on tower: Upgrade (100 gold then 200 gold) | SPACE: Start wave | A: Audio | H: Hall of Fame | P: Pause", 
                10, WINDOW_HEIGHT - 30, sf::Color::White, 15);

        window.display();
    }
};

int main() {
    std::cout << "Laura, Louis and Elodie's Tower Defense" << std::endl;
    std::cout << "=======================================" << std::endl;
    Game game;
    game.run();
    return 0;
}