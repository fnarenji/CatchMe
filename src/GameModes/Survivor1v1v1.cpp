#include <algorithm>
#include <sstream>

#include "Survivor1v1v1.h"
#include "Helpers.h"

#include "../Game.h"
#include "../Menu.h"

using namespace std;

namespace
{
	Game::CPositions ForbiddenPositions;
}

void Survivor1v1v1::GetSize (CPosition& Size)
{
    Menu::Clear ();
    
    Menu::AddItem ("Small map", [&Size] () { Size = { 5, 10 }; });
    Menu::AddItem ("Medium map", [&Size] () { Size = { 10, 20 }; });
    Menu::AddItem ("Great map", [&Size] () { Size = { 20, 40 }; });

    Menu::Run ();
}

void Survivor1v1v1::MovePlayer (const CMatrix& Matrix, CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)
{
    Helpers::MovePlayer (Matrix, PlayerPosition, MatrixSize, MoveY, MoveX);
}

void Survivor1v1v1::ValidatePlayerPositions (const CMatrix& Matrix, const CPositions& PlayerPositions, unsigned CurrentPlayer, vector<bool>& PlayerLifeStates)
{
	Helpers::ValidatePlayerPositionsNoTeam (PlayerPositions, CurrentPlayer, PlayerLifeStates);

    for (CPosition Position : ForbiddenPositions)
        if (PlayerPositions [CurrentPlayer] == Position)
            PlayerLifeStates [CurrentPlayer] = false;

    // If he just died, there will be someone else on his spot, so no obstacle yet.
    if(PlayerLifeStates[CurrentPlayer] && find (ForbiddenPositions.cbegin(), ForbiddenPositions.cend(), PlayerPositions [CurrentPlayer]) == ForbiddenPositions.cend())
        ForbiddenPositions.push_back (PlayerPositions [CurrentPlayer]);
}

void Survivor1v1v1::InitializeRound (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& MaxSize)
{
    PlayerPositions.resize (PlayerCount);

    PlayerPositions [0] = { 0, MaxSize.second - 1 }; // Top right
    PlayerPositions [1] = { MaxSize.first - 1, 0 }; // Bottom left
    PlayerPositions [2] = { 0, 0 }; // Top left
    
    ForbiddenPositions.clear ();
}

void Survivor1v1v1::BuildMatrix (CMatrix& Matrix, const CPositions& PlayerPositions, const vector<bool>& PlayerLifeStates, const char EmptyToken)
{
    for (CLine& Line : Matrix)
        fill (Line.begin (), Line.end (), EmptyToken);
    
    for (CPosition Position : ForbiddenPositions)
        Matrix [Position.first] [Position.second] = Game::KTokens [Game::KTokenObstacle];

    for (unsigned i = 0; i < PlayerPositions.size(); ++i)
        if (PlayerLifeStates[i])
            Matrix [PlayerPositions [i].first] [PlayerPositions [i].second] = Game::KTokens [i];

    std::stringstream FileName;
    FileName << "./" << Matrix.size() << "_" << Matrix.begin()->size() << ".map";

    Helpers::LoadObstaclesFromFile (Matrix, FileName.str());
}

bool Survivor1v1v1::IsGameOver (const vector<bool>& PlayerLifeStates)
{
    unsigned DeadPlayerCount = 0;

    for (bool PlayerState : PlayerLifeStates)
        if (!PlayerState)
            ++DeadPlayerCount;

    return DeadPlayerCount == 2;
}

