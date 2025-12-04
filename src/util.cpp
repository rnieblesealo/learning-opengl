#include "util.h"

#include <fstream>
#include <sstream>

namespace gle
{
std::string ReadWholeFile(std::filesystem::path const &filepath)
{
  std::ifstream      fstream(filepath); // NOTE: fstreams are closed automatically; ifstream uses RAII
  std::ostringstream string_stream;     // ostringstream is a fancy string type that offers fancy I/O operations

  string_stream << fstream.rdbuf();

  return string_stream.str();
}
} // namespace gle
