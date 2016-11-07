#include <QApplication>
#include <QDir>

#include <iostream>
#include <fstream>

#include "screenpreview.h"

namespace {

int first_available_evdi()
{
	QDir dri("/dev/dri");
	if (dri.exists()) {
		bool ok;
		for (auto& entry : dri.entryList(QStringList("card*"), QDir::System)) {
			int num = entry.remove("card").toInt(&ok);
			if (ok && evdi_check_device(num) == AVAILABLE) {
				return num;
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


int main(int argc, char *argv[])
{
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
    int devnum = first_available_evdi();
    if (devnum < 0) {
        evdi->add();
        devnum = first_available_evdi();
    }

    if (devnum >= 0) {
        evdi = std::make_unique<Evdi>(devnum);
    }

    if (!evdi || !*evdi) {
        std::cerr << "No usable EVDI found" << std::endl;
        return 2;
    }

    QApplication app(argc, argv);
    QEvdiScreen screen(*evdi, edid);
    ScreenPreview preview(screen);
    preview.show();

    return app.exec();
}
