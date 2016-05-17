#include <iostream>
#include <string>
#include "Bot.h"

// NA stands for North America Super Region (continent)
enum NA { Alaska = 1, Northwest_Territory, Greenland, Alberta, Ontario, Quebec, 
          Western_United_States, Eastern_United_States, Central_America };
// South America
enum SA { Venezuela = 10, Peru, Brazil, Argentina };
// Europe
enum EU { Iceland = 14, Great_Britain, Scandinavia, Ukraine, Western_Europe,
          Northern_Europe, Southern_Europe };
// Africa
enum AF { North_Africa = 21, Egypt, East_Africa, Congo, South_Africa, Madagascar };
// Asia
enum AS { Ural = 27, Siberia, Yakutsk, Kamchatka, Irkutsk, Kazakhstan, China,
          Mongolia, Japan, Middle_East, India, Siam };
// Australia (Oceania)
enum OC { Indonesia = 39, New_Guinea, Western_Australia, Eastern_Australia };

// a list of regions sorted by preference. If you want to try a different 
//   opening strategy, make a new list of territories that you want the
//   bot to choose when the game starts. Include all regions from THREE
//   super-regions, sorted by order of preference. In this case, I have
//   listed every region in the Australia and Asia super-regions, which
//   guarantees my initial 3 regions will be in these super-regions, as 
//   the server picks two random regions from each super-region.
const int AUSTRALIA_OPENER[] = 
    { Indonesia, New_Guinea, Western_Australia, Eastern_Australia,
      Siam, India, Middle_East, Kazakhstan, Mongolia, Japan,
      Ural, Siberia, Irkutsk, Yakutsk, Kamchatka,
      Venezuela, Peru, Brazil, Argentina };

// constructor
Bot::Bot() { }

// deconstructor
Bot::~Bot() { }

// plays a single game of Warlight. Call this method from main to get started!
void Bot::playGame()
{
    parser.initParser(this);
    parser.parseInput();
}

// makes moves for a single turn
void Bot::makeMoves()
{

}

//indicates to the engine that it has made its move
void Bot::endTurn()
{

}

void Bot::addRegion(unsigned noRegion, unsigned noSuperRegion)
{
    while (regions.size() <= noRegion)
    {
        regions.push_back(Region());
    }
    regions[noRegion] = Region(noRegion, noSuperRegion);
    superRegions[noSuperRegion].addRegion(noRegion);
}

void Bot::addNeighbors(unsigned noRegion, unsigned neighbors)
{
    regions[noRegion].addNeighbors(neighbors);
    regions[neighbors].addNeighbors(noRegion);
}

void Bot::addSuperRegion(unsigned noSuperRegion, int reward)
{
    while (superRegions.size() <=  noSuperRegion)
    {
        superRegions.push_back(SuperRegion());
    }
    superRegions[noSuperRegion]  = SuperRegion(reward);
}

void Bot::setBotName(string name)
{
    botName = name;
}

void Bot::setOpponentBotName(string name)
{
    opponentBotName = name;
}

void Bot::setArmiesLeft(int nbArmies)
{
    armiesLeft = nbArmies;
}

// called by the parser
void Bot::addStartingRegion(unsigned noRegion)
{
     startingRegionsReceived.push_back(noRegion);
}

void Bot::startDelay(int delay)
{

}

void Bot::setPhase(string pPhase)
{
    phase=pPhase;
}

// our code goes here
void Bot::executeAction()
{
    if (phase=="")
    {
        return;
    }
    if (phase == "pickPreferredRegion")
    {
        // you must submit exactly 6 preferred regions to the server
        const int MAX = 6;
        // keep track of how many regions we have submitted to the server so far
        int numSubmittedRegions = 0; 

        // loop through our list of preferred regions, checking to see which ones
        //   the server selected
        for (int prefRegionId : AUSTRALIA_OPENER)
        {
            for (int startRegionId : startingRegionsReceived)
            {
                if (numSubmittedRegions <= MAX &&
                    prefRegionId == startRegionId)
                {
                    cout << startRegionId;
                    ++numSubmittedRegions;

                    if (numSubmittedRegions < 6)
                    {
                        cout << " ";
                    }
                    else
                    {
                        cout << "\n";
                    }
                }
            }
        }
    }
    if (phase == "place_armies")
    {
        cout << botName << " place_armies " << ownedRegions[0] << " " << armiesLeft <<"\n";
    }
    if (phase == "attack/transfer")
    {
            cout << "No moves\n" ;
//          cout << botName << " attack/transfer " << from << " " << to << " "<< armiesMoved;
    }
    phase.clear();
}

void Bot::updateRegion(unsigned noRegion, string playerName, int nbArmies)
{
    regions[noRegion].setArmies(nbArmies);
    regions[noRegion].setOwner(playerName);
    if (playerName == botName)
    {
        ownedRegions.push_back(noRegion);
    }
}

void Bot::addArmies(unsigned noRegion, int nbArmies)
{
    regions[noRegion].setArmies(regions[noRegion].getArmies() + nbArmies);
}

void Bot::moveArmies(unsigned noRegion, unsigned toRegion, int nbArmies)
{
    if (regions[noRegion].getOwner() == regions[toRegion].getOwner()
        && regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
        regions[toRegion].setArmies(regions[toRegion].getArmies() + nbArmies);
    }
    else if (regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
    }
}

void Bot::resetRegionsOwned()
{
    ownedRegions.clear();
}

