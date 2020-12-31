#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>

#include "driver.hpp"
#include "libevdipp/evdi.hpp"
#include "libevdipp/screen.hpp"

namespace {

bool read_edid_from_file(const std::string& filename,
    std::vector<unsigned char>& edid)
{
    try {
        std::ifstream input(filename.c_str(), std::ios::binary | std::ios::ate);
        input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        if (input) {
            std::streamsize size = input.tellg();
            input.seekg(0, std::ios::beg);
            edid.resize(size);
            input.read((char*)edid.data(), size);
            return input.good();
        }
    } catch (const std::ios::failure&) {
        return false;
    }
    return false;
}

} // anonymous namespace

int main(int argc, char* argv[])
{
    std::vector<unsigned char> edid;
    if (argc > 1) {
        if (!read_edid_from_file(argv[1], edid)) {
            std::cerr << "Reading the EDID file " << argv[1] << " failed."
                      << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " <sample.edid>" << std::endl;
        return 1;
    }

    Evdi::log_handler = [](const std::string& message) {
        std::cout << "example: " << message << std::endl;
    };

    Evdi evdi;
    if (!evdi) {
        std::cerr << "No usable EVDI found" << std::endl;
        return 2;
    }

    auto screen = std::make_shared<Screen>(evdi, edid);

    Driver driver;
    driver.add_screen(screen);
    driver.drive(60); // Hz

    return 0;
}
