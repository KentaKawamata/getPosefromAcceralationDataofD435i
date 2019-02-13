#include <iostream>
#include <sstream>
#include "convert.hpp"

int main(int argc, char *argv[]){

    if(argc != 2){
        std::cout << "Input Error" << std::endl;
        std::cout << "Please Input filename" << std::endl;
        std::cout << "Exanple : ./imuTest ~/filePath/filename" << std::endl;
    }

    std::string filename(argv[1]);

    try {

        ConvertPCL convertPCL(filename);
        convertPCL.run();

    } catch (std::exception &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}