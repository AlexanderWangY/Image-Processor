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

  // Helper functions
  void readHeader();
  void writeHeader();
  void writeImageVector();
  void readImageVector();
  void readPixel(Pixel &newPixel);
  void writePixel(Pixel &pixel);
  void loadImage();
  NormalizedPixel normalizePixel(Pixel &pixel);
  Pixel pixelize(NormalizedPixel &normalPixel);
  int clamp(int value);

public:
  Image();
  Image(std::string _imagePath);
  void printHeader();
  void outputImage();
  void outputImage(std::string fileName);
  void importHeader(const Header &newHeader);
  void importImageVector(const std::vector<std::vector<Pixel>> &newImageVec);

  // Operations

  Image Multiply(Image &layer2);
  Image Screen(Image &layer2);
  Image Subtract(Image &layer2);
  Image Addition(Image &layer2);
  Image Overlay(Image &layer2);
};
