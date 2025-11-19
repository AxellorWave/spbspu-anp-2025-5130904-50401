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
        int** data;
        int rows;
        int cols;
    };

    void free_dynamic_matrix(dynamic_matrix_t* dm) {
        if (!dm) {
            return;
        }
        if (dm->data) {
            for (int i = 0; i < dm->rows; ++i) {
                std::free(dm->data[i]);
            }
            std::free(dm->data);
        }
        std::free(dm);
    }

bool read_matrix(const char* filename, fixed_matrix_t* fm, dynamic_matrix_t** dm_ptr, int mode) {
        FILE* f = std::fopen(filename, "r");
        if (!f) {
            return false;
        }
        
        int temp_data[100][100] = {{0}};
        int rows = 0;
        int cols = 0;
        bool first_line = true;

        while (rows < 100) {
            int row_vals[100];
            int col_count = 0;
             while (col_count < 100) {
                int value;
                if (fscanf(f, "%d", &value) != 1) {
                    break;
                }
                row_vals[col_count++] = value;
            }

            if (col_count == 0) break;

            if (first_line) {
                cols = col_count;
                first_line = false;
            } else if (col_count != cols) {
                std::fclose(f);
                return false;
            }

            for (int j = 0; j < cols; ++j) {
                temp_data[rows][j] = row_vals[j];
            }
            rows++;
        }
        std::fclose(f);

        if (rows == 0 || cols == 0) return false;

        if (mode == 1) {
            if (rows > 100 || cols > 100) return false;
            
            fm->rows = rows;
            fm->cols = cols;
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    fm->data[i][j] = temp_data[i][j];
                }
            }
        } 
        else if (mode == 2) {
            dynamic_matrix_t* dm = static_cast<dynamic_matrix_t*>(std::malloc(sizeof(dynamic_matrix_t)));
            if (!dm) return false;
            
            dm->rows = rows;
            dm->cols = cols;
            dm->data = static_cast<int**>(std::malloc(rows * sizeof(int*)));
            
            if (!dm->data) {
                std::free(dm);
                return false;
            }

            for (int i = 0; i < rows; ++i) {
                dm->data[i] = static_cast<int*>(std::malloc(cols * sizeof(int)));
                if (!dm->data[i]) {
                    for (int j = 0; j < i; ++j) {
                        std::free(dm->data[j]);
                    }
                    std::free(dm->data);
                    std::free(dm);
                    return false;
                }
                
                for (int j = 0; j < cols; ++j) {
                    dm->data[i][j] = temp_data[i][j];
                }
            }
            
            *dm_ptr = dm;
        }
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

        char* endptr;
        long num = std::strtol(argv[1], &endptr, 10);
        
        if (*endptr != '\0' || (num != 1 && num != 2)) {
            std::fprintf(stderr, "First parameter is invalid\n");
            return 1;
        }

        fixed_matrix_t fixed_matrix = {0};
        dynamic_matrix_t* dynamic_matrix = nullptr;

        if (!read_matrix(argv[2], &fixed_matrix, &dynamic_matrix, num)) {
            std::fprintf(stderr, "Error reading matrix from file\n");
            return 2;
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

        FILE* output_file = std::fopen(argv[3], "w");
        if (!output_file) {
            std::fprintf(stderr, "Cannot open output file\n");
            return 1;
        }

        std::fprintf(output_file, "%d\n", result);
        std::fclose(output_file);
        return 0;
    }
}

int main(int argc, char* argv[]) {
    return studilova::main_logic(argc, argv);
}
