#include "Image.h"
#include "Pixel.h"
#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <string>
#include <vector>

Image::Image() { imagePath = ""; }; // Default contructor (nothing happens)

Image::Image(std::string _imagePath) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imagePath = _imagePath;

  imageStream.open(imagePath, std::ios::binary | std::ios::in | std::ios::out);

  if (imageStream.is_open()) {
    this->loadImage();
    imageStream.close();
  } else {
    imageStream.close();
  }
}

void Image::setImagePath(std::string _imagePath) { imagePath = _imagePath; }
std::string Image::getImagePath() { return imagePath; }

void Image::outputImage() {

  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(imagePath, std::ios::binary | std::ios::out);

  this->writeHeader();
  this->writeImageVector();

  imageStream.close();
}

void Image::outputImage(std::string fileName) {

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

Header Image::outputHeader() { return this->header; }

std::vector<std::vector<Pixel>> Image::outputImageVector() {
  return this->imageVec;
}

// Operation functions!

void Image::Multiply(Image &layer2) {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
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

  this->imageVec = outputImageVector;
}

void Image::Screen(Image &layer2) {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      NormalizedPixel normalPixel_1 = this->normalizePixel(imageVec[x][y]);
      NormalizedPixel normalPixel_2 =
          this->normalizePixel(layer2.imageVec[x][y]);

      Pixel newPixel;
      newPixel.blue = (unsigned char)((
          int)((1.0 - (1.0 - normalPixel_1.blue) * (1.0 - normalPixel_2.blue)) *
                   255.0 +
               0.5f));
      newPixel.green =
          (unsigned char)((int)((1.0 - (1.0 - normalPixel_1.green) *
                                           (1.0 - normalPixel_2.green)) *
                                    255.0 +
                                0.5f));
      newPixel.red = (unsigned char)((
          int)((1.0 - (1.0 - normalPixel_1.red) * (1.0 - normalPixel_2.red)) *
                   255.0 +
               0.5f));
      newRow.push_back(newPixel);
    }
    outputImageVector.push_back(newRow);
  }

  this->imageVec = outputImageVector;
}

void Image::Subtract(Image &layer2) {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      Pixel newPixel;

      newPixel.blue = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].blue - (int)layer2.imageVec[x][y].blue));
      newPixel.green = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].green - (int)layer2.imageVec[x][y].green));
      newPixel.red = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].red - (int)layer2.imageVec[x][y].red));

      newRow.push_back(newPixel);
    }
    outputImageVector.push_back(newRow);
  }
  this->imageVec = outputImageVector;
}

void Image::Addition(Image &layer2) {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      Pixel newPixel;

      newPixel.blue = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].blue + (int)layer2.imageVec[x][y].blue));
      newPixel.green = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].green + (int)layer2.imageVec[x][y].green));
      newPixel.red = (unsigned char)(this->clamp(
          (int)this->imageVec[x][y].red + (int)layer2.imageVec[x][y].red));

      newRow.push_back(newPixel);
    }
    outputImageVector.push_back(newRow);
  }
  this->imageVec = outputImageVector;
}

void Image::Overlay(Image &layer2) {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      NormalizedPixel normalPixel_1 = this->normalizePixel(imageVec[x][y]);
      NormalizedPixel normalPixel_2 =
          this->normalizePixel(layer2.imageVec[x][y]);

      Pixel newPixel;

      newPixel.blue =
          (unsigned char)((int)(this->overlayConditional(normalPixel_1.blue,
                                                         normalPixel_2.blue) *
                                    255 +
                                0.5f));
      newPixel.green =
          (unsigned char)((int)(this->overlayConditional(normalPixel_1.green,
                                                         normalPixel_2.green) *
                                    255 +
                                0.5f));
      newPixel.red = (unsigned char)((
          int)(this->overlayConditional(normalPixel_1.red, normalPixel_2.red) *
                   255 +
               0.5f));

      newRow.push_back(newPixel);
    }
    outputImageVector.push_back(newRow);
  }
  this->imageVec = outputImageVector;
}

void Image::ChangeChannel(int amount, int channel) {
  std::vector<std::vector<Pixel>> outputImageVector;
  if (channel == 0) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue =
            (unsigned char)(this->clamp((int)imageVec[x][y].blue + amount));
        newPixel.green = imageVec[x][y].green;
        newPixel.red = imageVec[x][y].red;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 1) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green =
            (unsigned char)(this->clamp((int)imageVec[x][y].green + amount));
        newPixel.red = imageVec[x][y].red;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 2) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green = imageVec[x][y].green;
        newPixel.red =
            (unsigned char)(this->clamp((int)imageVec[x][y].red + amount));

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else {
    std::cout << "Invalid option selected!" << std::endl;
  }

  this->imageVec = outputImageVector;
}

void Image::MultChannel(int amount, int channel) {
  std::vector<std::vector<Pixel>> outputImageVector;
  if (channel == 0) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue =
            (unsigned char)(this->clamp((int)(imageVec[x][y].blue * amount)));
        newPixel.green = imageVec[x][y].green;
        newPixel.red = imageVec[x][y].red;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 1) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green =
            (unsigned char)(this->clamp((int)(imageVec[x][y].green * amount)));
        newPixel.red = imageVec[x][y].red;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 2) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green = imageVec[x][y].green;
        newPixel.red =
            (unsigned char)(this->clamp((int)(imageVec[x][y].red * amount)));

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else {
    std::cout << "Invalid option selected!" << std::endl;
  }
  this->imageVec = outputImageVector;
}

void Image::ExtractChannel(int channel) {
  std::vector<std::vector<Pixel>> outputImageVector;
  if (channel == 0) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green = imageVec[x][y].blue;
        newPixel.red = imageVec[x][y].blue;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 1) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].green;
        newPixel.green = imageVec[x][y].green;
        newPixel.red = imageVec[x][y].green;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 2) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].red;
        newPixel.green = imageVec[x][y].red;
        newPixel.red = imageVec[x][y].red;
        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else {
    std::cout << "Invalid option selected!" << std::endl;
  }

  this->imageVec = outputImageVector;
}

void Image::IsolateChannel(int channel) {
  std::vector<std::vector<Pixel>> outputImageVector;
  if (channel == 0) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = imageVec[x][y].blue;
        newPixel.green = 0;
        newPixel.red = 0;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 1) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = 0;
        newPixel.green = imageVec[x][y].green;
        newPixel.red = 0;

        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else if (channel == 2) {
    for (int x = 0; x < header.height; x++) {
      std::vector<Pixel> newRow;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        newPixel.blue = 0;
        newPixel.green = 0;
        newPixel.red = imageVec[x][y].red;
        newRow.push_back(newPixel);
      }

      outputImageVector.push_back(newRow);
    }
  } else {
    std::cout << "Invalid option selected!" << std::endl;
  }

  this->imageVec = outputImageVector;
}

void Image::Rotate() {
  std::vector<std::vector<Pixel>> outputImageVector;

  for (int x = 0; x < header.height; x++) {
    std::vector<Pixel> newRow;
    for (int y = 0; y < header.width; y++) {
      Pixel newPixel;
      newPixel.blue = imageVec[x][y].blue;
      newPixel.green = imageVec[x][y].green;
      newPixel.red = imageVec[x][y].red;
      newRow.push_back(newPixel);
    }

    std::reverse(newRow.begin(), newRow.end());

    outputImageVector.push_back(newRow);
  }

  std::reverse(outputImageVector.begin(), outputImageVector.end());

  this->imageVec = outputImageVector;
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
  if (imageStream.is_open()) {
    imageStream.close();
  }

  this->imageVec.clear();
  Header emptyheader;
  this->header = emptyheader;

  imageStream.open(imagePath, std::ios::binary | std::ios::in | std::ios::out);
  this->readHeader();
  this->readImageVector();
  imageStream.close();
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

float Image::overlayConditional(float p1, float p2) {
  if (p2 <= 0.5) {
    return 2 * p1 * p2;
  } else {
    return 1 - (2 * (1 - p1) * (1 - p2));
  }
}
