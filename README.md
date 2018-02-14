# Ahoy

[![Build Status](https://travis-ci.org/quittle/ahoy.svg?branch=master)](https://travis-ci.org/quittle/ahoy)

Ahoy is a class-based argument parser for C++.  Its aim is to be simple and lightweight without all
the confusing bits that macros bring in or extra depenencies like Boost's program_options.

## Classes

* `Newline` - Enum for deciding what type of newline to use in messages.
* `Option` subclasses - Modifiers for parameters passed into `Parser`.
* `Parser` - The main class were parameters are defined and then used to parse the arguments passed
             into the `main(int, char**)` method.

## Example
``` cpp
#include <iostream>
#include <set>
#include <string>

#include <ahoy/ahoy_all.h>

int main(int argc, char** argv) {
    // Declare parameters with default values
    int iterations = 3;
    bool verbose = false;
    std::string type, file_name;

    // Build the parser's parameter
    ahoy::Parser parser;
    parser.AddNamedParam(&iterations, ahoy::Name("Iterations"),
                                      ahoy::Description("Number of itmes to run"),
                                      ahoy::ShortForms({"i"}),
                                      ahoy::LongForms({"iterations"}));
    parser.AddNamedParam(&verbose, ahoy::Name("Verbose"),
                                   ahoy::Description("Enables verbose logging"),
                                   ahoy::ShortForms({"v"}),
                                   ahoy::LongForms({"verbose"}),
                                   ahoy::Flag());
    parser.AddNamedParam(&type, ahoy::Name("File Type"),
                                ahoy::Description("MIME type of file"),
                                ahoy::ShortForms({"t"}),
                                ahoy::LongForms({"type"}));
    parser.AddPositionalParam(&file_name, ahoy::Name("File"),
                                          ahoy::Description("The file to process"),
                                          ahoy::Required());

    // Parses |argc| and |argv| and returns false if the user invoked the program incorrectly.
    // Parsing updates the parameters
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Unable to parse arguments" << std::endl;

        // Built-in help message generator for the parameters
        std::cerr << parser.HelpMessage() << std::endl;

        return 1;
    }

    if (verbose) {
        std::cout << "Starting work" << std::endl;
    }

    for (int i = 0; i < iterations; i++) {
        if (type == "txt") {
            std::cout << "Looping over: " << file_name << std::endl;
            } else if (verbose) {
                std::cout << "Skipping file of type: " << type << std::endl;
            }
    }

    if (verbose) {
        std::cout << "Done" << std::endl;
    }

    return 0;
}
```

Example invocation:

``` bash
./example -v --iterations=10 -t "png" ./images/example.png
```