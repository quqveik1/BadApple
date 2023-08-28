// BadApple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>        
#include <fstream>

void asmPhoto(std::string path);
void saveAsmCode(std::string& asmCode);
void doPrimitivePhotoSave(std::string& asmCode, int photoLen, std::string& fullPath, const std::string& pattern, const std::string& extention);
void decodePhoto               (std::string& asmCode, const unsigned int width, const unsigned int height, sf::Image& image);
void decodeWithCompressionPhoto(std::string& asmCode, const unsigned int width, const unsigned int height, sf::Image& image);

int main()
{
    std::cout << "Bad apple started assembling!\n";

    asmPhoto("Photos/Bad_apple3/");

    std::cout << "Bad apple asm file created successfully\n";
}

void asmPhoto(std::string path)
{
    const int photoLen = 400;

    std::string pattern   = path;
    std::string extention = ".png";

    std::string fullPath{};

    std::string asmCode{};

    doPrimitivePhotoSave(asmCode, photoLen, fullPath, pattern, extention);

    saveAsmCode(asmCode);
}

void doPrimitivePhotoSave(std::string& asmCode, int photoLen, std::string& fullPath, const std::string& pattern, const std::string& extention)
{

    for (int i = 1; i <= photoLen; i++)
    {
        char buffer[6];
        sprintf_s(buffer, sizeof(buffer), "%05d", i);
        std::string istr = buffer;

        fullPath = pattern + istr + extention;

        sf::Image image;

        if (!image.loadFromFile(fullPath))
        {
            return;
        }

        unsigned int width = image.getSize().x;
        unsigned int height = image.getSize().y;

        decodeWithCompressionPhoto(asmCode, width, height, image);
    }

    asmCode += "\nhlt\n";
}

void decodePhoto(std::string& asmCode, const unsigned int width, const unsigned int height, sf::Image& image)
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

int lastCycleNum = 0;

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

void decodeWithCompressionPhoto(std::string& asmCode, const unsigned int width, const unsigned int height, sf::Image& image)
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

    asmCode += "push 1\n";
    asmCode += "wait\n";
}


void saveAsmCode(std::string& asmCode)
{
    std::ofstream file("Code/asmCode1.asm");

    if (file.is_open())
    {
        file << asmCode;
        file.close();
    }
}
