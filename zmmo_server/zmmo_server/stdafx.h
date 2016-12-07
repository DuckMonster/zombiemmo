#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <exception>
#include <random>
#include <thread>
#include <mutex>

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

#include <net/server.hpp>
#include <bstream.hpp>

#include "config.h"
#include "rect.h"
#include "types.h"
#include "network.h"
#include "mathutils.hpp"
#include "program.h"
#include "collection.h"