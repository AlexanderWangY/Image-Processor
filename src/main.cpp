#include "Image.h"
#include <fstream>
#include <ios>
#include <iostream>
int main() {
  Image image("./input/car.tga");
  Image image2("./input/layer2.tga");

  Image output = image.Subtract(image2);

  output.outputImage("./output/testingSubtract.tga");

  return 0;
}
