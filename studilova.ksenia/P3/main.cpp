#include <iostream>
#include <cstdlib>

int main(int argc, char * argv[]) {
    if (argc < 4) {
        std::cerr << "Not enought arguments\n";
        return 1;
    }
    else if (argc > 4) {
        std::cerr << "Too many arguments\n";
        return 1;
    }

    else if (argv[1][0] < '0' || argv[1][0] > '9') {
    std::cerr << "First parameter is not a number\n";
        return 1;
    }
    else if (argv[1][0] != '1' && argv[1][0] != '2') {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }

    const char * num = argv[1];
    const char * input_filename = argv[2];
    const char * output_filename = argv[3];

    int mode = num[0] - '0';

    return 0;
}