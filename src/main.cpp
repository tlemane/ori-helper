#include "cli.hpp"
#include "utils.hpp"
#include "bit_utils.hpp"

#include <filesystem>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/sfstream.hpp>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
  auto [cli, opt] = get_cli();

  try
  {
    (*cli).parse(argc, argv);
  }
  catch (const bc::ex::BCliError& e)
  {
    bc::utils::exit_bcli(e);
    exit(EXIT_FAILURE);
  }

  std::vector<std::string> files;
  std::ifstream infof(opt->fof_path, std::ios::in); check_fstream_good(opt->fof_path, infof);
  for (std::string line; std::getline(infof, line);)
  {
    if (!fs::exists(line))
      throw std::runtime_error(line + " doesn't exist.");
    files.push_back(line);
  }

  std::vector<std::vector<uint8_t>> data(files.size());
  int i = 0;
  for (auto& f: files)
  {
    std::ifstream bf(f, std::ios::binary | std::ios::in); check_fstream_good(f, bf);
    bf.seekg(0x70, bf.beg);
    sdsl::bit_vector tmp = sdsl::bit_vector();
    sdsl::load(tmp, bf);
    size_t nbits = tmp.bit_size();
    if (nbits < opt->bits)
    {
      opt->bits = nbits;
    }
    else if (opt->bits == 0)
    {
      opt->bits = nbits;
    }
    data[i].resize(opt->bits/8, 0);
    std::copy(tmp.begin(), tmp.begin()+((opt->bits/8)-1), data[i].begin());
    i++;
  }

  std::vector<std::vector<double>> res(files.size(), std::vector<double>(files.size(), 0.0));

  for (size_t v=0; v<files.size(); v++)
  {
    for (size_t w=v+1; w<files.size(); w++)
    {
      auto [x, y] = common_bits(data[v].data(), data[w].data(), opt->bits);

      res[v][w] = x;
      res[w][v] = y;
    }
  }

  std::ofstream out(opt->output, std::ios::out); check_fstream_good(opt->output, out);
  for (size_t i=0; i<res.size(); i++)
  {
    for (size_t j=0; j<res.size(); j++)
    {
      out << res[i][j] << " ";
    }
    out << "\n";
  }

  return 0;
}