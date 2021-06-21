#include "Game.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
constexpr char Game::fileName[];
Game::Game():success_{false}, numOfZombie_{DEFAULT_ZOMBIE}, numOfLand_{DEFAULT_LAND}, plantMinCost_{INT_MAX}, lastAction_{NUM_OF_PLANT_TYPES}
{
    std::ifstream inputFile(fileName, std::ios::in);
    if (!inputFile)
    {
        std::cout << "Open " << fileName << " Error\n";
    }
    else
    {   
        for (size_t i = 0; i < NUM_OF_PLANT_TYPES; ++i) {
            std::string line, plantType, plantName, plantCostStr;
            int plantHp, plantCost;
            getline(inputFile, line);
            std::istringstream iss(line);
            iss >> plantType >> plantName >> plantCostStr  >> plantHp;
            plantCost = stoi(plantCostStr.substr(1));
            plantMinCost_ = std::min(plantMinCost_, plantCost);
            printf("plantType:%s plantName:%s plantCost:%d plantHp:%d ", plantType.c_str(), plantName.c_str(), plantCost, plantHp);
            switch (plantType[0])
            {
                case 'C':
                    int visit, reward;
                    iss >> visit >> reward;
                    printf("visit:%d reward:%d\n", visit, reward);
                    plants_.push_back(new CoinPlant(plantCost, plantHp, reward, visit, plantName));
                    break;
                case 'S':
                    int dmg;
                    iss >> dmg;
                    printf("dmg:%d\n", dmg);
                    plants_.push_back(new HornPlant(plantCost, plantHp, dmg, plantName));
                    break;
                case 'B':
                    printf("\n");
                    plants_.push_back(new BombPlant(plantCost, plantHp, plantHp, plantName));
                    break;
                case 'H':
                    int health;
                    iss >> health;
                    printf("health:%d\n", health);
                    plants_.push_back(new HealPlant(plantCost, plantHp, health, plantName));
                    break;
                default:
                    std::cout << "Unknown Plant Type " << plantType << std::endl;
                    break;
            }
        }
        //waiting for plain virtual show()
        // for (int i = 0; i < NUM_OF_PLANT_TYPES; ++i) {
        //     std::cout << plants_[i] << "\n";
        // }
        GameSetUp();
        player_ = new Player(numOfLand_);
        for (size_t i = 0; i < numOfZombie_; ++i) {
            zombies_.push_back(new Zombie(numOfLand_));
        }
        map_ = new Map(numOfLand_, *player_, numOfZombie_, zombies_);
        success_ = true;
    }
    inputFile.close();
}
void Game::GameSetUp()
{
    std::cout
    << "------------------------------\n"
    << "|     Plants vs. Zombies     |\n"
    << "------------------------------\n";
    std::string userInput;
    std::istringstream iss;
    std::cout << "Number of lands on the map (1-10, default: 8)...>";
    std::getline(std::cin, userInput);
    iss.str(userInput);
    iss >> numOfLand_;
    iss.clear();
    std::cout << "Number of zombies on the map (1-10, default: 3)...>";
    std::getline(std::cin, userInput);
    iss.str(userInput);
    iss >> numOfZombie_;
    iss.clear();
    if (numOfLand_ < 1 || numOfLand_ > 10) {numOfZombie_ = DEFAULT_ZOMBIE;}
    if (numOfZombie_ < 1 || numOfZombie_ > 10) {numOfLand_ = DEFAULT_LAND;}
    std::cout
    << "=========================================================\n"
    << "Plants vs Zombies Rule:\n"
    << "How to win:\n"
    << "  (1) All zombies are dead.\n"
    << "  (2) At least one plant is live.\n"
    << "  (3) The number of dead bomb plants cannot exceed the number of zombies.\n\n"
    << "How to lose:\n"
    << "  All plants are dead.\n"
    << "=============================================================================\n";
    system("Pause");
    system("CLS");
}

bool Game::PlantActValid(Plant &plant)
{
    bool ret = false;
    if (player_->Planting(plant))
    {
        printf("You have planted %s at land %d !\n", plant.Name().c_str(), player_->Locate());
        map_->Planting(&plant, player_->Locate());
        ret = true;
    }
    else
    {
        std::cout << "Not Enough Money! Please input again!\n";
        system("Pause");
    }
    return ret;
}
void Game::Play()
{
    std::cout << *map_ << "---------------------------\nZombie information:\n";
    for (size_t i = 0; i < zombies_.size(); ++i) {
        std::cout << "[" << i << "] " << *zombies_[i] << '\n';
    }
    std::cout << "===========================\n";
    //waiting for plain virtual show()
    // for (int i = 0; i < NUM_OF_PLANT_TYPES; ++i) {
    //     std::cout << plants_[i] << "\n";
    // }
    if (EnoughCost()) {
        bool optInValid = true;
        do {
            printf("Player $%d:\t Enter Your choice (4 to give up, default : %d)...>", player_->Money(), lastAction_);
            size_t action = 0;
            std::string userInput;
            getline(std::cin, userInput);
            std::istringstream iss(userInput);
            iss >> action;
            if (action >= NUM_OF_PLANT_TYPES) {action = lastAction_;}
            printf("Action : %d\n", action);
            if (action == NUM_OF_PLANT_TYPES)
            {
                //checkGame
                std::cout << "You give up!\n";
                optInValid = false;
            }
            else
            {
                //plant
                if (PlantActValid(*plants_[action]))
                {
                    optInValid = false;
                    lastAction_ = action;
                }
            }
        }
        while (optInValid);
        std::cout << "one Round\n";
        system("Pause");
    }
    else {
        std::cout << "You don't have enough money to plant anything!";
    }
    system("CLS");
}
Game::~Game()
{
    delete player_;
    for (size_t i = 0; i < zombies_.size(); ++i) {
        delete zombies_[i];
    }
    for (size_t i = 0; i < NUM_OF_PLANT_TYPES; ++i) {
        delete plants_[i];
    }
    delete map_;
}