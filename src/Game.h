/**
 * @author F. Narenji
 *
 * @date   22/12/2013
 *
 * @brief  Main game header, contains tokens, controls, movement enums, player count and size constants
 *
**/
#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <chrono>

#include "GameMode.h"
#include "GameModes/Classic1v1.h"
#include "GameModes/Classic1v1v1.h"
#include "GameModes/Classic1v1v1v1.h"
#include "GameModes/Classic2v2.h"

namespace Game
{
     // Constants
    const unsigned                      KMaxPlayerCount          = 4;
    const unsigned                      KMinPlayerCount          = 2; // @todo This should be lowered to one if/once the "Player vs Computer" mode is ready.
    const std::vector<SGameMode>        KGameModes = 
    {
        MakeGameMode ("Classic 1v1", 2, Classic1v1::GetSize, Classic1v1::MovePlayer, Classic1v1::InitializePlayerPositions, Classic1v1::BuildMatrix, Classic1v1::ValidatePlayerPositions, Classic1v1::IsGameOver),
        MakeGameMode ("Classic 1v1v1", 3, Classic1v1v1::GetSize, Classic1v1v1::MovePlayer, Classic1v1v1::InitializePlayerPositions, Classic1v1v1::BuildMatrix, Classic1v1v1::ValidatePlayerPositions, Classic1v1v1::IsGameOver),
        MakeGameMode ("Classic 1v1v1v1", 4, Classic1v1v1v1::GetSize, Classic1v1v1v1::MovePlayer, Classic1v1v1v1::InitializePlayerPositions, Classic1v1v1v1::BuildMatrix, Classic1v1v1v1::ValidatePlayerPositions, Classic1v1v1v1::IsGameOver),
        MakeGameMode ("Classic 2v2", 4, Classic2v2::GetSize, Classic2v2::MovePlayer, Classic2v2::InitializePlayerPositions, Classic2v2::BuildMatrix, Classic2v2::ValidatePlayerPositions, Classic2v2::IsGameOver),
    };

    /**
     * 
     * The duration during which the game will be suspended to let the player read the error message.
     * 
     * @warning This should only be used for gameplay related error messages, such as wrong key
     *          and not for technical problems (such as exceptions reports).
     * 
     */
    const std::chrono::milliseconds KErrorMessageDisplayTime (2000);

    /**
     * 
     * The interval between each render loop.
     * 
     */
    const std::chrono::milliseconds KRenderLoopInterval (100);

    /**
     *
     * @brief List of all the possible tokens.
     * 
     * A token is an item that occupies a case on the grid, such as nothing, a player, or an obstacle.
     * These, however, do not include the borders.
     * The last token is the empty token.
     * 
     **/
    const std::vector<char> KTokens =
    {
        'X',
        'O',
        '#',
        '@',
        ' '
    };
    
    /**
     *
     * @brief Associates each (player) token with a list of controls
     * 
     * Every player token has its own set of controls, composed of 9 keyboard keys, which should form a square.
     * 0 -> Upleft, 1 -> Up, 2 -> Upright
     * 3 -> Left, 4 -> Stay (disabled), 5 -> Right
     * 6 -> Downleft, 7 -> Down, 8 -> Downright
     * 
     **/
    const std::map<char, std::string> KControlsByToken =
    {
        { 'X', "azeqsdwxc" },
        { 'O', "789456123" },
        { '#', "uiojkl,;:" },
        { '@', "rtyfghvbn" }
    };
    
    /**
     * 
     * @brief Main loop, handles just about everything.
     * 
     * If you want to start somewhere, this is a good place.
     * 
     **/
    int Run ();
}