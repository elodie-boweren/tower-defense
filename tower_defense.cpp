// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>

// // Constantes
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;

// // Classes simplifiées
// class Enemy {
// public:
//     sf::Vector2f position;
//     float speed;
//     int health;
//     int maxHealth;
//     int reward;
//     bool alive;
//     int pathIndex;

//     Enemy(int hp, float spd, int rew) : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0) {
//         position = sf::Vector2f(0, 300); // Position de départ
//     }

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= path.size() - 1) return;

//         // Se déplacer vers le prochain point du chemin
//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             direction = direction / distance; // Normaliser
//             position += direction * speed * deltaTime;
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         // Couleur selon la santé
//         if (health > maxHealth * 0.7f) shape.setFillColor(sf::Color::Green);
//         else if (health > maxHealth * 0.3f) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);

//         // Barre de vie simple
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

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1) {}

//     void update(float deltaTime, std::vector<Enemy>& enemies) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             // Chercher l'ennemi le plus proche dans la portée
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (auto& enemy : enemies) {
//                 if (!enemy.alive) continue;
                
//                 float distance = sqrt(pow(enemy.position.x - position.x, 2) + pow(enemy.position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = &enemy;
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 target->takeDamage(damage);
//                 timeSinceLastAttack = 0;
//                 std::cout << "Tour attaque! Dégâts: " << damage << std::endl;
//             }
//         }
//     }

//     void upgrade() {
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

//     void draw(sf::RenderWindow& window) {
//         // Cercle de portée (transparent)
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         // Tour elle-même
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }

//     int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0; // Niveau max
//     }

//     bool canUpgrade() {
//         return level < 3;
//     }
// };

// class Game {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<Enemy> enemies;
//     std::vector<Tower> towers;
//     int gold;
//     int lives;
//     int wave;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - Version Simple"),
//              gold(200), lives(20), wave(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false) {
        
//         std::cout << "=== TOWER DEFENSE DÉMARRE ===" << std::endl;
        
//         // Créer un chemin simple (ligne droite avec virages)
//         path.push_back(sf::Vector2f(0, 300));
//         path.push_back(sf::Vector2f(200, 300));
//         path.push_back(sf::Vector2f(200, 150));
//         path.push_back(sf::Vector2f(500, 150));
//         path.push_back(sf::Vector2f(500, 450));
//         path.push_back(sf::Vector2f(800, 450));
//         path.push_back(sf::Vector2f(800, 250));
//         path.push_back(sf::Vector2f(1000, 250));
        
//         std::cout << "Chemin créé avec " << path.size() << " points" << std::endl;
//         std::cout << "Or initial: " << gold << ", Vies: " << lives << std::endl;
//         std::cout << "CONTRÔLES:" << std::endl;
//         std::cout << "- Clic gauche: Placer tour (50 or)" << std::endl;
//         std::cout << "- Clic sur tour: Améliorer" << std::endl;
//         std::cout << "- Appuyez sur ESPACE pour démarrer la vague" << std::endl;
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
            
//             if (event.type == sf::Event::KeyPressed) {
//                 if (event.key.code == sf::Keyboard::Space && !waveActive) {
//                     startWave();
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                 sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
//                 // Vérifier si on clique sur une tour existante
//                 bool towerClicked = false;
//                 for (auto& tower : towers) {
//                     float distance = sqrt(pow(clickPos.x - tower.position.x, 2) + pow(clickPos.y - tower.position.y, 2));
//                     if (distance <= 20 && tower.canUpgrade() && gold >= tower.getUpgradeCost()) {
//                         gold -= tower.getUpgradeCost();
//                         tower.upgrade();
//                         towerClicked = true;
//                         std::cout << "Tour améliorée au niveau " << tower.level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // Sinon, essayer de placer une nouvelle tour
//                 if (!towerClicked && gold >= 50) {
//                     bool canPlace = true;
                    
//                     // Vérifier qu'on ne place pas sur une autre tour
//                     for (const auto& tower : towers) {
//                         float distance = sqrt(pow(clickPos.x - tower.position.x, 2) + pow(clickPos.y - tower.position.y, 2));
//                         if (distance < 50) {
//                             canPlace = false;
//                             break;
//                         }
//                     }
                    
//                     // Vérifier qu'on ne place pas sur le chemin
//                     for (const auto& pathPoint : path) {
//                         float distance = sqrt(pow(clickPos.x - pathPoint.x, 2) + pow(clickPos.y - pathPoint.y, 2));
//                         if (distance < 40) {
//                             canPlace = false;
//                             break;
//                         }
//                     }
                    
//                     if (canPlace) {
//                         towers.push_back(Tower(clickPos));
//                         gold -= 50;
//                         std::cout << "Tour placée! Or restant: " << gold << std::endl;
//                     } else {
//                         std::cout << "Impossible de placer ici!" << std::endl;
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== VAGUE " << wave << " COMMENCE ===" << std::endl;
//         std::cout << "Ennemis à venir: " << enemiesInWave << std::endl;
//     }

//     void update(float deltaTime) {
//         // Spawn des ennemis si vague active
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) { // Un ennemi par seconde
//                 // Créer différents types d'ennemis selon la vague
//                 if (wave <= 3) {
//                     enemies.push_back(Enemy(50, 60, 10)); // Gobelin
//                 } else if (wave <= 6) {
//                     if (enemiesSpawned % 2 == 0) {
//                         enemies.push_back(Enemy(120, 45, 20)); // Orc
//                     } else {
//                         enemies.push_back(Enemy(50, 60, 10)); // Gobelin
//                     }
//                 } else {
//                     if (enemiesSpawned % 3 == 0) {
//                         enemies.push_back(Enemy(300, 35, 50)); // Troll
//                     } else {
//                         enemies.push_back(Enemy(120, 45, 20)); // Orc
//                     }
//                 }
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//                 std::cout << "Ennemi spawné (" << enemiesSpawned << "/" << enemiesInWave << ")" << std::endl;
//             }
//         }

//         // Mettre à jour les ennemis
//         for (auto& enemy : enemies) {
//             if (enemy.alive) {
//                 enemy.update(deltaTime, path);
                
//                 // Vérifier si l'ennemi a atteint la fin
//                 if (enemy.reachedEnd(path)) {
//                     lives--;
//                     enemy.alive = false;
//                     std::cout << "Un ennemi a passé! Vies restantes: " << lives << std::endl;
//                 }
//             }
//         }

//         // Mettre à jour les tours
//         for (auto& tower : towers) {
//             tower.update(deltaTime, enemies);
//         }

//         // Supprimer les ennemis morts et donner de l'or
//         for (auto it = enemies.begin(); it != enemies.end();) {
//             if (!it->alive) {
//                 if (it->health <= 0) { // Tué par une tour
//                     gold += it->reward;
//                     std::cout << "Ennemi éliminé! +" << it->reward << " or (Total: " << gold << ")" << std::endl;
//                 }
//                 it = enemies.erase(it);
//             } else {
//                 ++it;
//             }
//         }

//         // Vérifier fin de vague
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             wave++;
//             enemiesInWave += 2; // Plus d'ennemis à chaque vague
//             std::cout << "=== VAGUE " << (wave-1) << " TERMINÉE ===" << std::endl;
//             std::cout << "Appuyez sur ESPACE pour la vague " << wave << std::endl;
            
//             if (wave > 10) {
//                 std::cout << "=== VICTOIRE! TOUTES LES VAGUES COMPLÉTÉES ===" << std::endl;
//             }
//         }

//         // Vérifier game over
//         if (lives <= 0) {
//             std::cout << "=== GAME OVER ===" << std::endl;
//         }
//     }

//     void render() {
//         window.clear(sf::Color(20, 20, 20)); // Gris foncé

//         // Dessiner le chemin
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vertex line[] = {
//                 sf::Vertex(path[i], sf::Color::White),
//                 sf::Vertex(path[i + 1], sf::Color::White)
//             };
//             window.draw(line, 2, sf::Lines);
//         }

//         // Points du chemin
//         for (const auto& point : path) {
//             sf::CircleShape dot(5);
//             dot.setPosition(point.x - 5, point.y - 5);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Dessiner les tours
//         for (auto& tower : towers) {
//             tower.draw(window);
//         }

//         // Dessiner les ennemis
//         for (auto& enemy : enemies) {
//             if (enemy.alive) {
//                 enemy.draw(window);
//             }
//         }

//         // Interface simple (rectangles colorés)
//         sf::RectangleShape goldBox(sf::Vector2f(150, 30));
//         goldBox.setPosition(10, 10);
//         goldBox.setFillColor(sf::Color::Yellow);
//         window.draw(goldBox);

//         sf::RectangleShape livesBox(sf::Vector2f(150, 30));
//         livesBox.setPosition(10, 50);
//         livesBox.setFillColor(sf::Color::Red);
//         window.draw(livesBox);

//         sf::RectangleShape waveBox(sf::Vector2f(150, 30));
//         waveBox.setPosition(10, 90);
//         waveBox.setFillColor(sf::Color::Blue);
//         window.draw(waveBox);

//         if (!waveActive && wave <= 10) {
//             sf::RectangleShape startBox(sf::Vector2f(200, 40));
//             startBox.setPosition(WINDOW_WIDTH/2 - 100, 50);
//             startBox.setFillColor(sf::Color::Green);
//             window.draw(startBox);
//         }

//         if (wave > 10) {
//             sf::RectangleShape victoryBox(sf::Vector2f(300, 60));
//             victoryBox.setPosition(WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 30);
//             victoryBox.setFillColor(sf::Color::Green);
//             window.draw(victoryBox);
//         }

//         if (lives <= 0) {
//             sf::RectangleShape gameOverBox(sf::Vector2f(300, 60));
//             gameOverBox.setPosition(WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 30);
//             gameOverBox.setFillColor(sf::Color::Red);
//             window.draw(gameOverBox);
//         }

//         window.display();
//     }
// };

// int main() {
//     std::cout << "Démarrage du Tower Defense..." << std::endl;
//     Game game;
//     game.run();
//     std::cout << "Jeu terminé." << std::endl;
//     return 0;
// }

// Ci-dessus version fonctionnelle basique

// Ci dessous version fonctionnelle un peu + élaborée ---------------------------------------------
// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <sstream>

// // Constantes
// const int WINDOW_WIDTH = 1000;
// const int WINDOW_HEIGHT = 700;
// const float TOWER_MIN_DISTANCE = 80.0f; // Distance minimale entre les tours

// // Classes simplifiées
// class Enemy {
// public:
//     sf::Vector2f position;
//     float speed;
//     int health;
//     int maxHealth;
//     int reward;
//     bool alive;
//     int pathIndex;

//     Enemy(int hp, float spd, int rew) : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), pathIndex(0) {
//         position = sf::Vector2f(0, 300); // Position de départ
//     }

//     void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
//         if (!alive || pathIndex >= path.size() - 1) return;

//         // Se déplacer vers le prochain point du chemin
//         sf::Vector2f target = path[pathIndex + 1];
//         sf::Vector2f direction = target - position;
//         float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

//         if (distance < 10.0f) {
//             pathIndex++;
//         } else {
//             direction = direction / distance; // Normaliser
//             position += direction * speed * deltaTime;
//         }
//     }

//     void takeDamage(int damage) {
//         health -= damage;
//         if (health <= 0) {
//             alive = false;
//         }
//     }

//     void draw(sf::RenderWindow& window) {
//         sf::CircleShape shape(15);
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         // Couleur selon la santé
//         if (health > maxHealth * 0.7f) shape.setFillColor(sf::Color::Green);
//         else if (health > maxHealth * 0.3f) shape.setFillColor(sf::Color::Yellow);
//         else shape.setFillColor(sf::Color::Red);
        
//         window.draw(shape);

//         // Barre de vie simple
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

// class Tower {
// public:
//     sf::Vector2f position;
//     int damage;
//     float range;
//     float attackCooldown;
//     float timeSinceLastAttack;
//     int level;

//     Tower(sf::Vector2f pos) : position(pos), damage(20), range(80), attackCooldown(1.0f), timeSinceLastAttack(0), level(1) {}

//     void update(float deltaTime, std::vector<Enemy>& enemies) {
//         timeSinceLastAttack += deltaTime;
        
//         if (timeSinceLastAttack >= attackCooldown) {
//             // Chercher l'ennemi le plus proche dans la portée
//             Enemy* target = nullptr;
//             float closestDistance = range + 1;
            
//             for (auto& enemy : enemies) {
//                 if (!enemy.alive) continue;
                
//                 float distance = sqrt(pow(enemy.position.x - position.x, 2) + pow(enemy.position.y - position.y, 2));
//                 if (distance <= range && distance < closestDistance) {
//                     target = &enemy;
//                     closestDistance = distance;
//                 }
//             }
            
//             if (target) {
//                 target->takeDamage(damage);
//                 timeSinceLastAttack = 0;
//                 std::cout << "Tour attaque! Dégâts: " << damage << std::endl;
//             }
//         }
//     }

//     void upgrade() {
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

//     void draw(sf::RenderWindow& window) {
//         // Cercle de portée (transparent)
//         sf::CircleShape rangeCircle(range);
//         rangeCircle.setPosition(position.x - range, position.y - range);
//         rangeCircle.setFillColor(sf::Color::Transparent);
//         rangeCircle.setOutlineColor(sf::Color(100, 100, 100, 100));
//         rangeCircle.setOutlineThickness(1);
//         window.draw(rangeCircle);

//         // Tour elle-même
//         sf::RectangleShape shape(sf::Vector2f(30, 30));
//         shape.setPosition(position.x - 15, position.y - 15);
        
//         if (level == 1) shape.setFillColor(sf::Color::Blue);
//         else if (level == 2) shape.setFillColor(sf::Color::Magenta);
//         else shape.setFillColor(sf::Color::Cyan);
        
//         window.draw(shape);
//     }

//     int getUpgradeCost() {
//         if (level == 1) return 100;
//         if (level == 2) return 200;
//         return 0; // Niveau max
//     }

//     bool canUpgrade() {
//         return level < 3;
//     }
// };

// class Game {
// private:
//     sf::RenderWindow window;
//     std::vector<sf::Vector2f> path;
//     std::vector<Enemy> enemies;
//     std::vector<Tower> towers;
//     int gold;
//     int lives;
//     int wave;
//     float enemySpawnTimer;
//     int enemiesInWave;
//     int enemiesSpawned;
//     bool waveActive;
//     sf::Font font; // Pour afficher le texte
//     bool fontLoaded;

// public:
//     Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense - Version Améliorée"),
//              gold(200), lives(20), wave(1), enemySpawnTimer(0), 
//              enemiesInWave(5), enemiesSpawned(0), waveActive(false), fontLoaded(false) {
        
//         std::cout << "=== TOWER DEFENSE DÉMARRE ===" << std::endl;
        
//         // Essayer de charger une police (optionnel)
//         if (!font.loadFromFile("arial.ttf")) {
//             std::cout << "Police non trouvée, utilisation de la police par défaut" << std::endl;
//         } else {
//             fontLoaded = true;
//         }
        
//         // Créer un chemin simple (ligne droite avec virages)
//         path.push_back(sf::Vector2f(0, 300));
//         path.push_back(sf::Vector2f(200, 300));
//         path.push_back(sf::Vector2f(200, 150));
//         path.push_back(sf::Vector2f(500, 150));
//         path.push_back(sf::Vector2f(500, 450));
//         path.push_back(sf::Vector2f(800, 450));
//         path.push_back(sf::Vector2f(800, 250));
//         path.push_back(sf::Vector2f(1000, 250));
        
//         std::cout << "Chemin créé avec " << path.size() << " points" << std::endl;
//         std::cout << "Or initial: " << gold << ", Vies: " << lives << std::endl;
//         std::cout << "CONTRÔLES:" << std::endl;
//         std::cout << "- Clic gauche: Placer tour (50 or)" << std::endl;
//         std::cout << "- Clic sur tour: Améliorer" << std::endl;
//         std::cout << "- Appuyez sur ESPACE pour démarrer la vague" << std::endl;
//         std::cout << "- Distance minimale entre tours: " << TOWER_MIN_DISTANCE << " pixels" << std::endl;
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
            
//             if (event.type == sf::Event::KeyPressed) {
//                 if (event.key.code == sf::Keyboard::Space && !waveActive) {
//                     startWave();
//                 }
//             }
            
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//                 sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
//                 // Vérifier si on clique sur une tour existante
//                 bool towerClicked = false;
//                 for (auto& tower : towers) {
//                     float distance = sqrt(pow(clickPos.x - tower.position.x, 2) + pow(clickPos.y - tower.position.y, 2));
//                     if (distance <= 20 && tower.canUpgrade() && gold >= tower.getUpgradeCost()) {
//                         gold -= tower.getUpgradeCost();
//                         tower.upgrade();
//                         towerClicked = true;
//                         std::cout << "Tour améliorée au niveau " << tower.level << std::endl;
//                         break;
//                     }
//                 }
                
//                 // Sinon, essayer de placer une nouvelle tour
//                 if (!towerClicked && gold >= 50) {
//                     bool canPlace = true;
                    
//                     // Vérifier qu'on ne place pas trop près d'une autre tour
//                     for (const auto& tower : towers) {
//                         float distance = sqrt(pow(clickPos.x - tower.position.x, 2) + pow(clickPos.y - tower.position.y, 2));
//                         if (distance < TOWER_MIN_DISTANCE) {
//                             canPlace = false;
//                             std::cout << "Trop proche d'une autre tour! Distance minimale: " << TOWER_MIN_DISTANCE << std::endl;
//                             break;
//                         }
//                     }
                    
//                     // Vérifier qu'on ne place pas sur le chemin (zone élargie)
//                     for (size_t i = 0; i < path.size() - 1; i++) {
//                         sf::Vector2f start = path[i];
//                         sf::Vector2f end = path[i + 1];
                        
//                         // Distance du point au segment de ligne
//                         float A = clickPos.x - start.x;
//                         float B = clickPos.y - start.y;
//                         float C = end.x - start.x;
//                         float D = end.y - start.y;
                        
//                         float dot = A * C + B * D;
//                         float lenSq = C * C + D * D;
//                         float param = (lenSq != 0) ? dot / lenSq : -1;
                        
//                         float xx, yy;
//                         if (param < 0) {
//                             xx = start.x;
//                             yy = start.y;
//                         } else if (param > 1) {
//                             xx = end.x;
//                             yy = end.y;
//                         } else {
//                             xx = start.x + param * C;
//                             yy = start.y + param * D;
//                         }
                        
//                         float distance = sqrt(pow(clickPos.x - xx, 2) + pow(clickPos.y - yy, 2));
//                         if (distance < 50) { // Zone interdite autour du chemin
//                             canPlace = false;
//                             std::cout << "Trop proche du chemin!" << std::endl;
//                             break;
//                         }
//                     }
                    
//                     if (canPlace) {
//                         towers.push_back(Tower(clickPos));
//                         gold -= 50;
//                         std::cout << "Tour placée! Or restant: " << gold << std::endl;
//                     }
//                 }
//             }
//         }
//     }

//     void startWave() {
//         waveActive = true;
//         enemiesSpawned = 0;
//         enemySpawnTimer = 0;
//         std::cout << "=== VAGUE " << wave << " COMMENCE ===" << std::endl;
//         std::cout << "Ennemis à venir: " << enemiesInWave << std::endl;
//     }

//     void update(float deltaTime) {
//         // Spawn des ennemis si vague active
//         if (waveActive && enemiesSpawned < enemiesInWave) {
//             enemySpawnTimer += deltaTime;
//             if (enemySpawnTimer >= 1.0f) { // Un ennemi par seconde
//                 // Créer différents types d'ennemis selon la vague
//                 if (wave <= 3) {
//                     enemies.push_back(Enemy(50, 60, 10)); // Gobelin
//                 } else if (wave <= 6) {
//                     if (enemiesSpawned % 2 == 0) {
//                         enemies.push_back(Enemy(120, 45, 20)); // Orc
//                     } else {
//                         enemies.push_back(Enemy(50, 60, 10)); // Gobelin
//                     }
//                 } else {
//                     if (enemiesSpawned % 3 == 0) {
//                         enemies.push_back(Enemy(300, 35, 50)); // Troll
//                     } else {
//                         enemies.push_back(Enemy(120, 45, 20)); // Orc
//                     }
//                 }
//                 enemiesSpawned++;
//                 enemySpawnTimer = 0;
//                 std::cout << "Ennemi spawné (" << enemiesSpawned << "/" << enemiesInWave << ")" << std::endl;
//             }
//         }

//         // Mettre à jour les ennemis
//         for (auto& enemy : enemies) {
//             if (enemy.alive) {
//                 enemy.update(deltaTime, path);
                
//                 // Vérifier si l'ennemi a atteint la fin
//                 if (enemy.reachedEnd(path)) {
//                     lives--;
//                     enemy.alive = false;
//                     std::cout << "Un ennemi a passé! Vies restantes: " << lives << std::endl;
//                 }
//             }
//         }

//         // Mettre à jour les tours
//         for (auto& tower : towers) {
//             tower.update(deltaTime, enemies);
//         }

//         // Supprimer les ennemis morts et donner de l'or
//         for (auto it = enemies.begin(); it != enemies.end();) {
//             if (!it->alive) {
//                 if (it->health <= 0) { // Tué par une tour
//                     gold += it->reward;
//                     std::cout << "Ennemi éliminé! +" << it->reward << " or (Total: " << gold << ")" << std::endl;
//                 }
//                 it = enemies.erase(it);
//             } else {
//                 ++it;
//             }
//         }

//         // Vérifier fin de vague
//         if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
//             waveActive = false;
//             wave++;
//             enemiesInWave += 2; // Plus d'ennemis à chaque vague
//             std::cout << "=== VAGUE " << (wave-1) << " TERMINÉE ===" << std::endl;
//             std::cout << "Appuyez sur ESPACE pour la vague " << wave << std::endl;
            
//             if (wave > 10) {
//                 std::cout << "=== VICTOIRE! TOUTES LES VAGUES COMPLÉTÉES ===" << std::endl;
//             }
//         }

//         // Vérifier game over
//         if (lives <= 0) {
//             std::cout << "=== GAME OVER ===" << std::endl;
//         }
//     }

//     std::string intToString(int value) {
//         std::ostringstream oss;
//         oss << value;
//         return oss.str();
//     }

//     void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::Black, int size = 16) {
//         sf::Text sfText;
//         if (fontLoaded) {
//             sfText.setFont(font);
//         }
//         sfText.setString(text);
//         sfText.setCharacterSize(size);
//         sfText.setFillColor(color);
//         sfText.setPosition(x, y);
//         window.draw(sfText);
//     }

//     void render() {
//         window.clear(sf::Color(40, 40, 40)); // Gris foncé

//         // Dessiner le chemin élargi
//         for (size_t i = 0; i < path.size() - 1; i++) {
//             sf::Vector2f start = path[i];
//             sf::Vector2f end = path[i + 1];
//             sf::Vector2f direction = end - start;
//             float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
//             if (length > 0) {
//                 direction = direction / length;
//                 sf::Vector2f perpendicular(-direction.y, direction.x);
                
//                 // Créer un rectangle pour représenter le chemin large
//                 sf::RectangleShape pathSegment;
//                 pathSegment.setSize(sf::Vector2f(length, 40)); // Largeur de 40 pixels
//                 pathSegment.setPosition(start);
//                 pathSegment.setFillColor(sf::Color(100, 100, 100)); // Gris
                
//                 // Calculer l'angle de rotation
//                 float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
//                 pathSegment.setRotation(angle);
//                 pathSegment.setOrigin(0, 20); // Centrer verticalement
                
//                 window.draw(pathSegment);
//             }
//         }

//         // Points du chemin (plus petits)
//         for (const auto& point : path) {
//             sf::CircleShape dot(3);
//             dot.setPosition(point.x - 3, point.y - 3);
//             dot.setFillColor(sf::Color::Yellow);
//             window.draw(dot);
//         }

//         // Dessiner les tours
//         for (auto& tower : towers) {
//             tower.draw(window);
//         }

//         // Dessiner les ennemis
//         for (auto& enemy : enemies) {
//             if (enemy.alive) {
//                 enemy.draw(window);
//             }
//         }

//         // Interface améliorée avec compteurs visibles
//         // Boîte pour l'or
//         sf::RectangleShape goldBox(sf::Vector2f(180, 35));
//         goldBox.setPosition(10, 10);
//         goldBox.setFillColor(sf::Color(255, 215, 0, 200)); // Or avec transparence
//         goldBox.setOutlineColor(sf::Color::Black);
//         goldBox.setOutlineThickness(2);
//         window.draw(goldBox);
        
//         // Texte de l'or
//         drawText(window, "Or: " + intToString(gold), 15, 18, sf::Color::Black, 18);

//         // Boîte pour les vies
//         sf::RectangleShape livesBox(sf::Vector2f(180, 35));
//         livesBox.setPosition(10, 55);
//         livesBox.setFillColor(sf::Color(255, 100, 100, 200)); // Rouge avec transparence
//         livesBox.setOutlineColor(sf::Color::Black);
//         livesBox.setOutlineThickness(2);
//         window.draw(livesBox);
        
//         // Texte des vies
//         drawText(window, "Vies: " + intToString(lives), 15, 63, sf::Color::Black, 18);

//         // Boîte pour la vague
//         sf::RectangleShape waveBox(sf::Vector2f(180, 35));
//         waveBox.setPosition(10, 100);
//         waveBox.setFillColor(sf::Color(100, 150, 255, 200)); // Bleu avec transparence
//         waveBox.setOutlineColor(sf::Color::Black);
//         waveBox.setOutlineThickness(2);
//         window.draw(waveBox);
        
//         // Texte de la vague
//         drawText(window, "Vague: " + intToString(wave), 15, 108, sf::Color::Black, 18);

//         // Bouton pour démarrer la vague
//         if (!waveActive && wave <= 10) {
//             sf::RectangleShape startBox(sf::Vector2f(250, 50));
//             startBox.setPosition(WINDOW_WIDTH/2 - 125, 50);
//             startBox.setFillColor(sf::Color(100, 255, 100, 220));
//             startBox.setOutlineColor(sf::Color::Black);
//             startBox.setOutlineThickness(3);
//             window.draw(startBox);
            
//             drawText(window, "ESPACE - Demarrer Vague", WINDOW_WIDTH/2 - 115, 65, sf::Color::Black, 16);
//         }

//         // Message de victoire
//         if (wave > 10) {
//             sf::RectangleShape victoryBox(sf::Vector2f(400, 80));
//             victoryBox.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 40);
//             victoryBox.setFillColor(sf::Color(100, 255, 100, 240));
//             victoryBox.setOutlineColor(sf::Color::Black);
//             victoryBox.setOutlineThickness(4);
//             window.draw(victoryBox);
            
//             drawText(window, "VICTOIRE!", WINDOW_WIDTH/2 - 60, WINDOW_HEIGHT/2 - 20, sf::Color::Black, 24);
//         }

//         // Message de game over
//         if (lives <= 0) {
//             sf::RectangleShape gameOverBox(sf::Vector2f(400, 80));
//             gameOverBox.setPosition(WINDOW_WIDTH/2 - 200, WINDOW_HEIGHT/2 - 40);
//             gameOverBox.setFillColor(sf::Color(255, 100, 100, 240));
//             gameOverBox.setOutlineColor(sf::Color::Black);
//             gameOverBox.setOutlineThickness(4);
//             window.draw(gameOverBox);
            
//             drawText(window, "GAME OVER!", WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 - 20, sf::Color::Black, 24);
//         }

//         // Instructions en bas d'écran
//         drawText(window, "Clic: Placer tour (50 or) | Clic sur tour: Ameliorer", 10, WINDOW_HEIGHT - 25, sf::Color::White, 14);

//         window.display();
//     }
// };

// int main() {
//     std::cout << "Démarrage du Tower Defense..." << std::endl;
//     Game game;
//     game.run();
//     std::cout << "Jeu terminé." << std::endl;
//     return 0;
// }

// Ci dessus version fonctionnelle un peu plus élaborée -----------------------------

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>

// Constantes
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const float TOWER_MIN_DISTANCE = 70.0f;

// Structure pour les projectiles
struct Projectile {
    sf::Vector2f position;
    sf::Vector2f target;
    sf::Vector2f velocity;
    float speed;
    int damage;
    bool active;
    sf::Color color;
    
    Projectile(sf::Vector2f start, sf::Vector2f end, int dmg, sf::Color col) 
        : position(start), target(end), damage(dmg), active(true), color(col), speed(300.0f) {
        sf::Vector2f direction = target - position;
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            velocity = (direction / length) * speed;
        }
    }
    
    void update(float deltaTime) {
        position += velocity * deltaTime;
        float distance = sqrt(pow(target.x - position.x, 2) + pow(target.y - position.y, 2));
        if (distance < 10.0f) {
            active = false;
        }
    }
    
    void draw(sf::RenderWindow& window) {
        sf::CircleShape bullet(4);
        bullet.setPosition(position.x - 4, position.y - 4);
        bullet.setFillColor(color);
        bullet.setOutlineColor(sf::Color::White);
        bullet.setOutlineThickness(1);
        window.draw(bullet);
        
        // Traînée
        sf::CircleShape trail(2);
        trail.setPosition(position.x - velocity.x * 0.05f - 2, position.y - velocity.y * 0.05f - 2);
        sf::Color trailColor = color;
        trailColor.a = 100;
        trail.setFillColor(trailColor);
        window.draw(trail);
    }
};

// Structure pour les particules d'explosion
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float maxLifetime;
    float size;
    
    Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col, float life, float sz) 
        : position(pos), velocity(vel), color(col), lifetime(life), maxLifetime(life), size(sz) {}
    
    void update(float deltaTime) {
        position += velocity * deltaTime;
        lifetime -= deltaTime;
        velocity *= 0.98f; // Friction
        
        // Fade out
        float alpha = (lifetime / maxLifetime) * 255;
        color.a = (sf::Uint8)std::max(0.0f, alpha);
    }
    
    void draw(sf::RenderWindow& window) {
        if (lifetime > 0) {
            sf::CircleShape particle(size * (lifetime / maxLifetime));
            particle.setPosition(position.x - particle.getRadius(), position.y - particle.getRadius());
            particle.setFillColor(color);
            window.draw(particle);
        }
    }
    
    bool isAlive() const { return lifetime > 0; }
};

class Enemy {
public:
    sf::Vector2f position;
    float speed;
    int health;
    int maxHealth;
    int reward;
    bool alive;
    int pathIndex;
    sf::Color baseColor;
    float animationTime;

    Enemy(int hp, float spd, int rew, sf::Color color) 
        : health(hp), maxHealth(hp), speed(spd), reward(rew), alive(true), 
          pathIndex(0), baseColor(color), animationTime(0) {
        position = sf::Vector2f(-30, 400);
    }

    void update(float deltaTime, const std::vector<sf::Vector2f>& path) {
        if (!alive || pathIndex >= path.size() - 1) return;
        
        animationTime += deltaTime * 3;

        sf::Vector2f target = path[pathIndex + 1];
        sf::Vector2f direction = target - position;
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 15.0f) {
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

    void draw(sf::RenderWindow& window) {
        // Ombre portée
        sf::CircleShape shadow(18);
        shadow.setPosition(position.x - 16, position.y - 12);
        shadow.setFillColor(sf::Color(0, 0, 0, 60));
        window.draw(shadow);

        // Corps principal avec animation
        float radius = 16 + sin(animationTime) * 2;
        sf::CircleShape shape(radius);
        shape.setPosition(position.x - radius, position.y - radius);
        
        // Couleur dynamique selon la santé
        float healthRatio = (float)health / maxHealth;
        sf::Color currentColor = baseColor;
        if (healthRatio < 0.5f) {
            // Mélange avec le rouge quand blessé
            currentColor.r = std::min(255, (int)(currentColor.r + (255 - currentColor.r) * (1 - healthRatio * 2)));
        }
        
        shape.setFillColor(currentColor);
        
        // Contour brillant
        shape.setOutlineColor(sf::Color(255, 255, 255, 150));
        shape.setOutlineThickness(2);
        window.draw(shape);

        // Barre de vie stylisée
        if (health < maxHealth) {
            // Fond de la barre
            sf::RectangleShape healthBarBg(sf::Vector2f(36, 8));
            healthBarBg.setPosition(position.x - 18, position.y - radius - 15);
            healthBarBg.setFillColor(sf::Color(50, 50, 50, 200));
            healthBarBg.setOutlineColor(sf::Color::White);
            healthBarBg.setOutlineThickness(1);
            window.draw(healthBarBg);

            // Remplissage de la barre
            if (health > 0) {
                sf::RectangleShape healthFill(sf::Vector2f(34 * healthRatio, 6));
                healthFill.setPosition(position.x - 17, position.y - radius - 14);
                
                // Couleur de la barre selon la santé
                sf::Color healthColor;
                if (healthRatio > 0.7f) healthColor = sf::Color::Green;
                else if (healthRatio > 0.4f) healthColor = sf::Color::Yellow;
                else healthColor = sf::Color::Red;
                
                healthFill.setFillColor(healthColor);
                window.draw(healthFill);
            }
        }

        // Point lumineux sur l'ennemi
        sf::CircleShape glow(6);
        glow.setPosition(position.x - 6, position.y - 6);
        sf::Color glowColor = sf::Color::White;
        glowColor.a = (sf::Uint8)(100 + 50 * sin(animationTime * 2));
        glow.setFillColor(glowColor);
        window.draw(glow);
    }

    bool reachedEnd(const std::vector<sf::Vector2f>& path) {
        return pathIndex >= path.size() - 1;
    }
};

class Tower {
public:
    sf::Vector2f position;
    int damage;
    float range;
    float attackCooldown;
    float timeSinceLastAttack;
    int level;
    bool isSelected;
    float rotationAngle;
    sf::Vector2f lastTarget;

    Tower(sf::Vector2f pos) : position(pos), damage(25), range(90), attackCooldown(1.0f), 
                              timeSinceLastAttack(0), level(1), isSelected(false), 
                              rotationAngle(0), lastTarget(pos) {}

    Enemy* findTarget(std::vector<Enemy>& enemies) {
        Enemy* target = nullptr;
        float closestDistance = range + 1;
        
        for (auto& enemy : enemies) {
            if (!enemy.alive) continue;
            
            float distance = sqrt(pow(enemy.position.x - position.x, 2) + pow(enemy.position.y - position.y, 2));
            if (distance <= range && distance < closestDistance) {
                target = &enemy;
                closestDistance = distance;
            }
        }
        
        return target;
    }

    bool update(float deltaTime, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles) {
        timeSinceLastAttack += deltaTime;
        
        Enemy* target = findTarget(enemies);
        if (target) {
            lastTarget = target->position;
            
            // Rotation vers la cible
            sf::Vector2f direction = target->position - position;
            rotationAngle = atan2(direction.y, direction.x) * 180 / 3.14159f;
            
            if (timeSinceLastAttack >= attackCooldown) {
                // Créer projectile
                sf::Color bulletColor = (level == 1) ? sf::Color::Cyan : 
                                       (level == 2) ? sf::Color::Magenta : sf::Color::Yellow;
                projectiles.push_back(Projectile(position, target->position, damage, bulletColor));
                timeSinceLastAttack = 0;
                return true;
            }
        }
        
        return false;
    }

    void upgrade() {
        if (level == 1) {
            level = 2;
            damage = 50;
            range = 110;
            attackCooldown = 0.8f;
        } else if (level == 2) {
            level = 3;
            damage = 90;
            range = 130;
            attackCooldown = 0.6f;
        }
    }

    void draw(sf::RenderWindow& window) {
        // Cercle de portée si sélectionné
        if (isSelected) {
            sf::CircleShape rangeCircle(range);
            rangeCircle.setPosition(position.x - range, position.y - range);
            rangeCircle.setFillColor(sf::Color(100, 150, 255, 30));
            rangeCircle.setOutlineColor(sf::Color(100, 150, 255, 100));
            rangeCircle.setOutlineThickness(2);
            window.draw(rangeCircle);
        }

        // Base de la tour avec dégradé
        sf::CircleShape base(22);
        base.setPosition(position.x - 22, position.y - 22);
        base.setFillColor(sf::Color(60, 60, 60));
        base.setOutlineColor(sf::Color(40, 40, 40));
        base.setOutlineThickness(3);
        window.draw(base);

        // Tour principale
        sf::RectangleShape towerBody(sf::Vector2f(28, 28));
        towerBody.setPosition(position.x - 14, position.y - 14);
        
        sf::Color towerColor;
        if (level == 1) towerColor = sf::Color(80, 120, 255);       // Bleu
        else if (level == 2) towerColor = sf::Color(255, 80, 255);  // Magenta
        else towerColor = sf::Color(255, 255, 80);                  // Jaune/Or
        
        towerBody.setFillColor(towerColor);
        towerBody.setOutlineColor(sf::Color::White);
        towerBody.setOutlineThickness(2);
        window.draw(towerBody);

        // Canon rotatif
        sf::RectangleShape cannon(sf::Vector2f(30, 6));
        cannon.setPosition(position.x, position.y - 3);
        cannon.setFillColor(sf::Color(80, 80, 80));
        cannon.setOutlineColor(sf::Color::White);
        cannon.setOutlineThickness(1);
        cannon.setOrigin(0, 3);
        cannon.setRotation(rotationAngle);
        window.draw(cannon);

        // Indicateurs de niveau (gemmes)
        for (int i = 0; i < level; i++) {
            sf::CircleShape gem(4);
            gem.setPosition(position.x - 8 + i * 8, position.y - 28);
            gem.setFillColor(sf::Color::Yellow);
            gem.setOutlineColor(sf::Color(255, 215, 0));
            gem.setOutlineThickness(1);
            window.draw(gem);
        }

        // Effet de sélection animé
        if (isSelected) {
            static float selectionTime = 0;
            selectionTime += 0.1f;
            
            sf::CircleShape selectionRing(26 + sin(selectionTime) * 3);
            selectionRing.setPosition(position.x - selectionRing.getRadius(), position.y - selectionRing.getRadius());
            selectionRing.setFillColor(sf::Color::Transparent);
            selectionRing.setOutlineColor(sf::Color(255, 255, 0, 200));
            selectionRing.setOutlineThickness(3);
            window.draw(selectionRing);
        }
    }

    int getUpgradeCost() const {
        if (level == 1) return 100;
        if (level == 2) return 200;
        return 0;
    }

    bool canUpgrade() const {
        return level < 3;
    }
};

class Game {
private:
    sf::RenderWindow window;
    std::vector<sf::Vector2f> path;
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    std::vector<Projectile> projectiles;
    std::vector<Particle> particles;
    int gold;
    int lives;
    int wave;
    float enemySpawnTimer;
    int enemiesInWave;
    int enemiesSpawned;
    bool waveActive;
    sf::Font font;
    bool fontLoaded;
    Tower* selectedTower;

public:
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense Deluxe"),
             gold(250), lives(20), wave(1), enemySpawnTimer(0), 
             enemiesInWave(6), enemiesSpawned(0), waveActive(false), 
             fontLoaded(false), selectedTower(nullptr) {
        
        std::cout << "=== TOWER DEFENSE DELUXE DÉMARRE ===" << std::endl;
        std::cout << "Le texte utilisera la police par défaut de SFML" << std::endl;
        
        // Créer un chemin sinueux et esthétique
        path.push_back(sf::Vector2f(-30, 400));
        path.push_back(sf::Vector2f(100, 400));
        path.push_back(sf::Vector2f(200, 350));
        path.push_back(sf::Vector2f(300, 300));
        path.push_back(sf::Vector2f(350, 200));
        path.push_back(sf::Vector2f(450, 150));
        path.push_back(sf::Vector2f(600, 200));
        path.push_back(sf::Vector2f(700, 300));
        path.push_back(sf::Vector2f(800, 400));
        path.push_back(sf::Vector2f(900, 450));
        path.push_back(sf::Vector2f(1000, 400));
        path.push_back(sf::Vector2f(1230, 400));
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
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && !waveActive) {
                    startWave();
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    if (selectedTower) selectedTower->isSelected = false;
                    selectedTower = nullptr;
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f clickPos(mousePos.x, mousePos.y);
                
                // Désélectionner tours précédentes
                if (selectedTower) selectedTower->isSelected = false;
                selectedTower = nullptr;
                
                // Vérifier clic sur tour existante
                bool towerClicked = false;
                for (auto& tower : towers) {
                    float distance = sqrt(pow(clickPos.x - tower.position.x, 2) + pow(clickPos.y - tower.position.y, 2));
                    if (distance <= 25) {
                        tower.isSelected = true;
                        selectedTower = &tower;
                        towerClicked = true;
                        
                        if (event.mouseButton.button == sf::Mouse::Right && tower.canUpgrade() && gold >= tower.getUpgradeCost()) {
                            gold -= tower.getUpgradeCost();
                            tower.upgrade();
                            // Effet de particules d'amélioration
                            createUpgradeEffect(tower.position);
                        }
                        break;
                    }
                }
                
                // Placer nouvelle tour
                if (!towerClicked && gold >= 50 && event.mouseButton.button == sf::Mouse::Left) {
                    if (canPlaceTower(clickPos)) {
                        towers.push_back(Tower(clickPos));
                        gold -= 50;
                        createPlacementEffect(clickPos);
                    }
                }
            }
        }
    }

    bool canPlaceTower(sf::Vector2f pos) {
        // Vérifier distance avec autres tours
        for (const auto& tower : towers) {
            float distance = sqrt(pow(pos.x - tower.position.x, 2) + pow(pos.y - tower.position.y, 2));
            if (distance < TOWER_MIN_DISTANCE) return false;
        }
        
        // Vérifier distance avec chemin
        for (size_t i = 0; i < path.size() - 1; i++) {
            sf::Vector2f start = path[i];
            sf::Vector2f end = path[i + 1];
            
            // Distance point-segment
            sf::Vector2f AP = pos - start;
            sf::Vector2f AB = end - start;
            float ab2 = AB.x * AB.x + AB.y * AB.y;
            float ap_ab = AP.x * AB.x + AP.y * AB.y;
            float t = ap_ab / ab2;
            
            if (t < 0.0f) t = 0.0f;
            else if (t > 1.0f) t = 1.0f;
            
            sf::Vector2f closest = start + t * AB;
            float distance = sqrt(pow(pos.x - closest.x, 2) + pow(pos.y - closest.y, 2));
            
            if (distance < 50) return false;
        }
        
        return true;
    }

    void createUpgradeEffect(sf::Vector2f pos) {
        for (int i = 0; i < 15; i++) {
            float angle = (float)i / 15.0f * 2 * 3.14159f;
            sf::Vector2f velocity(cos(angle) * 80, sin(angle) * 80);
            particles.push_back(Particle(pos, velocity, sf::Color::Yellow, 1.0f, 3.0f));
        }
    }

    void createPlacementEffect(sf::Vector2f pos) {
        for (int i = 0; i < 10; i++) {
            float angle = (float)i / 10.0f * 2 * 3.14159f;
            sf::Vector2f velocity(cos(angle) * 60, sin(angle) * 60);
            particles.push_back(Particle(pos, velocity, sf::Color::Green, 0.8f, 2.0f));
        }
    }

    void createHitEffect(sf::Vector2f pos) {
        for (int i = 0; i < 8; i++) {
            float angle = (float)rand() / RAND_MAX * 2 * 3.14159f;
            sf::Vector2f velocity(cos(angle) * 40, sin(angle) * 40);
            particles.push_back(Particle(pos, velocity, sf::Color::Red, 0.5f, 2.0f));
        }
    }

    void startWave() {
        waveActive = true;
        enemiesSpawned = 0;
        enemySpawnTimer = 0;
    }

    void update(float deltaTime) {
        // Spawn des ennemis avec variété
        if (waveActive && enemiesSpawned < enemiesInWave) {
            enemySpawnTimer += deltaTime;
            if (enemySpawnTimer >= 0.8f) {
                // Différents types d'ennemis selon la vague
                if (wave <= 3) {
                    enemies.push_back(Enemy(60, 70, 15, sf::Color::Green));
                } else if (wave <= 6) {
                    if (enemiesSpawned % 2 == 0) {
                        enemies.push_back(Enemy(120, 50, 25, sf::Color::Blue));
                    } else {
                        enemies.push_back(Enemy(60, 70, 15, sf::Color::Green));
                    }
                } else {
                    if (enemiesSpawned % 3 == 0) {
                        enemies.push_back(Enemy(250, 40, 50, sf::Color::Red));
                    } else {
                        enemies.push_back(Enemy(120, 50, 25, sf::Color::Blue));
                    }
                }
                enemiesSpawned++;
                enemySpawnTimer = 0;
            }
        }

        // Mettre à jour les ennemis
        for (auto& enemy : enemies) {
            if (enemy.alive) {
                enemy.update(deltaTime, path);
                
                if (enemy.reachedEnd(path)) {
                    lives--;
                    enemy.alive = false;
                }
            }
        }

        // Mettre à jour les tours
        for (auto& tower : towers) {
            tower.update(deltaTime, enemies, projectiles);
        }

        // Mettre à jour les projectiles
        for (auto& projectile : projectiles) {
            if (projectile.active) {
                projectile.update(deltaTime);
                
                // Vérifier collision avec ennemis
                for (auto& enemy : enemies) {
                    if (!enemy.alive) continue;
                    
                    float distance = sqrt(pow(enemy.position.x - projectile.position.x, 2) + 
                                        pow(enemy.position.y - projectile.position.y, 2));
                    if (distance < 20.0f) {
                        enemy.takeDamage(projectile.damage);
                        createHitEffect(enemy.position);
                        projectile.active = false;
                        break;
                    }
                }
            }
        }

        // Nettoyer les projectiles inactifs
        projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.active; }), projectiles.end());

        // Mettre à jour les particules
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }
        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.isAlive(); }), particles.end());

        // Supprimer les ennemis morts
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!it->alive) {
                if (it->health <= 0) {
                    gold += it->reward;
                    createHitEffect(it->position);
                }
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        // Vérifier fin de vague
        if (waveActive && enemiesSpawned >= enemiesInWave && enemies.empty()) {
            waveActive = false;
            wave++;
            enemiesInWave += 2;
        }
    }

    std::string intToString(int value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, sf::Color color = sf::Color::White, int size = 18) {
        sf::Text sfText;
        // Utiliser la police par défaut de SFML (pas besoin de charger un fichier)
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(color);
        sfText.setPosition(x, y);
        sfText.setOutlineColor(sf::Color::Black);
        sfText.setOutlineThickness(1);
        window.draw(sfText);
    }

    void render() {
        // Dégradé de fond
        sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        background.setFillColor(sf::Color(15, 25, 45));
        window.draw(background);

        // Grille décorative
        for (int x = 0; x < WINDOW_WIDTH; x += 100) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, 0), sf::Color(255, 255, 255, 8)),
                sf::Vertex(sf::Vector2f(x, WINDOW_HEIGHT), sf::Color(255, 255, 255, 8))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Dessiner le chemin avec style
        for (size_t i = 0; i < path.size() - 1; i++) {
            sf::Vector2f start = path[i];
            sf::Vector2f end = path[i + 1];
            sf::Vector2f direction = end - start;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (length > 0) {
                direction = direction / length;
                
                // Chemin principal large
                sf::RectangleShape pathSegment;
                pathSegment.setSize(sf::Vector2f(length, 40));
                pathSegment.setPosition(start);
                pathSegment.setFillColor(sf::Color(101, 67, 33));
                
                float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
                pathSegment.setRotation(angle);
                pathSegment.setOrigin(0, 20);
                window.draw(pathSegment);
                
                // Bordures du chemin
                sf::RectangleShape border;
                border.setSize(sf::Vector2f(length, 4));
                border.setPosition(start);
                border.setFillColor(sf::Color(139, 90, 43));
                border.setRotation(angle);
                border.setOrigin(0, 2);
                window.draw(border);
                
                border.setOrigin(0, -38);
                window.draw(border);
            }
        }

        // Points de chemin brillants
        for (const auto& point : path) {
            sf::CircleShape waypoint(6);
            waypoint.setPosition(point.x - 6, point.y - 6);
            waypoint.setFillColor(sf::Color(255, 215, 0));
            waypoint.setOutlineColor(sf::Color(255, 255, 255));
            waypoint.setOutlineThickness(2);
            window.draw(waypoint);
        }

        // Dessiner tours, ennemis, projectiles et particules
        for (auto& tower : towers) {
            tower.draw(window);
        }

        for (auto& enemy : enemies) {
            if (enemy.alive) {
                enemy.draw(window);
            }
        }

        for (auto& projectile : projectiles) {
            if (projectile.active) {
                projectile.draw(window);
            }
        }

        for (auto& particle : particles) {
            particle.draw(window);
        }

        // Interface élégante
        renderUI(window);

        window.display();
    }

    void renderUI(sf::RenderWindow& window) {
        // Panneau principal semi-transparent
        sf::RectangleShape uiPanel(sf::Vector2f(350, 120));
        uiPanel.setPosition(10, 10);
        uiPanel.setFillColor(sf::Color(0, 0, 0, 150));
        uiPanel.setOutlineColor(sf::Color(100, 150, 255));
        uiPanel.setOutlineThickness(2);
        window.draw(uiPanel);

        // Or avec représentation visuelle
        sf::CircleShape goldIcon(18);
        goldIcon.setPosition(20, 20);
        goldIcon.setFillColor(sf::Color(255, 215, 0));
        goldIcon.setOutlineColor(sf::Color(255, 255, 255));
        goldIcon.setOutlineThickness(2);
        window.draw(goldIcon);
        
        // Barres d'or (chaque barre = 50 or)
        int goldBars = std::min(gold / 50, 12);
        for (int i = 0; i < goldBars; i++) {
            sf::RectangleShape goldBar(sf::Vector2f(8, 16));
            goldBar.setPosition(50 + i * 10, 22);
            goldBar.setFillColor(sf::Color::Yellow);
            goldBar.setOutlineColor(sf::Color::Black);
            goldBar.setOutlineThickness(1);
            window.draw(goldBar);
        }
        
        // Texte de l'or (devrait s'afficher maintenant)
        drawText(window, "Or: " + intToString(gold), 200, 25, sf::Color::Yellow, 16);

        // Vies avec représentation visuelle
        sf::CircleShape lifeIcon(18);
        lifeIcon.setPosition(20, 55);
        lifeIcon.setFillColor(sf::Color::Red);
        lifeIcon.setOutlineColor(sf::Color::White);
        lifeIcon.setOutlineThickness(2);
        window.draw(lifeIcon);
        
        // Cœurs pour les vies
        for (int i = 0; i < std::min(lives, 15); i++) {
            sf::CircleShape heart(5);
            heart.setPosition(50 + i * 12, 60);
            heart.setFillColor(sf::Color::Red);
            window.draw(heart);
        }
        
        // Texte des vies
        drawText(window, "Vies: " + intToString(lives), 200, 60, sf::Color::Red, 16);

        // Vague avec représentation visuelle
        sf::RectangleShape waveIcon(sf::Vector2f(30, 30));
        waveIcon.setPosition(20, 90);
        waveIcon.setFillColor(sf::Color::Blue);
        waveIcon.setOutlineColor(sf::Color::White);
        waveIcon.setOutlineThickness(2);
        window.draw(waveIcon);
        
        // Étoiles pour les vagues
        for (int i = 0; i < std::min(wave, 10); i++) {
            sf::CircleShape star(3);
            star.setPosition(55 + (i % 5) * 10, 95 + (i / 5) * 8);
            star.setFillColor(sf::Color::Cyan);
            window.draw(star);
        }
        
        // Texte de la vague
        drawText(window, "Vague: " + intToString(wave), 200, 95, sf::Color::Cyan, 16);

        // Statistiques de tour sélectionnée
        if (selectedTower) {
            sf::RectangleShape towerInfo(sf::Vector2f(280, 140));
            towerInfo.setPosition(WINDOW_WIDTH - 290, 10);
            towerInfo.setFillColor(sf::Color(0, 0, 0, 180));
            towerInfo.setOutlineColor(sf::Color::Yellow);
            towerInfo.setOutlineThickness(2);
            window.draw(towerInfo);
            
            drawText(window, "TOUR SELECTIONNEE", WINDOW_WIDTH - 280, 20, sf::Color::Yellow, 16);
            drawText(window, "Niveau: " + intToString(selectedTower->level) + "/3", WINDOW_WIDTH - 280, 45, sf::Color::White, 14);
            drawText(window, "Degats: " + intToString(selectedTower->damage) + " HP", WINDOW_WIDTH - 280, 65, sf::Color::Red, 14);
            drawText(window, "Portee: " + intToString((int)selectedTower->range) + " px", WINDOW_WIDTH - 280, 85, sf::Color::Blue, 14);
            
            // Barres visuelles pour les stats
            // Dégâts (barres rouges)
            int damageBars = selectedTower->damage / 10;
            for (int i = 0; i < std::min(damageBars, 10); i++) {
                sf::RectangleShape dmgBar(sf::Vector2f(6, 10));
                dmgBar.setPosition(WINDOW_WIDTH - 100 + i * 8, 67);
                dmgBar.setFillColor(sf::Color::Red);
                window.draw(dmgBar);
            }
            
            // Portée (barres bleues)
            int rangeBars = (int)selectedTower->range / 15;
            for (int i = 0; i < std::min(rangeBars, 10); i++) {
                sf::RectangleShape rngBar(sf::Vector2f(6, 10));
                rngBar.setPosition(WINDOW_WIDTH - 100 + i * 8, 87);
                rngBar.setFillColor(sf::Color::Blue);
                window.draw(rngBar);
            }
            
            if (selectedTower->canUpgrade()) {
                int cost = selectedTower->getUpgradeCost();
                sf::Color upgradeColor = (gold >= cost) ? sf::Color::Green : sf::Color::Red;
                drawText(window, "Clic droit: Ameliorer", WINDOW_WIDTH - 280, 105, upgradeColor, 13);
                drawText(window, "Cout: " + intToString(cost) + " or", WINDOW_WIDTH - 280, 125, upgradeColor, 13);
            } else {
                drawText(window, "NIVEAU MAXIMUM", WINDOW_WIDTH - 280, 105, sf::Color::Cyan, 13);
            }
        }

        // Bouton de démarrage de vague avec animation
        if (!waveActive) {
            static float buttonPulse = 0;
            buttonPulse += 0.1f;
            
            sf::RectangleShape startButton(sf::Vector2f(220 + sin(buttonPulse) * 10, 50));
            startButton.setPosition(WINDOW_WIDTH/2 - 110 - sin(buttonPulse) * 5, 150);
            startButton.setFillColor(sf::Color(0, 200, 0, 200 + sin(buttonPulse * 2) * 55));
            startButton.setOutlineColor(sf::Color::White);
            startButton.setOutlineThickness(3);
            window.draw(startButton);
            
            // Icône ESPACE stylisée
            sf::RectangleShape spaceIcon(sf::Vector2f(60, 20));
            spaceIcon.setPosition(WINDOW_WIDTH/2 - 30, 160);
            spaceIcon.setFillColor(sf::Color::White);
            spaceIcon.setOutlineColor(sf::Color::Black);
            spaceIcon.setOutlineThickness(1);
            window.draw(spaceIcon);
            
            drawText(window, "ESPACE - Demarrer Vague " + intToString(wave), WINDOW_WIDTH/2 - 100, 175, sf::Color::White, 14);
        }

        // Instructions stylisées
        sf::RectangleShape instructionPanel(sf::Vector2f(450, 60));
        instructionPanel.setPosition(10, WINDOW_HEIGHT - 70);
        instructionPanel.setFillColor(sf::Color(0, 0, 0, 120));
        instructionPanel.setOutlineColor(sf::Color(100, 100, 100));
        instructionPanel.setOutlineThickness(1);
        window.draw(instructionPanel);
        
        drawText(window, "Clic gauche: Placer tour (50 or)", 20, WINDOW_HEIGHT - 60, sf::Color::White, 14);
        drawText(window, "Clic droit: Ameliorer | ESC: Deselectionner", 20, WINDOW_HEIGHT - 40, sf::Color::White, 14);

        // Informations de progression pendant la vague
        if (waveActive) {
            sf::RectangleShape progressPanel(sf::Vector2f(250, 60));
            progressPanel.setPosition(WINDOW_WIDTH/2 - 125, 220);
            progressPanel.setFillColor(sf::Color(0, 0, 0, 150));
            progressPanel.setOutlineColor(sf::Color(255, 165, 0));
            progressPanel.setOutlineThickness(2);
            window.draw(progressPanel);
            
            drawText(window, "Ennemis spawnes: " + intToString(enemiesSpawned) + "/" + intToString(enemiesInWave), 
                    WINDOW_WIDTH/2 - 115, 235, sf::Color(255, 165, 0), 14);
            drawText(window, "Ennemis restants: " + intToString(enemies.size()), 
                    WINDOW_WIDTH/2 - 80, 255, sf::Color::Red, 14);
            
            // Barre de progression visuelle
            sf::RectangleShape progressBar(sf::Vector2f(200, 8));
            progressBar.setPosition(WINDOW_WIDTH/2 - 100, 270);
            progressBar.setFillColor(sf::Color(50, 50, 50));
            progressBar.setOutlineColor(sf::Color::White);
            progressBar.setOutlineThickness(1);
            window.draw(progressBar);
            
            // Remplissage de la barre
            float progress = (float)enemiesSpawned / enemiesInWave;
            sf::RectangleShape progressFill(sf::Vector2f(198 * progress, 6));
            progressFill.setPosition(WINDOW_WIDTH/2 - 99, 271);
            progressFill.setFillColor(sf::Color(255, 165, 0));
            window.draw(progressFill);
        }

        // Effets visuels d'interface améliorés
        static float decorTime = 0;
        decorTime += 0.05f;
        
        // Particules décoratives qui bougent
        for (int i = 0; i < 5; i++) {
            sf::CircleShape decorParticle(2 + sin(decorTime + i) * 1);
            decorParticle.setPosition(30 + i * 20, 140 + sin(decorTime + i * 0.5f) * 15);
            sf::Color decorColor = sf::Color::Cyan;
            decorColor.a = (sf::Uint8)(80 + 60 * sin(decorTime + i));
            decorParticle.setFillColor(decorColor);
            window.draw(decorParticle);
        }

        // Indicateur de performance (FPS approximatif)
        static int frameCount = 0;
        static float timeAccumulator = 0;
        static float currentFPS = 60;
        
        frameCount++;
        timeAccumulator += 1.0f/60.0f; // Approximation
        
        if (frameCount % 60 == 0) {
            currentFPS = 60.0f / timeAccumulator;
            timeAccumulator = 0;
        }
        
        // Petit indicateur de performance
        sf::CircleShape performanceIndicator(8);
        performanceIndicator.setPosition(WINDOW_WIDTH - 30, 20);
        sf::Color perfColor = (currentFPS > 50) ? sf::Color::Green : 
                             (currentFPS > 30) ? sf::Color::Yellow : sf::Color::Red;
        performanceIndicator.setFillColor(perfColor);
        window.draw(performanceIndicator);
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}