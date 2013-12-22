#include "Game.h"
#include "GameUI.h"

#include <iostream>
#include <limits>
#include <thread>
#include <ncurses.h>

using namespace std;

namespace
{
    using namespace Game;

    /**
     * 
     * @brief Ask user how many players he wants.
     * 
     **/
    void GetPlayerCount (unsigned& PlayerCount)
    {
        do
        {
            cout << "How many players do want you to play with ("<< KMinPlayerCount << "-" << KMaxPlayerCount <<") ? " << flush;

            char c = cin.get ();
            Console::ClearInputBuffer();

            if (!isdigit (c))
            {
                cerr << "You must enter a digit." << endl;
                continue;
            }

            PlayerCount = c - '0'; // Digits in the ASCII table are sorted in ascending order.

            if (PlayerCount < KMinPlayerCount || PlayerCount > KMaxPlayerCount)
            {
                cerr << "The player count must be between " << KMinPlayerCount << " and " << KMaxPlayerCount << "." << endl;
                PlayerCount = 0;
            }
            else
                cout << "There will be " << PlayerCount << " players." << endl;

        } while (0 == PlayerCount);
    }

    /**
     * 
     * Update the Matrix to reflect the new positions of the players.
     * 
     **/
    void RefreshMatrix (CMatrix& Matrix, const CPositions& PlayerPositions, const char EmptyToken)
    {
        for (CLine& Line : Matrix)
            fill (Line.begin (), Line.end (), EmptyToken);

        for (unsigned i = 0; i < PlayerPositions.size(); ++i)
            Matrix  [PlayerPositions[i].first] [PlayerPositions[i].second] = KTokens [i];
    }

    /**
     * 
     * @brief Set the size of the Matrix
     * 
     **/
    void InitializeMatrix (CMatrix& Matrix, const CPosition& Size, const CPositions& PlayerPositions, const char EmptyToken)
    {
        Matrix.resize (Size.first);

        for (CLine& Line : Matrix)
            Line.resize (Size.second);

        RefreshMatrix (Matrix, PlayerPositions, EmptyToken);
    }

    /**
     * 
     * @brief Each player gets a corner of the grid.
     * 
     * @todo Randomize each player number (so that the same player doesn't start in the same spot everytime).
     * 
     * Player 1 is top right.
     * Player 2 is bottom left.
     * Player 3 is top left.
     * Player 4 is bottom right.
     * 
     **/
    void InitializePlayerPositions (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& Size)
    {
        PlayerPositions.resize (PlayerCount);

        for (unsigned i = 0; i < PlayerCount; ++i)
        {
            switch (i)
            {
                case 0:
                    PlayerPositions[0] = { 0, Size.second - 1 }; // Top right
                    break;
                case 1:
                    PlayerPositions[1] = { Size.first - 1, 0 }; // Bottom left
                    break;
                case 2:
                    PlayerPositions[2] = { 0, 0 }; // Top left
                    break;
                case 3:
                    PlayerPositions[3] = { Size.first - 1, Size.second - 1 }; // Bottom right
                    break;
            }
        }
    }
    
    /**
     * 
     * @brief Moves a player in a certain direction, checks for walls and plays beeping sound if wall is hit (last part is not tested).
     * 
     * @param hardWalls This determines whether a player might or might not go through the wall.
     * 
     * @todo hardWalls should be a float between 0 and 1 determining the probabilty of the player going through the wall (or failing to do so).
     * 
     **/
    void MovePlayer(CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesX MoveX, const PlayerMovesY MoveY, bool hardWalls = true)
    {
        int DiffX = static_cast<int> (MoveX);
        int DiffY = static_cast<int> (MoveY);
        
        // Make him pop on the other side if he leaves the boundaries
        if (PlayerPosition.first + DiffX < 0)
            if (!hardWalls)
                PlayerPosition.first = MatrixSize.first - 1; 
            else beep();
        else if (PlayerPosition.first + DiffX > MatrixSize.first)
            if (!hardWalls)
                PlayerPosition.first = 0;
            else beep();
        else
            PlayerPosition.first += DiffX;

        if (PlayerPosition.second + DiffY < 0)
            if (!hardWalls)
                PlayerPosition.second = MatrixSize.second - 1;
            else beep();
        else if (PlayerPosition.second + DiffY > MatrixSize.second)
            if (!hardWalls)
                PlayerPosition.second = 0;
            else beep();
        else
            PlayerPosition.second += DiffY;
    }
    
    /**
     * 
     * @brief Gets the input, checks it and calls the MovePlayer function according to which key was pressed.
     * 
     */
    bool HandleMovement(CPosition& PlayerPosition, const unsigned CurrentPlayer)
    {
        char Opcode = cin.get ();
        Console::ClearInputBuffer();

        size_t Action = KControlsByToken.at (KTokens.at (CurrentPlayer)).find(Opcode);

        if (Action == string::npos)
        {
            cout << "The key you entered wasn't valid." << endl;
            this_thread::sleep_for (KErrorMessageDisplayTime); // Wait a defined amount of time for the message to be shown.
            return false; // The player failed. Let's render the grid again and ask him once more.
        }

        switch (Action)
        {
            case 0: // Up & left
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KUp, PlayerMovesY::KLeft);
                break;
            case 1: // Up
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KUp, PlayerMovesY::KStay);
                break;
            case 2: // Up & right
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KUp, PlayerMovesY::KRight);
                break;
            case 3: // Left
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KStay, PlayerMovesY::KLeft);
                break;
            case 4: // Stay
                return false; // Having a Stay move has been found to flaw the gameplay.
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KStay, PlayerMovesY::KStay);
                break;
            case 5: // Right
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KStay, PlayerMovesY::KRight);
                break;
            case 6: // Down & left
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KDown, PlayerMovesY::KLeft);
                break;
            case 7: // Down
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KDown, PlayerMovesY::KStay);
                break;
            case 8: // Down & right
                MovePlayer (PlayerPosition, KMatrixSize, PlayerMovesX::KDown, PlayerMovesY::KRight);
                break;
        }

        return true;
    }
}

int Game::Run ()
{
    CPositions  PlayerPositions;
    unsigned    PlayerCount = 0;
    unsigned    CurrentPlayer = 0; // Whose turn it is
    CMatrix     Matrix;
    
    GetPlayerCount (PlayerCount);
    InitializePlayerPositions (PlayerPositions, PlayerCount, KMatrixSize);
    InitializeMatrix (Matrix, KMatrixSize, PlayerPositions, (*KTokens.rbegin ()));
    
    for (;;)
    {
        UI::ShowMatrix (Matrix);
        UI::ShowControls (CurrentPlayer);
        
        if (!HandleMovement (PlayerPositions [CurrentPlayer], CurrentPlayer))
            continue;

        RefreshMatrix (Matrix, PlayerPositions, (*KTokens.rbegin ()));

        CurrentPlayer++;
        if (CurrentPlayer >= PlayerCount)
            CurrentPlayer = 0;
    }

    return 0;
}