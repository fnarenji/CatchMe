#pragma once

#include "../Game.h"

#include <vector>

namespace Helpers
{
    void MovePlayer (const Game::CMatrix& Matrix, Game::CPosition& PlayerPosition, const Game::CPosition& MatrixSize, const Game::PlayerMovesY MoveY, const Game::PlayerMovesX MoveX);
	void ValidatePlayerPositionsNoTeam (const Game::CPositions& PlayerPositions, unsigned CurrentPlayer, std::vector<bool>& PlayerLifeStates);
	void LoadObstaclesFromFile (Game::CMatrix& Matrix, std::string FileName);
}