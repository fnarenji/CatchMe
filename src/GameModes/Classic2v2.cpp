#include "Classic2v2.h"
#include "Helpers.h"

#include "../Game.h"
#include "../Menu.h"

using namespace std;

namespace
{
    Game::CPositions ObstaclesPositions;
}

void Classic2v2::GetSize (CPosition& Size)
{
    Menu::Clear ();

    Menu::AddItem ("Small map", [&Size] () { Size = { 5, 10 }; });
    Menu::AddItem ("Medium map", [&Size] () { Size = { 10, 20 }; });
    Menu::AddItem ("Great map", [&Size] () { Size = { 20, 40 }; });

    Menu::Run ();
}

void Classic2v2::MovePlayer (const CMatrix& Matrix, CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)
{
    Helpers::MovePlayer (Matrix, PlayerPosition, MatrixSize, MoveY, MoveX);
}

void Classic2v2::ValidatePlayerPositions (const CMatrix& Matrix, const CPositions& PlayerPositions, unsigned CurrentPlayer, vector<bool>& PlayerLifeStates)
{
    for (unsigned i = 0; i < PlayerPositions.size (); ++i)
    {
        if (i == CurrentPlayer || !PlayerLifeStates[i] || (i + CurrentPlayer) % 2 == 0) // Player 1 & 3 and 2 & 4 are teamed up.
            continue;

        if (PlayerPositions [CurrentPlayer] == PlayerPositions [i])
            PlayerLifeStates[i] = false;
    }
}

void Classic2v2::InitializeRound (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& MaxSize)
{
    PlayerPositions.resize (PlayerCount);

    PlayerPositions [0] = { 0, MaxSize.second - 1 }; // Top right
    PlayerPositions [1] = { 0, 0 }; // Top left
    PlayerPositions [2] = { MaxSize.first - 1, 0 }; // Bottom left
    PlayerPositions [3] = { MaxSize.first - 1, MaxSize.second - 1 }; // Bottom right

    ObstaclesPositions.clear ();
    Helpers::LoadObstaclesFromFile (ObstaclesPositions, MaxSize);

    Menu::Clear ();

    Menu::AddItem ("The players 1 and 3 form a team against the players 2 and 4");

    Menu::Run ();
}

void Classic2v2::BuildMatrix (CMatrix& Matrix, const CPositions& PlayerPositions, const vector<bool>& PlayerLifeStates, const char EmptyToken)
{
    Helpers::AddObstaclesAndPlayersToMatrix (Matrix, PlayerPositions, PlayerLifeStates, ObstaclesPositions, EmptyToken);
}

bool Classic2v2::IsGameOver (const vector<bool>& PlayerLifeStates)
{
    return (!PlayerLifeStates[0] && !PlayerLifeStates[2]) || (!PlayerLifeStates[1] && !PlayerLifeStates[3]);
}
