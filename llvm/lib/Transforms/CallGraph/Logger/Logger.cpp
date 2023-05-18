#include "../../../../../elf-parser/include/parser.hpp"
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/// Class for implementing Parsing and Changing dot file with graph
/// Singleton implementation
class GraphEditor {
private:
  /// In that unordered map we store edges info:
  /// key -- addr of caller
  /// value -- map of all callees
  /// value::key -- addr of callee
  /// value::value -- calls amnt
  std::unordered_map<int64_t, std::map<int64_t, int64_t>> Graph;

  void writeGraph() const;

  // Ctor will dump /proc/self/maps to the maps.txt files
  // It is necessary, because we support -fPIC flag
  GraphEditor() {
    auto pid = getpid();
    std::string system_text =
        "cat /proc/" + std::to_string(pid) + "/maps > maps.txt";
    system(system_text.c_str());
  };

public:
  static GraphEditor &getInstance() {
    static GraphEditor Object;
    return Object;
  }

  void addCall(int64_t Caller, int64_t Callee);

  // We will write to file in the destructor, because we want to do it
  // only one time
  ~GraphEditor() { writeGraph(); }
};

void GraphEditor::addCall(int64_t Caller, int64_t Callee) {
  Graph[Caller][Callee]++;
}

void GraphEditor::writeGraph() const {
  std::ofstream OutFile;
  OutFile.open("Graph.txt");
  if (!OutFile.is_open())
    return;

  // Now need traverse Graph map and print edges to dot file
  for (auto &Edges : Graph) {
    for (auto &Child : Edges.second) {
      // Edge.first - addr of caller
      // Edge.second - map Children (<callee, calls_amnt>)
      // Child - pair <callee_addr, calls_amnt>
      OutFile << std::hex << Edges.first << ' ' << Child.first << ' '
              << std::dec << Child.second << '\n';
    }
  }
  OutFile.close();
}

// We will use linkonceodr, so we must say compiler not to inline the call
// Or remove it, if compiler inline in any way
void Logger() __attribute__((noinline));

void Logger() {
  GraphEditor &graph = GraphEditor::getInstance();
  graph.addCall(reinterpret_cast<int64_t>(__builtin_return_address(1)),
                reinterpret_cast<int64_t>(__builtin_return_address(0)));
}
