#include <QApplication>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <fstream>
#include <iostream>

#include <libevdipp/evdi.hpp>
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
        std::cerr << "Warning: no argument passed, using built-in sample EDID" << std::endl;
    }

    QApplication app(argc, argv);

    auto log = new QListWidget;
    Evdi::log_handler = [&log](const std::string& message) {
        new QListWidgetItem(QString::fromStdString(message), log);
    };

    Evdi evdi;
    if (!evdi) {
        std::cerr << "No usable EVDI found" << std::endl;
        return 3;
    }

    auto window = new QWidget;

    QEvdiScreen screen(evdi, edid);
    auto preview = new ScreenPreview(screen);

    auto grid = new QGridLayout(window);
    auto button = new QPushButton("Save screenshot");
    grid->addWidget(button, 0, 0);
    grid->addWidget(log, 0, 1);
    grid->addWidget(preview, 1, 0, 1, 2);
    window->setLayout(grid);
    window->connect(button, SIGNAL(clicked()), preview, SLOT(save_screenshot()));

    window->show();
    return app.exec();
}
