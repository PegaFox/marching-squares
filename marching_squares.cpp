#include "marching_squares.hpp"
#include <iostream>

namespace pfsq
{
  static const float THRESHOLD = 0.0f;

  glm::vec2 biasedPos(const glm::vec2& pos1, const glm::vec2& pos2, float val1, float val2);

  std::vector<std::vector<glm::vec2>> generateRegion(glm::ivec2 pos, glm::ivec2 size, GetAt getAt)
  {
    std::vector<std::vector<glm::vec2>> polygons;
    for (int y = pos.y; y < pos.y + size.y-1; y++)
    {
      for (int x = pos.x; x < pos.x + size.x-1; x++)
      {
        glm::vec2 coords[4] = {
          glm::vec2(x, y),
          glm::vec2(x+1, y),
          glm::vec2(x+1, y+1),
          glm::vec2(x, y+1)
        };

        float vals[4] = {
          getAt(glm::vec2(x, y)),
          getAt(glm::vec2(x+1, y)),
          getAt(glm::vec2(x+1, y+1)),
          getAt(glm::vec2(x, y+1))
        };

        uint8_t mask = ((vals[0] > THRESHOLD) << 3) | ((vals[1] > THRESHOLD) << 2) | ((vals[2] > THRESHOLD) << 1) | ((vals[3] > THRESHOLD));

        uint8_t activeNum = (vals[0] > THRESHOLD) + (vals[1] > THRESHOLD) + (vals[2] > THRESHOLD) + (vals[3] > THRESHOLD);

        switch (activeNum)
        {
          case 1: {
            uint8_t startInd = 255;
            for (uint8_t i = 0; i < 4; i++)
            {
              if ((mask>>i) & 1)
              {
                startInd = 3-i;
                break;
              }
            }
            assert(startInd != 255);

            polygons.emplace_back(4);

            polygons.back()[0] = coords[startInd];
            polygons.back()[1] = biasedPos(coords[startInd], coords[(startInd+1) & 3], vals[startInd], vals[(startInd+1) & 3]);
            polygons.back()[2] = biasedPos(coords[startInd], coords[(startInd+2) & 3], vals[startInd], vals[(startInd+2) & 3] * 3.0f);
            polygons.back()[3] = biasedPos(coords[startInd], coords[(startInd-1) & 3], vals[startInd], vals[(startInd-1) & 3]);

            break;
          } case 2: {
            bool diagonal = false;
            uint8_t startInd = 255;
            for (uint8_t i = 0; i < 4; i++)
            {
              if ((mask>>i) & 1 && (mask>>((i+1)&3)) & 1)
              {
                startInd = (2-i) & 3;
                break;
              }
              if ((mask>>i) & 1 && (mask>>((i+2)&3)) & 1)
              {
                startInd = (3-i) & 3;
                diagonal = true;
                break;
              }
            }
            assert(startInd != 255);

            if (diagonal)
            {
              polygons.emplace_back(6);

              polygons.back()[0] = coords[startInd];
              polygons.back()[1] = biasedPos(coords[startInd], coords[(startInd+1) & 3], vals[startInd], vals[(startInd+1) & 3]);
              polygons.back()[2] = biasedPos(coords[(startInd+1) & 3], coords[(startInd+2) & 3], vals[(startInd+1) & 3], vals[(startInd+2) & 3]);

              polygons.back()[3] = coords[(startInd+2) & 3];
              polygons.back()[4] = biasedPos(coords[(startInd-1) & 3], coords[(startInd+2) & 3], vals[(startInd-1) & 3], vals[(startInd+2) & 3]);
              polygons.back()[5] = biasedPos(coords[startInd], coords[(startInd-1) & 3], vals[startInd], vals[(startInd-1) & 3]);
            } else
            {
              polygons.emplace_back(4);

              polygons.back()[0] = coords[startInd];
              polygons.back()[1] = coords[(startInd+1) & 3];
              polygons.back()[2] = biasedPos(coords[(startInd+1) & 3], coords[(startInd+2) & 3], vals[(startInd+1) & 3], vals[(startInd+2) & 3]);
              polygons.back()[3] = biasedPos(coords[startInd], coords[(startInd-1) & 3], vals[startInd], vals[(startInd-1) & 3]);
            }

            break;
          } case 3: {
            uint8_t startInd = 255;
            for (uint8_t i = 0; i < 4; i++)
            {
              if (!((mask>>i) & 1))
              {
                startInd = (3-i) & 3;
                break;
              }
            }
            assert(startInd != 255);
            
            polygons.emplace_back(5);

            polygons.back()[0] = biasedPos(coords[startInd], coords[(startInd+1) & 3], vals[startInd], vals[(startInd+1) & 3]);
            polygons.back()[1] = coords[(startInd+1) & 3];
            polygons.back()[2] = coords[(startInd+2) & 3];
            polygons.back()[3] = coords[(startInd-1) & 3];
            polygons.back()[4] = biasedPos(coords[startInd], coords[(startInd-1) & 3], vals[startInd], vals[(startInd-1) & 3]);

            break;
          } case 4:
            polygons.emplace_back(4);
            polygons.back()[0] = coords[0];
            polygons.back()[1] = coords[1];
            polygons.back()[2] = coords[2];
            polygons.back()[3] = coords[3];

            break;
          default:

            break;
        }
      }
    }

    return polygons;
  }

  glm::vec2 biasedPos(const glm::vec2& pos1, const glm::vec2& pos2, float val1, float val2)
  {
    // point will tend towards higher value

    //float bias = 0.5f + (val1/(MAX_VAL * 2.0f) + val2/(MAX_VAL * 2.0f));
    float bias = val1 == val2 ? 0.0f : val1 / (val1-val2);

    return pos1 + (pos2-pos1)*(bias);
  }
}
