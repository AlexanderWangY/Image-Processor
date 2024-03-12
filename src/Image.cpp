#include "Image.h"
#include "Pixel.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <vector>

Image::Image(){};

Image::Image(std::string _imagePath) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imagePath = _imagePath;

  imageStream.open(imagePath, std::ios::binary | std::ios::in | std::ios::out);
}

void Image::loadImage() {
  if (imageStream.is_open()) {
    this->loadHeader();

    for (int x = 0; x < header.height; x++) {

      std::vector<Pixel> newVector;
      for (int y = 0; y < header.width; y++) {
        Pixel newPixel;
        imageStream.read(reinterpret_cast<char *>(&newPixel.blue),
                         sizeof(unsigned char));
        imageStream.read(reinterpret_cast<char *>(&newPixel.green),
                         sizeof(unsigned char));
        imageStream.read(reinterpret_cast<char *>(&newPixel.red),
                         sizeof(unsigned char));

        newVector.push_back(newPixel);
      }

      imageVec.push_back(newVector);
    }

    std::cout << "Loaded all the pixels...\n";

    imageStream.close();
  } else {
    std::cout << "No file path is loaded!" << std::endl;
  }
}

void Image::loadHeader() {
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

void Image::writeImage(std::string fileName) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(fileName, std::ios::binary | std::ios::out);

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

  for (std::vector<Pixel> vec : imageVec) {
    for (Pixel p : vec) {
      imageStream.write(reinterpret_cast<char *>(&p.blue),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&p.green),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&p.red),
                        sizeof(unsigned char));
    }
  }

  imageStream.close();
}

void Image::Multiply(Image &layer2, std::string outputName) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(outputName, std::ios::binary | std::ios::out);
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

  for (int x = 0; x < header.height; x++) {
    for (int y = 0; y < header.width; y++) {
      Pixel newPixel;

      float N_P1_BLUE = (int)imageVec[x][y].blue / 255.0;
      float N_P1_GREEN = (int)imageVec[x][y].green / 255.0;
      float N_P1_RED = (int)imageVec[x][y].red / 255.0;
      float N_P2_BLUE = (int)layer2.imageVec[x][y].blue / 255.0;
      float N_P2_GREEN = (int)layer2.imageVec[x][y].green / 255.0;
      float N_P2_RED = (int)layer2.imageVec[x][y].red / 255.0;

      newPixel.blue =
          (unsigned char)((int)((N_P1_BLUE * N_P2_BLUE) * 255.0 + 0.5));
      newPixel.green =
          (unsigned char)((int)((N_P1_GREEN * N_P2_GREEN) * 255.0 + 0.5));
      newPixel.red =
          (unsigned char)((int)((N_P1_RED * N_P2_RED) * 255.0 + 0.5));

      imageStream.write(reinterpret_cast<char *>(&newPixel.blue),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&newPixel.green),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&newPixel.red),
                        sizeof(unsigned char));
    }
  }

  imageStream.close();
}

void Image::Screen(Image &layer2, std::string outputName) {
  if (imageStream.is_open()) {
    imageStream.close();
  }

  imageStream.open(outputName, std::ios::binary | std::ios::out);
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

  for (int x = 0; x < header.height; x++) {
    for (int y = 0; x < header.width; y++) {
      int subBlue = (int)imageVec[x][y].blue - (int)layer2.imageVec[x][y].blue;
      if (subBlue < 0) {
        subBlue = 0;
      } else if (subBlue > 255) {
        subBlue = 255;
      }
      int subGreen = (int)imageVec[x][y].blue - (int)layer2.imageVec[x][y].blue;
      if (subGreen < 0) {
        subGreen = 0;
      } else if (subGreen > 255) {
        subGreen = 255;
      }
      int subRed = (int)imageVec[x][y].blue - (int)layer2.imageVec[x][y].blue;
      if (subRed < 0) {
        subRed = 0;
      } else if (subRed > 255) {
        subRed = 255;
      }

      subBlue = (unsigned char)subBlue;
      subGreen = (unsigned char)subGreen;
      subRed = (unsigned char)subRed;

      imageStream.write(reinterpret_cast<char *>(&subBlue),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&subGreen),
                        sizeof(unsigned char));
      imageStream.write(reinterpret_cast<char *>(&subRed),
                        sizeof(unsigned char));
    }
  }

  imageStream.close();
}
