#include <iostream>
#include <fstream>





namespace levin {
    enum class MemoryType : int {
        STATIC_MEMORY = 1,
        DYNAMIC_MEMORY
    };

    struct Memory {
        int* buffer;  

        void createBuffer() {}
        void clearBuffer() {}
        int* LFT_BOT_CNT(); 
    }; 
} 

int main (int argc, char ** argv) {
    if (argc != 4 || !(argv[1][0] == '1' || argv[1][0] == '2')) {
        std::cout << "wrong arg input\n";
        return 1;
    }
    
    std::ifstream input(argv[2]);
    std::ofstream ouput(argv[3]);
}