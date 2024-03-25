#include "Image.h"
#include "Pixel.h"
#include "TempImage.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

using std::cout;
using std::endl;

bool validFile(std::string filename);
bool validOutputFile(std::string filename);
bool isFileMode(const std::string &filename);
bool isNumberMode(const std::string &filename);
bool isNoneMode(const std::string &filename);

int main(int argc, char *argv[]) {

  // No args response
  if (argc <= 1) {
    std::cout << "Project 2: Image Processing, Spring "
                 "2024\n\nUsage:\n\t./project2.out [output] [firstImage] "
                 "[method] [...]"
              << std::endl;
    return 0;
  }

  // Loading up all arguments into a vector

  std::vector<std::string> CLIargs;

  for (int i = 1; i < argc; i++) {
    std::string newArg = argv[i];
    CLIargs.push_back(newArg);
  }

  // Checking for --help flag

  if (CLIargs[0] == "--help") {
    if (CLIargs.size() == 1) {
      cout << "Project 2: Image Processing, Spring "
              "2024\n\nUsage:\n\t./project2.out [output] [firstImage] "
              "[method] [...]"
           << endl;
    } else {
      cout << "Extra arguments after --help" << endl;
    }
    return 0;
  }

  std::string outputFilename = CLIargs[0];

  // Test if output file is valid
  if (!validOutputFile(outputFilename)) { // NOT
    return 0;
  }

  // Test size of CLIargs to see if there is anythiing after output file

  if (CLIargs.size() <= 1) {
    cout << "No tracking file specified" << endl;
    return 0;
  }

  // POSSIBLY CHECK IF THERE IS AN ARGUMENT AFTER OUTPUT FILE?

  // Check if base image is valid
  std::string trackingFilename = CLIargs[1];

  if (!validFile(trackingFilename)) {
    return 0;
  }

  Image trackingImage(trackingFilename);

  // Variables for loop
  int stage = 0;               // 0 - func, 1 - file, 2 -number
  std::string fileLoaded = ""; // For combine function
  std::string loadedFunction = "";

  for (int x = 2; x < CLIargs.size(); x++) {

    // Stage 0 - Finding function

    if (stage == 0) {
      if (isFileMode(CLIargs[x])) {
        stage = 1;
        loadedFunction = CLIargs[x];
      } else if (isNumberMode(CLIargs[x])) {
        stage = 2;
        loadedFunction = CLIargs[x];
      } else if (isNoneMode(CLIargs[x])) {
        loadedFunction = CLIargs[x];
        if (loadedFunction == "flip") {
          cout << "... Flipping ..." << endl;
          trackingImage.Rotate();
          loadedFunction = "";
          stage = 0;
        } else if (loadedFunction == "onlyred") {
          trackingImage.ExtractChannel(2);
          loadedFunction = "";
          stage = 0;
        } else if (loadedFunction == "onlygreen") {
          trackingImage.ExtractChannel(1);
          loadedFunction = "";
          stage = 0;
        } else if (loadedFunction == "onlyblue") {
          trackingImage.ExtractChannel(0);
          loadedFunction = "";
          stage = 0;
        } else {
          cout << "Something wrong with non type method" << endl;
          return 0;
        }
      } else {
        cout << "Invalid method name." << endl;
        return 0;
      }
    } else if (stage == 1) { // Stage 1 Implementation (File based)
      // Check if file is valid
      if (!validFile(CLIargs[x])) {
        return 0;
      }

      Image layer2(CLIargs[x]);

      if (loadedFunction == "multiply") {
        trackingImage.Multiply(layer2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "subtract") {
        trackingImage.Subtract(layer2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "overlay") {
        trackingImage.Overlay(layer2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "screen") {
        trackingImage.Screen(layer2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "combine") {

        // Check if this is the second file
        if (fileLoaded == "") {
          fileLoaded = CLIargs[x];
        } else {
          Image layer3(fileLoaded);
          trackingImage.IsolateChannel(2);
          layer2.IsolateChannel(0);
          layer3.IsolateChannel(1);
          trackingImage.Addition(layer2);
          trackingImage.Addition(layer3);
          fileLoaded = "";
          loadedFunction = "";
          stage = 0;
        }
        // -------------------
      }
    } else if (stage == 2) {

      int value;
      // Attempt to convert to number

      try {
        value = std::stoi(CLIargs[x]);
      } catch (std::invalid_argument) {
        cout << "Invalid argument, expected number." << endl;
        return 0;
      }

      if (loadedFunction == "addred") {
        trackingImage.ChangeChannel(value, 2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "addgreen") {
        trackingImage.ChangeChannel(value, 1);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "addblue") {
        trackingImage.ChangeChannel(value, 0);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "scalered") {
        trackingImage.MultChannel(value, 2);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "scalegreen") {
        trackingImage.MultChannel(value, 1);
        loadedFunction = "";
        stage = 0;
      } else if (loadedFunction == "scaleblue") {
        trackingImage.MultChannel(value, 0);
        loadedFunction = "";
        stage = 0;
      } else {
        cout << "There was an error with finding the correct method name"
             << endl;
        return 0;
      }

    } else {
      cout << "Actually how" << endl;
      return 0;
    }
  }

  // Outputting image or throwing error

  if (loadedFunction != "") {
    cout << "Argument: " << loadedFunction << " is missing an argument" << endl;
    cout << "Missing argument." << endl;
  } else {
    trackingImage.outputImage(outputFilename);
  }

  return 0;
}

// Some helper methods

bool validOutputFile(std::string filename) {
  size_t lastPOS = filename.find_last_of('.');
  if (lastPOS == std::string::npos) {
    cout << "Invalid file name." << endl;
    return false;
  } else if (filename.substr(lastPOS + 1) != "tga") {
    cout << "Invalid file name." << endl;
    return false;
  }

  std::fstream testStream(filename, std::ios::binary | std::ios::out);

  if (testStream.is_open()) {
    return true;
  } else {
    cout << "File does not exist" << endl;
    return false;
  }
}

bool validFile(std::string filename) {
  size_t lastPOS = filename.find_last_of('.');
  if (lastPOS == std::string::npos) {
    cout << "Invalid file name." << endl;
    return false;
  } else if (filename.substr(lastPOS + 1) != "tga") {
    cout << "Invalid file name." << endl;
    return false;
  }

  std::ifstream testStream(filename, std::ios::binary);

  if (testStream.is_open()) {
    return true;
  } else {
    cout << "Invalid argument, file does not exist." << endl;
    return false;
  }
}

bool isFileMode(const std::string &function) {
  return function == "multiply" || function == "subtract" ||
         function == "overlay" || function == "screen" || function == "combine";
}

bool isNumberMode(const std::string &function) {

  return function == "addred" || function == "addgreen" ||
         function == "addblue" || function == "scalered" ||
         function == "scalegreen" || function == "scaleblue";
}

bool isNoneMode(const std::string &function) {
  return function == "flip" || function == "onlyred" ||
         function == "onlygreen" || function == "onlyblue";
}
