#pragma once

#include <vector>
#include <functional>

namespace Game
{
    enum struct PlayerMovesY : int
    {
        KUp   = -1,
        KStay = 0,
        KDown = 1,
    };

    enum struct PlayerMovesX : int
    {
        KLeft  = -1,
        KStay  = 0,
        KRight = 1,
    };

    typedef std::vector<char>       CLine;
    typedef std::vector<CLine>      CMatrix;
    typedef std::pair<int, int>     CPosition;
    typedef std::vector<CPosition>  CPositions;

    struct SGameMode
    {
        std::string Name;
        unsigned PlayerCount;
        
        /**
         * 
         * @brief Size of the grid
         * 
         **/
        std::function<void (CPosition& Size)> GetSize;
        
        /**
         * 
         * @brief Gets the input, checks it and calls the MovePlayer function according to which key was pressed.
         * 
         **/
        // Lol removed

        /**
         * 
         * @brief Moves a player in a certain direction, checks for walls and plays beeping sound if wall is hit (last part is not tested).
         * 
         **/
        std::function<void (const CMatrix& Matrix, CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)> MovePlayer;
        
        /**
         * 
         * @brief Each player gets a corner of the grid.
         * 
         * @todo Randomize each player number (so that the same player doesn't start in the same spot everytime).
         * 
		 * Ex:
         * 	Player 1 is top right.
         * 	Player 2 is bottom left.
         * 	Player 3 is top left.
         * 	Player 4 is bottom right.
         * 
         **/
        std::function<void (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& Size)> InitializePlayerPosition;

        /**
         * 
         * @brief Update the Matrix to reflect the new positions of the players, state of the ground, barriers etc etc
         * 
         **/
        std::function<void (CMatrix& Matrix, const CPositions& PlayerPositions, const std::vector<bool>& PlayerLifeStates, const char EmptyToken)> BuildMatrix;
	
        /**
         * 
         * @brief Checks if a players position overlaps with another one
         * 
         **/
        std::function<void (CPositions PlayerPositions, unsigned CurrentPlayer, std::vector<bool>& PlayerLifeStates)> ValidatePlayerPositions;
        
        /**
         * 
         * @brief Returns whether or not the game is over
         * 
         **/
        std::function<bool (const std::vector<bool>& PlayerLifeStates)> IsGameOver;
    };

    SGameMode MakeGameMode (std::string Name, unsigned PlayerCount,
        std::function<void (CPosition& Size)> GetSize,
        std::function<void (const CMatrix& Matrix, CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)> MovePlayer,
        std::function<void (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& Size)> InitializePlayerPosition,
        std::function<void (CMatrix& Matrix, const CPositions& PlayerPositions, const std::vector<bool>& PlayerLifeStates, const char EmptyToken)> BuildMatrix,
        std::function<void (CPositions PlayerPositions, unsigned CurrentPlayer, std::vector<bool>& PlayerLifeStates)> ValidatePlayerPositions,
        std::function<bool (const std::vector<bool>& PlayerLifeStates)> IsGameOver);
}
