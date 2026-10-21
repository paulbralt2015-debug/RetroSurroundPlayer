#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("RetroSurround Player");
    QApplication::setOrganizationName("RetroSurround");
    MainWindow window;
    window.show();
    return app.exec();
}
