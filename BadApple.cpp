// BadApple.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>        
#include <fstream>

void asmPhoto(std::string path);

int main()
{
    std::cout << "Bad apple started assembling!\n";

    asmPhoto("Bad_apple3/");
}

void asmPhoto(std::string path)
{
    const int photoLen = 410;

    std::string pattern   = path;
    std::string extention = ".png";

    std::string fullPath{};

    std::string asmCode{};

    for(int i = 1; i <= photoLen; i++)
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

        for (unsigned int x = 0; x < width; x++)
        {
            for (unsigned int y = 0; y < height; y++)
            {
                sf::Color pixelColor = image.getPixel(x, y);

                asmCode += "push " + std::to_string(x)            + "\n";
                asmCode += "push " + std::to_string(y)            + "\n";

                asmCode += "push " + std::to_string(pixelColor.r) + "\n";
                asmCode += "push " + std::to_string(pixelColor.g) + "\n";
                asmCode += "push " + std::to_string(pixelColor.b) + "\n";

                asmCode += "setpxl\n";
            }
        }

        asmCode += "push 1\n";
        asmCode += "wait\n";
    }

    asmCode += "\nhlt\n";

    saveAsmCode(asmCode);
}

void saveAsmCode(std::string& asmCode)
{
    std::ofstream file("asmCode.asm");

    if (file.is_open())
    {
        file << asmCode;
        file.close();
    }
}
