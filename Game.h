#ifndef GAME_H
#define GAME_H
#include "Game.h"
#include "Player.h"
#include "Zombie.h"
#include "Map.h"
#include "BombPlant.h"
#include "CoinPlant.h"
#include "HealPlant.h"
#include "HornPlant.h"
#include "climits"
#include <vector>
class Game
{
private:
    static constexpr char fileName[] = "plants.txt";
    int plantMinCost_, defaultAction_;
    void GameSetUp();
    bool success_;
    size_t lastAction_;
    bool EnoughCost() {return player_->Money() > plantMinCost_;}
    bool PlantActValid(Plant &plant);
    static constexpr size_t DEFAULT_LAND = 8, DEFAULT_ZOMBIE = 3, NUM_OF_PLANT_TYPES = 4;
public:
    Game();
    void Play();
    ~Game();
    std::vector<Plant*> plants_;
    Player *player_;
    std::vector<Zombie*> zombies_;
    Map *map_;
    size_t numOfLand_, numOfZombie_;
    bool Success() const {return success_;}
};
#endif