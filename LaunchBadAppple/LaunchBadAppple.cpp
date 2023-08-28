#include <iostream>
#include <string>

#include "../Constants.h"


void launchFilm();

int main()
{
    launchFilm();

    return 0;
}

void launchFilm()
{
    for (int stepNum = 1; stepNum <= cSteps; stepNum++)
    {
        std::cout << "Launching " << stepNum << ":\n";
        system(("C:/Users/Алехандро/Desktop/AlexProjects/MyVirtualMachine/x64/Release/MyVirtualMachine.exe "
            "C:/Users/Алехандро/Desktop/AlexProjects/BadApple/Code/202p/asmCode_" + std::to_string(stepNum) + ".bin ").c_str());
    }
}

