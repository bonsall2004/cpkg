#parse("C File Header.h")

#[[#include]]# <CLI/App.hpp>
#[[#include]]# "CLIHelper.hpp"

namespace cpkg::cli::${Category}
{
  void ${Command_Name}_execute(CLI::App& app)
  {

  }

  CLI::App* ${Command_Name}_setup(CLI::App& app)
  {
    auto subcommand = app.add_subcommand("${Command_Name}", "${Description}");

    subcommand->callback([&]()
    {
      ${Command_Name}_execute(app);
    });

    return subcommand;
  }
}

REGISTER_SUBCOMMAND_SETUP(cpkg::cli::${Category}::${Command_Name}_setup);