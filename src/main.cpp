#include "Image.h"
#include <fstream>
#include <ios>
#include <iostream>
int main() {
  Image image("./input/layer1.tga");
  Image image2("./input/text.tga");

  image.loadImage();
  image2.loadImage();

  image.Screen(image2, "./output/Part2.tga");

  return 0;
}
