# Ahoy

Ahoy is a class-based argument parser for C++.  Its aim is to be simple and lightweight without all
the confusing bits that macros bring in or extra depenencies like Boost's program_options.

## Classes

* `ActualizedParameter` - The parameters used by your program and returned by
                          `ParseResult::params()`.
* `FormalParameter` - One of the types of parameters passed into `Parser::Create`. These are
                      switches (`-f`/`--foo`) that may or may not have values.
* `NamedFormalParameter` - One of the types of parameters passed into `Parser::Create`. These are
                           regular arguments that required by the program.
* `Newline` - Enum for deciding what type of newline to use in messages.
* `Parser` - Takes in `FormalParameter`s and `NamedFormalParameters`, processes arguments passed in,
             and generates `ParseResult`s.
* `ParseResult` - The results of a `Parser` processing arguments.

## Example
``` cpp
#include <iostream>
#include <string>

#include <ahoy/ahoy-all.h>

int main(int argc, char** argv) {
    // Use constants or an enum or any basic type as IDs
    const char param_iterations = 0;
    const char param_verbose = 1;
    const char param_type = 2;
    const char param_file_name = 3;

    // Build the parser
    const ahoy::Parser<char> parser = ahoy::Parser<char>::Create(
        {
            { param_iterations, { // ID
                {"i"}, // Short Forms
                {"iterations"}, // Long Forms
                "Number of times to run", // Description
                "3" // Default Value
            } },
            { param_verbose, { // ID
                {"v"}, // Short Forms
                {"verbose"}, // Long Forms
                "Enables verbose logging", // Description
                true // (Optional but defaults to false) Marks this as a boolean-flag switch
            } },
            { param_type, { // ID
                {"t"}, // Short Forms
                {"type"}, // Long Forms
                "MIME type of file", // Description
                false // (Optional, defaults to false) Marks this as a non-boolean-flag switch,
                      // which makes is a required switch as it has no default value
            } }
        }, {
            { param_file_name, { // ID
                "file", // Name of the argument
                "The file to process" // Description
            } }
        }
    );

    // Check to make sure that the parser was constructed properly. This should be deterministic at
    // runtime given the same build.
    if (!parser) {
        std::cerr << "Invalid parser" << std::endl;
        return 1;
    }

    // Parses |argc| and |argv|
    ahoy::ParseResult<char> parsed_args = parser.Parse(argc, argv);

    // If |parsed_args| is falsey, then user called the program incorrectly
    if (!parsed_args) {
        std::cerr << "Unable to parse arguments" << std::endl;
        // Built-in help message generator for the parameters
        std::cerr << parser.HelpMessage() << std::endl;
        // Stores a std::vector of errors found when parsing
        for (const std::string& error : parsed_args.errors()) {
            std::cerr << error << std::endl;
        }
        return 2;
    }

    // Parameter values are stored as a map
    std::map<char, ahoy::ActualizedParameter> params = parsed_args.params();

    // The map is keyed with the IDs used during creation of the parser
    bool verbose = params[param_verbose].AsBool();
    int iterations = params[param_iterations].AsInt();
    std::string type = params[param_type].AsString();
    std::string file_name = params[param_file_name].AsString();

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

Example run:

``` bash
./example -v --iterations=10 -t "png" ./images/example.png
```