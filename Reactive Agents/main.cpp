#include "agent.h"

int main(int argc, char* argv[]) {
    std::cout << "---Program running---" << std::endl;

    std::cout << "==================================" << std::endl;

    std::cout << "Random samples are placed all over the grid" << std::endl;
    std::cout << "The agents' goal is to take all samples 1 by 1" << std::endl;
    std::cout << "And place them into a ship" << std::endl;

    std::cout << "==================================" << std::endl;
    
    std::cout << std::endl <<"Controls:" << std::endl;
    std::cout << std::endl << "Press space bar to start the agents (and toggle)" << std::endl;
    std::cout <<"Left click for placing an obstacle" << std::endl;
    std::cout << "Right click for deleting an obstacle" << std::endl;
    std::cout << std::endl << "Press escape to close the program ..." << std::endl;

    runProgram();
    return 0;
}