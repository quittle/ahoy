# Ahoy

[![Build Status](https://travis-ci.org/quittle/ahoy.svg?branch=master)](https://travis-ci.org/quittle/ahoy)

Ahoy is a class-based argument parser for C++.  Its aim is to be simple and lightweight without all
the confusing bits that macros bring in or extra depenencies like Boost's program_options.

## Classes

All classes live in the `ahoy` namespace.

* `ahoy::Parser` - The main class were parameters are defined and then used to parse the arguments
                   passed into the `main(int, char**)` method.rameters passed into `Parser`.
* `ahoy::Parameter` - Represents a single argument for the program. Parameter instances may be
                      re-used
* `ahoy::Option` subclasses - Flags for `ahoy::Parameter`s to specify how a parameter works.

## Example
``` cpp
#include <iostream>
#include <set>
#include <string>

#include <ahoy/ahoy_all.h>

int main(int argc, char** argv) {
    // Declare parameters with default values
    int iterations = 3;
    bool verbose = false, help = false;
    std::string file_name, file_type;

    // Build the parser's parameter
    const ahoy::Parser parser = ahoy::Parser()
        .withOptions( // Options are, well, optional parameters that are not required
            ahoy::Parameter(&iterations, // Parameters take pointers to the variables to store arguments in
                    ahoy::ShortForms({"i"}), // Translates to supporting "-i=3"
                    ahoy::LongForms({"iterations"}), // Translates to supporting "--iterations 3"
                    ahoy::Name("Iterations"),
                    ahoy::Description("Number of times to run")),
            ahoy::Parameter(&verbose,
                    ahoy::ShortForms({"v"}),
                    ahoy::LongForms({"verbose"}),
                    ahoy::Name("Verbose"),
                    ahoy::Description("Enable verbose logging"),
                    ahoy::Flag()) // This option indicates there is no "value" but sets its pointer to true if found
        )
        .then( // You can branch what options are available in "then" blocks
            ahoy::Parameter(&help,
                    ahoy::ShortForms({"h"}),
                    ahoy::LongForms({"help"}),
                    ahoy::Name("Help"),
                    ahoy::Description("Displays help message"),
                    ahoy::Flag()),
            ahoy::Parameter(&file_name, // This parameter has no "form" so it is a positional arg that
                    ahoy::Name("File"), // eats up the first value that doesn't match any "form"
                    ahoy::Description("The file to process")).withOptions(
                            ahoy::Parameter(&file_type,      // Optional follow-up parameter indicating the file type.
                                    ahoy::Name("File Type")) // Only valid immediately following the file name
                    )
        );

    // Parses |argc| and |argv| and returns false if the user invoked the program incorrectly.
    // Parsing updates the parameters
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Unable to parse arguments" << std::endl;

        return 1;
    }

    if (help) {
        // Print out help message
        return 0;
    }

    if (verbose) {
        std::cout << "Starting work" << std::endl;
    }

    for (int i = 0; i < iterations; i++) {
        std::cout << "Looping over: " << file_name << std::endl;

        if (verbose && !file_type.empty()) {
            std::cout << "Processing file of type: " << file_type << std::endl;
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
./example -v --iterations=10 ./images/example.png png
```