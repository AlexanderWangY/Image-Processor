#pragma once
#include "Header.h"
#include "Pixel.h"
#include <fstream>
#include <string>
#include <vector>
class Image {
private:
  std::string imagePath;
  Header header;
  std::vector<std::vector<Pixel>> imageVec;
  std::fstream imageStream;

public:
  Image();
  Image(std::string _imagePath);

  void loadImage();
  void loadImage(std::string _imagePath);
  void loadHeader();
  void printHeader();
  void writeImage(std::string fileName);
  Image getImage();

  // Operations

  void Multiply(Image &layer2, std::string outputName);
  void Screen(Image &layer2, std::string outputName);
  void Subtract(Image &layer2, std::string outputName);
  void Addition(Image &layer2, std::string outputName);
  void Overlay(Image &layer2, std::string outputName);
};
