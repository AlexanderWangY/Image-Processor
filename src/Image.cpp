#include "Image.h"
#include "Pixel.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <vector>

Image::Image() { imagePath = ""; }; // Default contructor (nothing happens)

Image::Image(std::string _imagePath) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imagePath = _imagePath;

  imageStream.open(imagePath, std::ios::binary | std::ios::in | std::ios::out);

  if (imageStream.is_open()) {
    std::cout << "File found! Loading image..." << std::endl;
    this->loadImage();
    imageStream.close();
  } else {
    imageStream.close();
    std::cout << "File not found, saving filename..." << std::endl;
  }
}

void Image::outputImage() {
  std::cout << "Outputting image: " << imagePath << std::endl;

  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(imagePath, std::ios::binary | std::ios::out);

  this->writeHeader();
  this->writeImageVector();

  imageStream.close();
}

void Image::outputImage(std::string fileName) {
  std::cout << "Outputting image: " << fileName << std::endl;

  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(fileName, std::ios::binary | std::ios::out);

  this->writeHeader();
  this->writeImageVector();

  imageStream.close();
}

void Image::importHeader(const Header &newHeader) { this->header = newHeader; }

void Image::importImageVector(
    const std::vector<std::vector<Pixel>> &newImageVector) {
  this->imageVec = newImageVector;
}

void Image::printHeader() {
  // For testing!
  std::cout << "ID Length: " << static_cast<int>(header.idLength)
            << "\ncolorMapType: " << static_cast<int>(header.colorMapType)
            << "\nDataTypeCode: " << static_cast<int>(header.dataTypeCode)
            << "\nColorMapOrigin: " << header.colorMapOrigin
            << "\nColorMapLength: " << header.colorMapLength
            << "\nColorMapDepth: " << static_cast<int>(header.colorMapDepth)
            << "\nXOrigin: " << header.xOrigin
            << "\nYOrigin: " << header.yOrigin << "\nWidth: " << header.width
            << "\nHeight: " << header.height
            << "\nBitsPerPixel: " << static_cast<int>(header.bitsPerPixel)
            << "\nImageDescriptor: " << static_cast<int>(header.imageDescriptor)
            << std::endl;
}

// Operation functions!

Image Image::Multiply(Image &layer2) {
  Image outputImage;
  Header outputHeader = header;
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < outputHeader.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < outputHeader.width; y++) {
      NormalizedPixel newNormalPixel_L1 =
          this->normalizePixel(this->imageVec[x][y]);
      NormalizedPixel newNormalPixel_L2 =
          this->normalizePixel(layer2.imageVec[x][y]);

      NormalizedPixel normalMultiply;
      normalMultiply.blue = newNormalPixel_L1.blue * newNormalPixel_L2.blue;
      normalMultiply.green = newNormalPixel_L1.green * newNormalPixel_L2.green;
      normalMultiply.red = newNormalPixel_L1.red * newNormalPixel_L2.red;

      Pixel newPixel = this->pixelize(normalMultiply);

      newRow.push_back(newPixel);
    }

    outputImageVector.push_back(newRow);
  }

  outputImage.importHeader(outputHeader);
  outputImage.importImageVector(outputImageVector);

  return outputImage;
}

Image Image::Subtract(Image &layer2) {
  Image outputImage;
  Header outputHeader = header;
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < outputHeader.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < outputHeader.width; y++) {
      Pixel newPixel;

      newPixel.blue =
          (unsigned char)(this->clamp((int)this->imageVec[x][y].blue) -
                          (int)layer2.imageVec[x][y].blue);
      newPixel.green =
          (unsigned char)(this->clamp((int)this->imageVec[x][y].green) -
                          (int)layer2.imageVec[x][y].green);
      newPixel.red =
          (unsigned char)(this->clamp((int)this->imageVec[x][y].red) -
                          (int)layer2.imageVec[x][y].red);

      newRow.push_back(newPixel);
    }
    outputImageVector.push_back(newRow);
  }

  outputImage.importHeader(outputHeader);
  outputImage.importImageVector(outputImageVector);
  return outputImage;
}

// Helper function declarations

void Image::readHeader() {
  imageStream.read(reinterpret_cast<char *>(&header.idLength), sizeof(char));
  imageStream.read(reinterpret_cast<char *>(&header.colorMapType),
                   sizeof(char));
  imageStream.read(reinterpret_cast<char *>(&header.dataTypeCode),
                   sizeof(char));
  imageStream.read(reinterpret_cast<char *>(&header.colorMapOrigin),
                   sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.colorMapLength),
                   sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.colorMapDepth),
                   sizeof(char));
  imageStream.read(reinterpret_cast<char *>(&header.xOrigin), sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.yOrigin), sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.width), sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.height), sizeof(short));
  imageStream.read(reinterpret_cast<char *>(&header.bitsPerPixel),
                   sizeof(char));
  imageStream.read(reinterpret_cast<char *>(&header.imageDescriptor),
                   sizeof(char));
}

void Image::writeHeader() {
  imageStream.write(reinterpret_cast<char *>(&header.idLength), sizeof(char));
  imageStream.write(reinterpret_cast<char *>(&header.colorMapType),
                    sizeof(char));
  imageStream.write(reinterpret_cast<char *>(&header.dataTypeCode),
                    sizeof(char));
  imageStream.write(reinterpret_cast<char *>(&header.colorMapOrigin),
                    sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.colorMapLength),
                    sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.colorMapDepth),
                    sizeof(char));
  imageStream.write(reinterpret_cast<char *>(&header.xOrigin), sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.yOrigin), sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.width), sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.height), sizeof(short));
  imageStream.write(reinterpret_cast<char *>(&header.bitsPerPixel),
                    sizeof(char));
  imageStream.write(reinterpret_cast<char *>(&header.imageDescriptor),
                    sizeof(char));
}

void Image::writeImageVector() {
  for (std::vector<Pixel> row : imageVec) {
    for (Pixel pixel : row) {
      this->writePixel(pixel);
    }
  }
}

void Image::readImageVector() {
  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      Pixel newPixel;
      readPixel(newPixel);
      newRow.push_back(newPixel);
    }

    imageVec.push_back(newRow);
  }
}

void Image::readPixel(Pixel &newPixel) {
  imageStream.read(reinterpret_cast<char *>(&newPixel.blue),
                   sizeof(unsigned char));
  imageStream.read(reinterpret_cast<char *>(&newPixel.green),
                   sizeof(unsigned char));
  imageStream.read(reinterpret_cast<char *>(&newPixel.red),
                   sizeof(unsigned char));
}

void Image::writePixel(Pixel &pixel) {
  imageStream.write(reinterpret_cast<char *>(&pixel.blue),
                    sizeof(unsigned char));
  imageStream.write(reinterpret_cast<char *>(&pixel.green),
                    sizeof(unsigned char));
  imageStream.write(reinterpret_cast<char *>(&pixel.red),
                    sizeof(unsigned char));
}

void Image::loadImage() {
  this->readHeader();
  this->readImageVector();
}

NormalizedPixel Image::normalizePixel(Pixel &pixel) {
  NormalizedPixel normalizedPixel;

  normalizedPixel.blue = (int)pixel.blue / 255.0;
  normalizedPixel.green = (int)pixel.green / 255.0;
  normalizedPixel.red = (int)pixel.red / 255.0;

  return normalizedPixel;
}

Pixel Image::pixelize(NormalizedPixel &normalPixel) {
  Pixel newPixel;
  newPixel.blue = (unsigned char)((int)(normalPixel.blue * 255.0 + 0.5));
  newPixel.green = (unsigned char)((int)(normalPixel.green * 255.0 + 0.5));
  newPixel.red = (unsigned char)((int)(normalPixel.red * 255.0 + 0.5));

  return newPixel;
}

int Image::clamp(int value) {
  if (value < 0) {
    return 0;
  } else if (value > 255) {
    return 255;
  } else {
    return value;
  }
}
