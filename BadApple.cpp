// BadApple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>        
#include <fstream>
#include <cstdlib>

#include "Constants.h"

void asmPhoto                  (std::string path);
void saveAsmCode               (std::string& asmCode, const int index, const std::string& pattern);
void loadAndSavePhotoCollection(std::string& asmCode, const int photoStartIndex, const int photoFinishIndex, const std::string& pattern, const std::string& extention);
void decodePhoto               (std::string& asmCode, const unsigned int width, const unsigned int height, const sf::Image& image);
void decodeWithCompressionPhoto(std::string& asmCode, const unsigned int width, const unsigned int height, const sf::Image& image);

int main()
{
    std::cout << "Bad apple started assembling!\n";

    asmPhoto("Photos/Bad_apple3/");

    std::cout << "Compiled successfully\n";
}

void asmPhoto(std::string path)
{
    const int step = 300;

    const std::string extention   = ".jpg";

    const int stepStart = 6;

    const int frameLen = 6572;

    int start = 1 + step * (stepStart - 1);
    int finish = start + step - 1;

    for (int stepNum = stepStart; start <= frameLen; stepNum++)
    {
        std::string asmCode{};

        loadAndSavePhotoCollection(asmCode, start, finish, path, extention);

        saveAsmCode(asmCode, stepNum, destAsmSavePath);

        std::cout << "-----------\n";
        std::cout << "Bad apple part " + std::to_string(stepNum) + "/" + std::to_string(cSteps) +" asm file created successfully\n";

        system(("C:/Users/Алехандро/Desktop/AlexProjects/MyVirtualMachine/x64/Release/TextToNumConverter.exe " +
            destAsmSavePath + std::to_string(stepNum) + ".asm "
            "false").c_str());

        std::cout << "Bad apple part " + std::to_string(stepNum) + "/" + std::to_string(cSteps) + " compiled successfully\n";

        start += step;
        finish += step;
    }
}

void loadAndSavePhotoCollection(std::string& asmCode, const int photoStartIndex, const int photoFinishIndex, const std::string& pattern, const std::string& extention)
{
    for (int i = photoStartIndex; i <= photoFinishIndex; i++)
    {
        char buffer[6];
        sprintf_s(buffer, sizeof(buffer), "%05d", i);
        std::string istr = buffer;

        std::string fullPath{};

        fullPath = pattern + istr + extention;

        sf::Image image;

        if (!image.loadFromFile(fullPath))
        {
            break;
        }

        const unsigned int width = image.getSize().x;
        const unsigned int height = image.getSize().y;

        decodeWithCompressionPhoto(asmCode, width, height, image);
    }

    asmCode += "\nhlt\n";
}

void decodePhoto(std::string& asmCode, const unsigned int width, const unsigned int height, const sf::Image& image)
{
    for (unsigned int x = 0; x < width; x++)
    {
        for (unsigned int y = 0; y < height; y++)
        {
            sf::Color pixelColor = image.getPixel(x, y);

            asmCode += "push " + std::to_string(x) + "\n";
            asmCode += "push " + std::to_string(y) + "\n";

            asmCode += "push " + std::to_string(pixelColor.r) + "\n";
            asmCode += "push " + std::to_string(pixelColor.g) + "\n";
            asmCode += "push " + std::to_string(pixelColor.b) + "\n";

            asmCode += "setpxl\n";
        }
    }

    asmCode += "push 1\n";
    asmCode += "wait\n";
}

void saveColorLine(const unsigned int start, const unsigned int finish, const unsigned int y, const sf::Color color, std::string& asmCode)
{
    const int r = color.r;
    const int g = color.g;
    const int b = color.b;

    const std::string rt = std::to_string(r);
    const std::string gt = std::to_string(g);
    const std::string bt = std::to_string(b);

    asmCode += "push " + std::to_string(start) + "\n";
    asmCode += "pop ax\n";


    static int lastCycleNum = 0;
    const std::string circleName = "cycle_" + std::to_string(lastCycleNum++);

    asmCode += circleName + ":\n";

    asmCode += "push ax\n";
    asmCode += "push " + std::to_string(y) + "\n";
    asmCode += "push " + rt + "\n";
    asmCode += "push " + gt + "\n";
    asmCode += "push " + bt + "\n";

    asmCode += "setpxl\n";

    asmCode += "push ax\n";
    asmCode += "push 1 \n";
    asmCode += "add    \n";
    asmCode += "pop ax \n";
    asmCode += "push ax\n";
    asmCode += "push " + std::to_string(finish) + "\n";
    asmCode += "jbe " + circleName + "\n";
}

void decodeWithCompressionPhoto(std::string& asmCode, const unsigned int width, const unsigned int height, const sf::Image& image)
{
    for (unsigned int y = 0; y < height; y++)
    {
        
        int colorXStart = 0;
        sf::Color lastColor = image.getPixel(colorXStart, y);

        for (unsigned int x = 1; x < width; x++)
        {
            sf::Color pixelColor = image.getPixel(x, y);

            if (pixelColor != lastColor)
            {
                saveColorLine(colorXStart, x - 1, y, lastColor, asmCode);

                colorXStart = x;
                lastColor = pixelColor;
            }
        }

        saveColorLine(colorXStart, width - 1, y, lastColor, asmCode);
    }

    asmCode += "push 20\n";
    asmCode += "wait\n";
}

void saveAsmCode(std::string& asmCode, const int index, const std::string& pattern)
{
    std::ofstream file(pattern + std::to_string(index) + ".asm");

    if (file.is_open())
    {
        file << asmCode;
        file.close();
    }
}
