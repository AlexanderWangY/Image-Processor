#include "Header.h"
#include "Pixel.h"
#include <vector>

struct TempImage {
  std::vector<std::vector<Pixel>> imgVec;
  Header header;
};
