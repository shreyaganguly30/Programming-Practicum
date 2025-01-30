#include <utility>
#include <random>
#include <set>
#include "grid.h"
#include "maze.h"
using namespace std;

// My netID
const string kYourNetID = "sgang29";

// Paths to get out of the mazes
const string kPathOutOfRegularMaze = "ESWSSNNENEESSWWS";
const string kPathOutOfTwistyMaze = "WWEESNWWWENWW";

bool isPathToFreedom(MazeCell *start, const string& moves) {
    // booleans to hold if you have the item or not
    bool spellbook = false;
    bool wand = false;
    bool potion = false;

    // check if start position has an item
    if(start->whatsHere == "Potion"){
        potion = true;
    }else if(start->whatsHere == "Spellbook"){
        spellbook = true;
    }else if(start->whatsHere == "Wand"){
        wand = true;
    }

    // parse through your maze key one letter at a time
    for(char currentMove : moves){
        // if N, move north
        if(currentMove == 'N' && start->north){
            start = start->north;
        }
        // if S, move south
        else if(currentMove == 'S' && start->south){
            start = start->south;
        }
        // if E, move east
        else if(currentMove == 'E' && start->east){
            start = start->east;
        }
        // if W, move west
        else if(currentMove == 'W' && start->west){
            start = start->west;
        }
        // if invalid, return false
        else{
            return false;
        }

        // check if position has an item
        if(start->whatsHere == "Potion"){
            potion = true;
        }else if(start->whatsHere == "Spellbook"){
            spellbook = true;
        }else if(start->whatsHere == "Wand"){
            wand = true;
        }
    }
    // return true if you have all three items
    return (spellbook && wand && potion);
}
