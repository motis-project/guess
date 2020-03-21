#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include "guess/guesser.h"

using namespace guess;

struct new_line_tokenizer : std::ctype<char> {
  new_line_tokenizer() : std::ctype<char>(get_table()) {}
  static mask const* get_table() {
    static mask rc[table_size];
    rc['\n'] = std::ctype_base::space;
    return &rc[0];
  }
};

int main(int argc, char const** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " FILENAME\n";
    return 1;
  }

  auto line = 0U;
  std::vector<std::pair<std::string, float>> stations;
  try {
    std::fstream in(argv[1]);
    in.exceptions(std::ifstream::failbit);
    in.imbue(std::locale(std::cin.getloc(), new new_line_tokenizer));
    for (auto it = std::istream_iterator<std::string>(in),
              end = std::istream_iterator<std::string>();
         it != end; ++it) {
      stations.emplace_back(*it, 1.0F);
      ++line;
    }
  } catch (std::exception const& e) {
    std::cout << "WARNING file read error at line " << line << " : " << e.what()
              << "\n ";
    if (stations.empty()) {
      return 1;
    }
  }

  guesser g(stations);

  std::string input;
  while (std::cout << "$ " && std::getline(std::cin, input)) {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::system_clock;
    auto start = system_clock::now();
    auto candidates = g.guess_match(input);
    auto duration = duration_cast<milliseconds>(system_clock::now() - start);
    std::cout << "\n\t" << duration.count() << "ms\n\n";

    for (auto const& guess : candidates) {
      std::cout << "  " << stations[guess.index].first << "\n";
    }
    std::cout << "\n";
  }
}