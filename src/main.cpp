#include "Image.h"
#include "Pixel.h"
#include "TempImage.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

Image createImage(TempImage tempimage) {
  Image temp;
  temp.importImageVector(tempimage.imgVec);
  temp.importHeader(tempimage.header);
  return temp;
}

int main(int argc, char *argv[]) {
  std::cout << "There are " << argc << " arguements entered\n";

  // No args response
  if (argc <= 1) {
    std::cout << "Project 2: Image Processing, Spring "
                 "2024\n\nUsage:\n\t./project2.out [output] [firstImage] "
                 "[method] [...]"
              << std::endl;
    return 0;
  }

  std::vector<std::string> CLIargs;
  std::string outputPath = "";
  Image baseLayer;

  for (int x = 1; x < argc; x++) {
    std::string newArg = argv[x];
    CLIargs.push_back(newArg);
  }

  if (CLIargs[0] == "--help" && CLIargs.size() == 1) {
    std::cout << "Project 2: Image Processing, Spring "
                 "2024\n\nUsage:\n\t./project2.out [output] [firstImage] "
                 "[method] [...]"
              << std::endl;
    return 0;
  } else if (CLIargs[0] == "--help" && CLIargs.size() != 1) {
    std::cout << "Extra arguemnts for help!" << std::endl;
  }

  if (CLIargs.size() == 1) {
    std::cout << "Invalid file name." << std::endl;
    return 0;
  } else {
    outputPath = CLIargs[0];
  }

  std::cout << "Loading image from " << CLIargs[1] << std::endl;

  baseLayer.setImagePath(CLIargs[1]);
  baseLayer.loadImage();

  std::string currentFunction = "";
  int num_factor = 0;
  Image nextLayer;
  Image nextnextLayer;

  TempImage tempimage;
  tempimage.header = baseLayer.outputHeader();
  tempimage.imgVec = baseLayer.outputImageVector();

  for (int i = 2; i < CLIargs.size(); i++) {
    if (currentFunction == "") {
      if (CLIargs[i] == "flip") {
        Image temp = createImage(tempimage);
        Image result = temp.Rotate();
        tempimage.header = result.outputHeader();
        tempimage.imgVec = result.outputImageVector();
      } else {
        currentFunction = CLIargs[i];
      }
    } else {
      if (currentFunction == "addred" || currentFunction == "addgreen" ||
          currentFunction == "addblue" || currentFunction == "scalered" ||
          currentFunction == "scalegreen" || currentFunction == "scaleblue") {

        try {
          num_factor = std::stoi(CLIargs[i]);

        } catch (std::invalid_argument) {
          std::cout << "Please use a number when using scale or add!"
                    << std::endl;
          return 0;
        }
      } else {

        if (currentFunction == "combine" && nextLayer.getImagePath() == "") {
          nextLayer.setImagePath(CLIargs[i]);
          nextLayer.loadImage();
          continue;
        } else if (currentFunction == "combine" &&
                   nextnextLayer.getImagePath() != "") {
          nextnextLayer.setImagePath(CLIargs[i]);
          nextnextLayer.loadImage();
        } else {
          nextLayer.setImagePath(CLIargs[i]);
          nextLayer.loadImage();
        }

        if (currentFunction == "multiply") {
          std::cout << "Multiplying..." << std::endl;
          Image result = createImage(tempimage).Multiply(nextLayer);
          tempimage.header = result.outputHeader();
          tempimage.imgVec = result.outputImageVector();
        } else if (currentFunction == "subtract") {
          std::cout << "Subtracting..." << std::endl;
          Image result = createImage(tempimage).Subtract(nextLayer);
          tempimage.header = result.outputHeader();
          tempimage.imgVec = result.outputImageVector();
        } else if (currentFunction == "overlay") {
          std::cout << "Overlaying..." << std::endl;
          Image result = createImage(tempimage).Overlay(nextLayer);
          tempimage.header = result.outputHeader();
          tempimage.imgVec = result.outputImageVector();
        } else if (currentFunction == "screen") {
          std::cout << "Screening..." << std::endl;
          Image result = createImage(tempimage).Screen(nextLayer);
          tempimage.header = result.outputHeader();
          tempimage.imgVec = result.outputImageVector();
        } else if (currentFunction == "combine") {
          Image result =
              createImage(tempimage).Addition(nextLayer).Addition(nextLayer);
          tempimage.header = result.outputHeader();
          tempimage.imgVec = result.outputImageVector();
        }
        currentFunction = "";
      }
    }

    Image outputImage;
    outputImage.importImageVector(tempimage.imgVec);
    outputImage.importHeader(tempimage.header);
    outputImage.outputImage(outputPath);

    // Image car("./input/car.tga");
    // Image circles("./input/circles.tga");
    // Image layer1("./input/layer1.tga");
    // Image layer2("./input/layer2.tga");
    // Image layer_blue("./input/layer_blue.tga");
    // Image layer_green("./input/layer_green.tga");
    // Image layer_red("./input/layer_red.tga");
    // Image pattern1("./input/pattern1.tga");
    // Image pattern2("./input/pattern2.tga");
    // Image text("./input/text.tga");
    // Image text2("./input/text2.tga");

    // // Part 1
    // Image part1 = layer1.Multiply(pattern1);
    // part1.outputImage("./output/part1.tga");

    // // Part 2
    // Image part2 = car.Subtract(layer2);
    // part2.outputImage("./output/part2.tga");

    // // Part 3
    // Image part3 = layer1.Multiply(pattern2).Screen(text);
    // part3.outputImage("./output/part3.tga");

    // // Part 4
    // Image part4 = layer2.Multiply(circles).Subtract(pattern2);
    // part4.outputImage("./output/part4.tga");

    // // Part 5
    // Image part5 = layer1.Overlay(pattern1);
    // part5.outputImage("./output/part5.tga");

    // // Part 6
    // Image part6 = car.ChangeChannel(200, 1);
    // part6.outputImage("./output/part6.tga");

    // // Part 7
    // Image part7 = car.MultChannel(4, 2).MultChannel(0, 0);
    // part7.outputImage("./output/part7.tga");

    // // Part 8 B
    // Image part8_b = car.ExtractChannel(0);
    // part8_b.outputImage("./output/part8_b.tga");

    // // Part 8 G
    // Image part8_g = car.ExtractChannel(1);
    // part8_g.outputImage("./output/part8_g.tga");

    // // Part 8 R
    // Image part8_r = car.ExtractChannel(2);
    // part8_r.outputImage("./output/part8_r.tga");

    // // Part 9
    // Image isolated_b = layer_blue.IsolateChannel(0);
    // Image isolated_g = layer_green.IsolateChannel(1);
    // Image isolated_r = layer_red.IsolateChannel(2);

    // Image part9 = isolated_b.Addition(isolated_g).Addition(isolated_r);
    // part9.outputImage("./output/part9.tga");

    // // Part 10
    // Image part10 = text2.Rotate();
    // part10.outputImage("./output/part10.tga");

    return 0;
  }
}
