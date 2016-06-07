// Example program

#include <iostream>
#include <string>

#include <ahoy/ahoy-all.h>

int main(const int argc, char const * const * const argv) {
    const char param_iterations = 0;
    const char param_verbose = 1;
    const char param_help = 2;
    const char param_file_name = 3;
    const auto parser = ahoy::Parser<char>::Create(
        {
            { param_iterations, { {"i"}, {"iterations"}, "Number of times to run", "3" } },
            { param_verbose, { {"v"}, {"verbose"}, "Enables verbose logging", true } },
            { param_help, { {"h"}, {"help"}, "Displays help message", true } }
        }, {
            { param_file_name, { "file", "The file to process" } }
        }
    );

    if (!parser) {
        std::cerr << "Invalid parser" << std::endl;
        return 1;
    }

    const ahoy::ParseResult<char> parsed_args = parser.Parse(argc, argv);

    // Non-const so [] can be used
    std::map<char, ahoy::ActualizedParameter> params = parsed_args.params();

    if (params[param_help].AsBool()) {
        std::cout << "Displaying help message" << std::endl;
        std::cout << parser.HelpMessage() << std::endl;
        return 0;
    }

    if (!parsed_args) {
        std::cerr << "Unable to parse arguments" << std::endl;
        std::cerr << parser.HelpMessage() << std::endl;
        for (const std::string& error : parsed_args.errors()) {
            std::cerr << error << std::endl;
        }
        return 2;
    }

    const bool verbose = params[param_verbose].AsBool();
    const int iterations = params[param_iterations].AsInt();
    const std::string file_name = params[param_file_name].AsString();



    if (verbose) {
        std::cout << "Starting work" << std::endl;
    }

    for (int i = 0; i < iterations; i++) {
        std::cout << "Looping over: " << file_name << std::endl;
    }

    if (verbose) {
        std::cout << "Done" << std::endl;
    }

    return 0;
}
