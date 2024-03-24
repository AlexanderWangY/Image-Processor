#include "Image.h"
#include <fstream>
#include <ios>
#include <iostream>
int main() {
  Image car("./input/car.tga");
  Image circles("./input/circles.tga");
  Image layer1("./input/layer1.tga");
  Image layer2("./input/layer2.tga");
  Image layer_blue("./input/layer_blue.tga");
  Image layer_green("./input/layer_green.tga");
  Image layer_red("./input/layer_red.tga");
  Image pattern1("./input/pattern1.tga");
  Image pattern2("./input/pattern2.tga");
  Image text("./input/text.tga");
  Image text2("./input/text2.tga");

  // Part 1
  Image part1 = layer1.Multiply(pattern1);
  part1.outputImage("./output/part1.tga");

  // Part 2
  Image part2 = car.Subtract(layer2);
  part2.outputImage("./output/part2.tga");

  // Part 3
  Image part3 = layer1.Multiply(pattern2).Screen(text);
  part3.outputImage("./output/part3.tga");

  // Part 4
  Image part4 = layer2.Multiply(circles).Subtract(pattern2);
  part4.outputImage("./output/part4.tga");

  // Part 5
  Image part5 = layer1.Overlay(pattern1);
  part5.outputImage("./output/part5.tga");

  // Part 6
  Image part6 = car.ChangeChannel(200, 1);
  part6.outputImage("./output/part6.tga");

  // Part 7
  Image part7 = car.MultChannel(4, 2).MultChannel(0, 0);
  part7.outputImage("./output/part7.tga");

  // Part 8 B
  Image part8_b = car.ExtractChannel(0);
  part8_b.outputImage("./output/part8_b.tga");

  // Part 8 G
  Image part8_g = car.ExtractChannel(1);
  part8_g.outputImage("./output/part8_g.tga");

  // Part 8 R
  Image part8_r = car.ExtractChannel(2);
  part8_r.outputImage("./output/part8_r.tga");

  // Part 9
  Image isolated_b = layer_blue.IsolateChannel(0);
  Image isolated_g = layer_green.IsolateChannel(1);
  Image isolated_r = layer_red.IsolateChannel(2);

  Image part9 = isolated_b.Addition(isolated_g).Addition(isolated_r);
  part9.outputImage("./output/part9.tga");

  // Part 10
  Image part10 = text2.Rotate();
  part10.outputImage("./output/part10.tga");

  return 0;
}
