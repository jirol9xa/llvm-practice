#include <iostream>
#include "parser.hpp"

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Error: missing file name!\n";
        return -1;
    }

    Parser psr(argv[1], argv[2], argv[3]);

    qsort ((void *)psr.getSymArr()->symbols, psr.getSymArr()->size, sizeof (Elf64_Sym_W_Name), symbolComp);

    std::map <std::pair<uint64_t, uint64_t>, int> funcHashTable;

    std::cout << "I am here!1\n"; 
    fillHashMap (funcHashTable, &psr);

    std::cout << "I am here2!\n";
    dumpMapToFile (funcHashTable, &psr);

    return 0;
}
