// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

// Example program

#include <iostream>
#include <string>

#include <ahoy/ahoy_all.h>
#include <ahoy/options2.h>

#include <set> // REMOVE ME

void other_main(const int argc, char const * const * const argv) {
    std::string file_name;
    bool passed(false);

    ahoy::Parser parser = ahoy::Parser()
        .withOptions(
            ahoy::Option2(&file_name, ahoy::ShortForms({"a"}), ahoy::Description("this is a description"), ahoy::Name("name")),
            ahoy::Option2(&file_name)
        )
        .then(
            ahoy::Option2(&passed)
        );
    const bool success = parser.Parse2(argc, argv);

    std::cout << "OTHER MAIN >>>>>>" << std::endl;
    std::cout << "Success: " << (success ? "true" : "false") << std::endl;
    std::cout << "file_name: " << file_name << std::endl;
    std::cout << "passed: " << passed << std::endl;
    std::cout << "<<<<<< OTHER MAIN" << std::endl;
}

int main(const int argc, char const * const * const argv) {
    // std::set<std::string> vals = { "-a" };
    // std::cout << (vals.count("-" + std::string("a")) ? "t" : "f");
    // return 0;
    other_main(argc, argv);
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
    const bool parse_success = true;//parser.Parse(argc, argv);

    if (help) {
        std::cout << "Displaying help message" << std::endl;
        return 0;
    }

    if (!parse_success) {
        std::cerr << "Unable to parse arguments" << std::endl;
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
