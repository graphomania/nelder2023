#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <string_view>
#include <algorithm>

#include "../internal/method_nelder_mead.h"
#include "../internal/method_random_walk.h"
#include "../internal/test_functions.h"

struct Argumemt {
    std::unique_ptr<Method> method{};
    Function function = nullptr;

    std::optional<Area> area;
    size_t dimensions{};
    bool help{};
    bool debug{};
};

class CLI {
    static std::unique_ptr<Method> parse_method(const std::string_view method_name, const bool debug) {
        auto tracer = Tracer::muted();
        if (debug) {
            tracer = Tracer::logging();
        }
        if (method_name.starts_with("nelder")) {
            return std::make_unique<NelderMeadMethod>(tracer);
        }
        if (method_name.starts_with("walk")) {
            return std::make_unique<RandomWalk>(tracer);
        }
        throw std::invalid_argument("unexpected method argument");
    }

    static Function parse_function(const std::string_view method_name) {
        if (method_name.starts_with("himm")) {
            return himmelblau_function;
        }
        if (method_name.starts_with("rastr")) {
            return rastrigin_function;
        }
        throw std::invalid_argument("unexpected function argument");
    }

    static bool parse_debug(const std::vector<std::string>&args) {
        for (auto&arg: args) {
            if (arg == "-d" || arg == "--debug") { return true; }
        }
        return false;
    }

    static double parse_double(const std::string&value) {
        std::istringstream iss(value);
        double ret;
        iss >> ret;
        return ret;
    }

    static size_t parse_size_t(const std::string&value) {
        std::istringstream iss(value);
        size_t ret;
        iss >> ret;
        return ret;
    }

    static size_t parse_dim(const std::vector<std::string>&args) {
        for (size_t i = 0; i < args.size(); i++) {
            if (auto&arg = args[i]; arg == "-D" || arg == "--dimensions") {
                if (i + 1 == args.size()) {
                    throw std::invalid_argument("BAD ARGUMENT, USE --help FOR HELP");
                }
                return parse_size_t(args[i + 1]);
            }
        }

        return 2;
    }

public:
    static std::string help() {
        return "Simple numberic methods for finding local min/max of functions.\n"
                "usage: ./nelder [-h -d -a -D] --method <method> --function <function>\n"
                "\n"
                "> Required:\n"
                "-m/--method <nedler | walk> -- method to use (Nelder Mead or Random Walk)\n"
                "-f/--func   <himm | rastr>  -- function to test (Himmelblau (2d), Rastrigin (Nd))\n"
                "> Optional:\n"
                "-a/--area   <min> <max>     -- area to to look in (cube [min, max]x[min, max]...)\n"
                "-D/--dim    <dimension>     -- dimensions N (default: 2)\n"
                "-h/--help                   -- get this help message and exit\n"
                "-d/--debug                  -- print debug tracing info\n"
                "\n"
                "// src: https://github.com/graphomania/nedler2023\n";
    }

    static Argumemt parse(const int argn, char* argv[]) {
        auto args = std::vector<std::string>(argn - 1);
        for (size_t i = 1; i < argn; i++) {
            args[i - 1] = argv[i];
        }
        return parse(args);
    }

    static std::pair<Argumemt, std::optional<std::string>> try_parse(const int argn, char* argv[]) {
        try {
            return {parse(argn, argv), {}};
        }
        catch (std::invalid_argument&err) {
            return {Argumemt{}, err.what()};
        }
    }

    static Argumemt parse(const std::vector<std::string>&args) {
        Argumemt arguments{};
        arguments.debug = parse_debug(args);
        arguments.dimensions = parse_dim(args);

        for (size_t i = 0; i < args.size(); i++) {
            if (auto&arg = args[i]; arg == "-h" || arg == "--help") {
                arguments.help = true;
                return arguments;
            }
            else if (arg == "-m" || arg == "--method") {
                if (i + 1 == args.size()) {
                    throw std::invalid_argument("BAD ARGUMENT, USE --help FOR HELP");
                }
                arguments.method = parse_method(args[i + 1], arguments.debug);
                i += 1;
            }
            else if (arg == "-f" || arg == "--func" || arg == "--function") {
                if (i + 1 == args.size()) {
                    throw std::invalid_argument("BAD ARGUMENT, USE --help FOR HELP");
                }
                arguments.function = parse_function(args[i + 1]);
                i += 1;
            }
            else if (arg == "-a" || arg == "--area") {
                if (i + 2 >= args.size()) {
                    throw std::invalid_argument("BAD ARGUMENT, USE --help FOR HELP");
                }
                auto min = parse_double(args[i + 1]);
                auto max = parse_double(args[i + 2]);
                arguments.area = Area({std::vector(arguments.dimensions, min)},
                                      {std::vector(arguments.dimensions, max)});
                i += 2;
            }
        }

        if (!arguments.area.has_value()) {
            arguments.area = Area({std::vector(arguments.dimensions, -5.0)},
                                  {std::vector(arguments.dimensions, 5.0)});
        }

        if (arguments.method == nullptr) {
            throw std::invalid_argument("no method is passed");
        }
        if (arguments.function == nullptr) {
            throw std::invalid_argument("no function is passed");
        }

        return arguments;
    }
};

#endif //ARGS_H
