#include <fstream>
#include <sstream>

#include "Helpers.h"

using namespace NSGame;

void Helpers::MovePlayer (const CMatrix& Matrix, CPosition& PlayerPosition, const CPosition& MatrixSize, const PlayerMovesY MoveY, const PlayerMovesX MoveX)
{
    int DiffY = static_cast<int> (MoveY);
    int DiffX = static_cast<int> (MoveX);

    if (PlayerPosition.first + DiffY < 0 || PlayerPosition.first + DiffY >= MatrixSize.first)
        DiffY = 0;

    if (PlayerPosition.second + DiffX < 0 || PlayerPosition.second + DiffX >= MatrixSize.second)
        DiffX = 0;

    if (PlayerPosition.first + DiffY >= 0 && PlayerPosition.second + DiffX >= 0
        && PlayerPosition.first + DiffY <= MatrixSize.first && PlayerPosition.second + DiffX <= MatrixSize.second)
    {
        if (Matrix [PlayerPosition.first + DiffY] [PlayerPosition.second + DiffX] == KTokens [KTokenObstacle]) // Is direct movement impossbru ? Try to keep just X or Y movement
        {
            if (Matrix [PlayerPosition.first + DiffY] [PlayerPosition.second] == KTokens [KTokenObstacle])
                DiffY = 0;

            if (Matrix [PlayerPosition.first] [PlayerPosition.second + DiffX] == KTokens [KTokenObstacle])
                DiffX = 0;
        }
    }

    PlayerPosition.first += DiffY;
    PlayerPosition.second += DiffX;
}

void Helpers::ValidatePlayerPositionsNoTeam (const CPositions& PlayerPositions, unsigned CurrentPlayer, std::vector<bool>& PlayerLifeStates)
{
	for (unsigned i = 0; i < PlayerPositions.size (); ++i)
    {
        if (i == CurrentPlayer || !PlayerLifeStates[i])
            continue;

        if (PlayerPositions [CurrentPlayer] == PlayerPositions [i])
        {
            PlayerLifeStates[i] = false;
            break;
        }
    }
}

void Helpers::LoadObstaclesFromFile (CPositions& ObstaclesPositions, const CPosition& MaxSize)
{
    std::stringstream FileName;
    FileName << "./" << MaxSize.first << "_" << MaxSize.second << ".map";

	std::ifstream File (FileName.str ());

	if (!File)
    {
        return; // @todo Remove this once every file was created and populated
		throw std::runtime_error("There was an error trying to open the file: " + FileName.str ());
    }

	unsigned Y, X;
	while (File >> Y >> X)
		if (MaxSize.first > Y && MaxSize.second > X)
			ObstaclesPositions.push_back ({ Y, X });
}

void Helpers::AddObstaclesAndPlayersToMatrix (CMatrix& Matrix, const CPositions& PlayerPositions, const std::vector<bool>& PlayerLifeStates, const CPositions& ObstaclesPositions, char EmptyToken)
{
    for (CLine& Line : Matrix)
        fill (Line.begin (), Line.end (), EmptyToken);

    for (CPosition Position : ObstaclesPositions)
        Matrix [Position.first] [Position.second] = NSGame::KTokens [NSGame::KTokenObstacle];

    for (unsigned i = 0; i < PlayerPositions.size (); ++i)
        if (PlayerLifeStates[i])
            Matrix [PlayerPositions [i].first] [PlayerPositions [i].second] = NSGame::KTokens [i];
}


