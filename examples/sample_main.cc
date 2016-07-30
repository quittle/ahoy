// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

// Example program

#include <iostream>
#include <string>

#include <ahoy/ahoy_all.h>

int main(const int argc, char const * const * const argv) {
    // Set default values for parameters
    int iterations = 3;
    bool verbose = false;
    bool help = false;
    std::string file_name;

    // Create parser and set parameters
    ahoy::Parser parser;
    parser.AddNamedParam(&iterations,
            ahoy::ShortForms({"i"}), ahoy::LongForms({"iterations"}),
            ahoy::Name("Iterations"), ahoy::Description("Number of times to run"));
    parser.AddNamedParam(&verbose,
            ahoy::ShortForms({"v"}), ahoy::LongForms({"verbose"}),
            ahoy::Name("Verbose"), ahoy::Description("Enable verbose logging"),
            ahoy::Flag());
    parser.AddNamedParam(&help,
            ahoy::ShortForms({"h"}), ahoy::LongForms({"help"}),
            ahoy::Name("Help"), ahoy::Description("Displays help message"),
            ahoy::Flag());
    parser.AddPositionalParam(&file_name,
            ahoy::Name("File"), ahoy::Description("The file to process"),
            ahoy::Required());

    // Current limitation of parser does not allow branching so missing, required parameters will
    // fail the parsing even if the help parametrer is passed in
    const bool parse_success = parser.Parse(argc, argv);

    if (help) {
        std::cout << "Displaying help message" << std::endl;
        std::cout << parser.HelpMessage() << std::endl;
        return 0;
    }

    if (!parse_success) {
        std::cerr << "Unable to parse arguments" << std::endl;
        std::cerr << parser.HelpMessage() << std::endl;
        return 1;
    }

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
