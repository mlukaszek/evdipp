#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "evdi.hpp"
#include "screen.hpp"
#include "driver.hpp"

namespace {

int first_available_device()
{
    using namespace boost;
    using namespace boost::filesystem;

    const path dri("/dev/dri");
    if (is_directory(dri)) {
        for (auto& entry : directory_iterator(dri))
        {
            const std::string name = entry.path().filename().string();
            if (starts_with(name, "card")) {
                try {
                    int num = lexical_cast<int>(name.substr(4));
                    if (evdi_check_device(num) == AVAILABLE) {
                        return num;
                    }
                } catch (bad_lexical_cast&) {
                    continue;
                }
            }
        }
    }
    return -1;
}

bool read_edid_from_file(const std::string& filename, std::vector<unsigned char>& edid)
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

int main(int argc, char *argv[]) {
    std::vector<unsigned char> edid;
    if (argc > 1) {
        if (!read_edid_from_file(argv[1], edid)) {
            std::cerr << "Reading the EDID file " << argv[1] << " failed." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Usage: " << argv[0] << " <sample.edid>" << std::endl;
        return 1;
    }

    std::unique_ptr<Evdi> evdi(new Evdi);
    int devnum = first_available_device();
    if (devnum < 0) {
        evdi->add();
        devnum = first_available_device();
    }

    if (devnum >= 0) {
        std::cout << "Attempting to use device /dev/dri/card" << devnum << "..." << std::endl;
        evdi.reset(new Evdi(devnum));
    }

    if (!evdi || !*evdi) {
        std::cerr << "No usable EVDI found" << std::endl;
        return 2;
    }

    auto screen = std::make_shared<Screen>(*evdi, edid);

    Driver driver;
    driver.add_screen(screen);
    driver.drive(60); // Hz

    return 0;
}
