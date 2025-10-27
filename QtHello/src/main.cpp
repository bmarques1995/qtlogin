#include "MainWindow.hpp"

#ifdef SAMPLE_WINDOWED
#include <windows.h>
#endif

#include <QApplication>

int main(int argc, char* argv[])

{
    QApplication app(argc, argv);

    MainWindow w;
    w.setWindowTitle("Qt Sample");
    w.show();
    while(w.IsRunning())
    {
        app.processEvents();
    }
    return 0;
}
