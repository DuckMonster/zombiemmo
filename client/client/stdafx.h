#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <exception>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include <glt/GLT.h>
#include <glt/Shader.hpp>
#include <glt/Stencil.hpp>
#include <glt/FrameBuffer.hpp>
#include <glt/LogLevel.hpp>
#include <glt/Time.hpp>
#include <SOIL/SOIL.h>
#include <protocol.h>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <net/client.hpp>
#include <bstream.hpp>
#include <types.h>

#include "timer.h"
#include "tiledrawer.h"
#include "mathutils.hpp"
#include "colorutils.h"
#include "line.h"