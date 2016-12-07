#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "game.h"

namespace shadow {
	void GetShadows( glm::ivec2 origin, CGame& game, std::vector<glm::ivec2>& hidden, std::vector<glm::ivec2>& visible );
}