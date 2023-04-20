#include "corevim.hpp"

int main(int argc, char **argv){
    if (argc > 1){
        auto corevim = std::make_shared<CoreVim>(argv[1]);
        corevim->run();
    }else{
        auto corevim = std::make_shared<CoreVim>("");
        corevim->run();
    }
    return 0;
}
