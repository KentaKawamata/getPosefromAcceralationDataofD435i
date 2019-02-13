#include <iostream>
#include <sstream>
#include "convert.hpp"

int main(int argc, char *argv[]){

    if(argc != 5){
        std::cout << "Input Error" << std::endl;
        std::cout << "Please Input filename" << std::endl;
        std::cout << "Exanple : ./imuTest ~/filePath/filename" << std::endl;
        return -1;
    }

    std::string filename(argv[1]);
    float X = std::stof(argv[2]);
    float Y = std::stof(argv[3]);
    float Z = std::stof(argv[4]);

    try {

        ConvertPCL convertPCL(filename, X, Y, Z);
        convertPCL.run();

    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}
