#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <chrono>

namespace Game
{
    enum struct PlayerMovesX : int
    {
        KUp   = -1,
        KStay = 0,
        KDown = 1,
    };
    
    enum struct PlayerMovesY : int
    {
        KLeft  = -1,
        KStay  = 0,
        KRight = 1,
    };
    
    typedef std::vector<char>       CLine;
    typedef std::vector<CLine>      CMatrix;
    typedef std::pair<int, int>     CPosition;
    typedef std::vector<CPosition>  CPositions;

    // Constants
    const std::pair<unsigned, unsigned> KMatrixSize              = { 10, 20 }; // X, Y
    const unsigned                      KMaxPlayerCount          = 4;
    const unsigned                      KMinPlayerCount          = 2; // @todo This should be lowered to one if/once the "Player vs Computer" mode is ready.
    const std::chrono::milliseconds     KErrorMessageDisplayTime(2000);

    // List of all the possible tokens
    const std::vector<char> KTokens =
    {
        'X',
        'O',
        '#',
        '@',
        ' '
    };
    
    // Controls assiocated to (player) tokens
    const std::map<char, std::string> KControlsByToken =
    {
        { 'X', "azeqsdwxc" }, // On your AZERTY keyboard, theses charcters should form a square. Top left is one up, one left, top center is one up etc etc etc
        { 'O', "987456123" },
        { '#', "uiojkl,;:" },
        { '@', "rtyfghvbn" }
    };
    
    int Run ();
}