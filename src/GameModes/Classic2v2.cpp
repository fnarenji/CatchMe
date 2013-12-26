#include "Classic2v2.h"
#include "Helpers.h"

#include "../Game.h"

using namespace std;

/**
 *
 * @todo Add a menu here, that allows the player to selection the size of the grid he wants to play on.
 * 
 **/
void Classic2v2::GetSize (CPosition& Size)
{
    Size = { 4, 4 };
}

void Classic2v2::MovePlayer (CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)
{
    Helpers::MovePlayer(PlayerPosition, MatrixSize, MoveY, MoveX);
}

void Classic2v2::ValidatePlayerPositions (CPositions PlayerPositions, unsigned CurrentPlayer, vector<bool>& PlayerStates)
{
  for (unsigned i = 0; i < PlayerPositions.size (); ++i)
  {
    if (i == CurrentPlayer || !PlayerStates[i] || (i + CurrentPlayer) % 2 == 0)
      continue;
    
    if (PlayerPositions[CurrentPlayer].first == PlayerPositions[i].first
      && PlayerPositions[CurrentPlayer].second == PlayerPositions[i].second)
    {
      PlayerStates[i] = false;
    }
  }
}

void Classic2v2::InitializePlayerPositions (CPositions& PlayerPositions, const unsigned PlayerCount, const CPosition& MaxSize)
{
    PlayerPositions.resize (PlayerCount);

    for (unsigned i = 0; i < PlayerCount; ++i)
    {
        switch (i)
        {
            case 0:
                PlayerPositions [0] = { 0, MaxSize.second - 1 }; // Top right
                break;
            case 1:
                PlayerPositions [1] = { MaxSize.first - 1, 0 }; // Bottom left
                break;
            case 2:
                PlayerPositions[2] = { 0, 0}; // Top left
                break;
            case 3:
                PlayerPositions[3] = { MaxSize.first - 1, MaxSize.second - 1 }; // Bottom right
                break;
        }
    }
}

void Classic2v2::BuildMatrix (CMatrix& Matrix, const CPositions& PlayerPositions, const char EmptyToken)
{
    for (CLine& Line : Matrix)
        fill (Line.begin (), Line.end (), EmptyToken);

    for (unsigned i = 0; i < PlayerPositions.size (); ++i)
        Matrix  [PlayerPositions [i].first] [PlayerPositions [i].second] = Game::KTokens [i];
}

bool Classic2v2::IsGameOver (const vector<bool>& PlayerStates)
{
    unsigned DeadPlayerCount = 0;
    for (bool PlayerState : PlayerStates)
        if (!PlayerState)
            ++DeadPlayerCount;
    if (DeadPlayerCount ==2 && PlayerStates[0] == PlayerStates [2])
        ++DeadPlayerCount;
    return DeadPlayerCount == 3;
}