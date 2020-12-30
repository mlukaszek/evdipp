#include <QApplication>
#include <fstream>
#include <iostream>

#include "screenpreview.h"

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
        return 2;
    }

    Evdi evdi;
    if (!evdi) {
        std::cerr << "No usable EVDI found" << std::endl;
        return 3;
    }

    QApplication app(argc, argv);
    QEvdiScreen screen(evdi, edid);
    ScreenPreview preview(screen);
    preview.show();

    return app.exec();
}
