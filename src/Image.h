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
  NormalizedPixel normalizePixel(Pixel &pixel);
  Pixel pixelize(NormalizedPixel &normalPixel);
  int clamp(int value);
  float overlayConditional(float p1, float p2);

public:
  Image();
  Image(std::string _imagePath);
  void setImagePath(std::string _imagePath);
  std::string getImagePath();
  void loadImage();
  void printHeader();
  void outputImage();
  void outputImage(std::string fileName);
  void importHeader(const Header &newHeader);
  void importImageVector(const std::vector<std::vector<Pixel>> &newImageVec);
  Header outputHeader();
  std::vector<std::vector<Pixel>> outputImageVector();

  // Operations

  void Multiply(Image &layer2);
  void Screen(Image &layer2);
  void Subtract(Image &layer2);
  void Addition(Image &layer2);
  void Overlay(Image &layer2);
  void ChangeChannel(int amount,
                     int channel); // 0 for blue, 1 for green, 2 for red
  void SubChannel(int amount, int channel);
  void MultChannel(int amount, int channel);
  void ExtractChannel(int channel);
  void IsolateChannel(int channel);
  void Rotate();
};
