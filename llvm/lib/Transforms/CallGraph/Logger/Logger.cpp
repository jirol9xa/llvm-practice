#include "../../../../../elf-parser/include/parser.hpp"
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include <cstring>

/// Class for implementing Parsing and Changing dot file with graph
/// Singleton implementation
class GraphEditor {
private:
  // Ctor will dump /proc/self/maps to the maps.txt files
  // It is necessary, because we support -fPIC flag
  GraphEditor() {
    auto pid = getpid();
    char *system_text = (char *) calloc(32, sizeof(char));
    if (!system_text)
        return;

    sprintf(system_text, "cat /proc/%d/maps > maps.txt", pid);

    system(system_text);
    free(system_text);

    OutFile = fopen("Graph.txt", "w");
  };
    
  FILE *OutFile;

public:
  static GraphEditor &getInstance() {
    static GraphEditor Object;
    return Object;
  }

  void addCall(int64_t Caller, int64_t Callee);

  ~GraphEditor() { 
    fclose(OutFile);
  }
};

void GraphEditor::addCall(int64_t Caller, int64_t Callee) {
  if (OutFile)
    fprintf(OutFile, "%lx %lx\n", Caller, Callee);
}

// We will use linkonceodr, so we must say compiler not to inline the call
// Or remove it, if compiler inline in any way
void Logger() __attribute__((noinline));

void Logger() {
  GraphEditor &graph = GraphEditor::getInstance();
  graph.addCall(reinterpret_cast<int64_t>(__builtin_return_address(1)),
                reinterpret_cast<int64_t>(__builtin_return_address(0)));
}
