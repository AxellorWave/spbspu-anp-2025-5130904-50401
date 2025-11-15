#include <iostream>

namespace studilova {

     struct fixed_matrix_t {
        int data[100][100];
        int rows;
        int cols;
    };

    struct dynamic_matrix_t {
        int** data[100][100];
        int rows;
        int cols;
    };

    bool read_matrix(const char* filename, fixed_matrix_t* fm, dynamic_matrix_t** dm_ptr, int mode) {
        FILE* f = std::fopen(filename, "r");
        if (!f) {
            return false;
        }
        std::fclose(f);
        return true;
    }

    int main_logic(int argc, char* argv[]) {
        return 0;
    }
}

int main(int argc, char* argv[]) {
    return studilova::main_logic(argc, argv);
}
