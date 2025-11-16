#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>

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

    int count_saddle_points(const fixed_matrix_t* m) {
        if (!m || m->rows <= 0 || m->cols <= 0) {
            return 0;
        }
        int count = 0;
        for (int i = 0; i < m->rows; ++i) {
            for (int j = 0; j < m->cols; ++j) {
                int current_val = m->data[i][j];
                bool is_min_in_row = true;
                for (int col = 0; col < m->cols; ++col) {
                    if (m->data[i][col] < current_val) {
                        is_min_in_row = false;
                        break;
                    }
                }
                if (!is_min_in_row) {
                    continue;
                }
                bool is_max_in_col = true;
                for (int row = 0; row < m->rows; ++row) {
                    if (m->data[row][j] > current_val) {
                        is_max_in_col = false;
                        break;
                    }
                }
                if (is_max_in_col) {
                    count++;
                }
            }
        }
        return count;
    }

    int max_sum_diagonals(const fixed_matrix_t* m) {
        if (!m) {
            return 0;
        }
        if (m->rows <= 0 || m->cols <= 0) {
            return 0;
        }
        int max_sum = INT_MIN;
        for (int k = -(m->cols - 1); k < m->rows; ++k) {
            int sum = 0;
            bool has_elements = false;
            for (int i = 0; i < m->rows; ++i) {
                int j = i - k;
                if (j >= 0 && j < m->cols) {
                    sum += m->data[i][j];
                    has_elements = true;
                }
            }
            if (has_elements && sum > max_sum) {
                max_sum = sum;
            }
        }
        return (max_sum == INT_MIN) ? 0 : max_sum;
    }

    int main_logic(int argc, char* argv[]) {
        if (argc != 4) {
            std::fprintf(stderr, "Not enough arguments\n");
            return 1;
        }

        const char* num_str = argv[1];
        char* endptr_num = nullptr;
        long num_long = std::strtol(num_str, &endptr_num, 10);
        if (endptr_num == num_str || *endptr_num != '\0') {
            std::fprintf(stderr, "First parameter is not a number\n");
            return 1;
        }
        if (num_long != 1 && num_long != 2) {
            std::fprintf(stderr, "First parameter is out of range\n");
            return 1;
        }

        int num = static_cast<int>(num_long);

        const char* input_filename = argv[2];
        const char* output_filename = argv[3];

        fixed_matrix_t fixed_matrix = {0};
        dynamic_matrix_t* dynamic_matrix = nullptr;

        if (!read_matrix(input_filename, &fixed_matrix, &dynamic_matrix, num)) {
            std::fprintf(stderr, "Error reading matrix from file\n");
            return 1;
        }

        int result = 0;
        if (num == 1) {
            result = max_sum_diagonals(&fixed_matrix);
        } else {
            fixed_matrix_t temp_matrix = {0};
            temp_matrix.rows = dynamic_matrix->rows;
            temp_matrix.cols = dynamic_matrix->cols;

            for (int i = 0; i < dynamic_matrix->rows; ++i) {
                for (int j = 0; j < dynamic_matrix->cols; ++j) {
                    temp_matrix.data[i][j] = (int)dynamic_matrix->data[i][j];
                }
            }

            result = count_saddle_points(&temp_matrix);
            free_dynamic_matrix(dynamic_matrix);
        }
        return 0;
    }
}

int main(int argc, char* argv[]) {
    return studilova::main_logic(argc, argv);
}
