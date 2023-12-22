#include "args.h"

using namespace std;


int main(const int argc, char* argv[]) {
    auto [args, err] = CLI::try_parse(argc, argv);
    if (err.has_value()) {
        std::cout << "error: " << err.value() << " -- use `--help` for help.\n";
        return 1;
    }
    if (args.help) {
        std::cout << CLI::help();
        return 0;
    }

    auto [point, value] = args.method->minimal(args.function, args.area.value());

    std::cout << args.method->name() << " minimal in area " << args.area->to_string() << "\n"
            << "point: " << point << ", function value = " << value << "\n";

    return 0;
}
