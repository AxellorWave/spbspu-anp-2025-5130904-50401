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

     void free_dynamic_matrix(dynamic_matrix_t* dm) {
        if (!dm) {
            return;
        }
        for (int i = 0; i < dm->rows; ++i) {
            std::free(dm->data[i]);
        }
        std::free(dm->data);
        std::free(dm);
    }

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
