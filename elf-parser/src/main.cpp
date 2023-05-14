#include <iostream>
#include "parser.hpp"

int main (int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Error: missing file name!\n";
        return -1;
    }

    uint8_t *binary = createBuffer (argv[1]);
    if (!binary) {
        return -1;
    }

    Elf64_Ehdr *elfHeader = (Elf64_Ehdr *)binary;
    Elf64_Sym_Arr *symbolArr = getSymbols(elfHeader);
    if (!symbolArr) {
        return -1;
    }

    printSymbolsValues (symbolArr);
    qsort ((void *)symbolArr->symbols, symbolArr->size, sizeof (Elf64_Sym_W_Name), symbolComp);
    printSymbolsValues (symbolArr);

    return 0;
}