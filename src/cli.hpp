#pragma once

#include <tuple>
#include <string>

#include <bcli/bcli.hpp>

using cli_t = std::shared_ptr<bc::Parser<0>>;

struct options
{
  std::string fof_path;
  std::string output;
  uint64_t bits;
};

using options_t = std::shared_ptr<struct options>;

inline std::tuple<cli_t, options_t> get_cli()
{
  cli_t cli = std::make_shared<bc::Parser<0>>("ori-helper", "ori-helper", "0.1", "");
  options_t opt = std::make_shared<struct options>();

  cli->add_param("-l/--list", "Fof with one bf path per line.")
     ->meta("FILE")
     ->checker(bc::check::is_file)
     ->setter(opt->fof_path);

  cli->add_param("-o/--output", "output matrix.")
     ->meta("FILE")
     ->def("output.txt")
     ->setter(opt->output);

  auto c = [](const std::string& s, const std::string& v) -> bc::check::checker_ret_t {
    size_t b = std::stoull(v);
    bool t = true;
    if (b % 8 != 0)
      t = false;
    return std::make_tuple(t, "--bits must be a multiple of 8.");
  };

  cli->add_param("-b/--bits", "Number of bits to use. (0 -> all)")
     ->meta("INT")
     ->def("0")
     ->checker(bc::check::is_number)
     ->setter(opt->bits);

  cli->add_common();

  return std::make_tuple(cli, opt);
};