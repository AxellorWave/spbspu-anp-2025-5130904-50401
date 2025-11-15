#include <iostream>

namespace studilova {
    int main_impl(int argc, char* argv[]) {
        return 0;
    }
}

int main(int argc, char* argv[]) {
    return studilova::main_impl(argc, argv);
}
