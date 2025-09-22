#include "display.hpp"
#include <QApplication>
#include <exception>
#include <iostream>
#include <qmainwindow.h>
#include <string>

int main(int argc, char* argv[])
{
    try {
        if (argc <= 1) {
            std::cout << "Use: ./chip8 [rom]" << std::endl;
            return 0;
        }

        QApplication app(argc, argv);

        qt8::display_widget* qt_wrapper = new qt8::display_widget();
        qt_wrapper->show();
        QTimer::singleShot(100, [&]() {
            qt_wrapper->run_vm(argv[1]);
        });
        return app.exec();
    }
    catch (std::exception& e) {
        std::cerr << e.what();
    }
}
