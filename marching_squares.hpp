#ifndef PEGAFOX_MARCHING_SQUARES_HPP
#define PEGAFOX_MARCHING_SQUARES_HPP

#include <vector>
#include <glm/glm.hpp>

namespace pfsq
{
  typedef float (*GetAt)(glm::ivec2 pos);

  std::vector<std::vector<glm::vec2>> generateRegion(glm::ivec2 pos, glm::ivec2 size, GetAt getAt);
}

#endif // PEGAFOX_MARCHING_SQUARES_HPP
