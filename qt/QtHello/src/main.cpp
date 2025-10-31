#include "MainWindow.hpp"

#ifdef SAMPLE_WINDOWED
#include <windows.h>
#endif

#include <QApplication>
#include "ProcessWebInputs.hpp"
#include "QtListenServer.hpp"
#include <QMessageBox>

int main(int argc, char* argv[])

{
    QApplication app(argc, argv);

	QtListenServer server(argc, argv);
    
    // If this instance started from protocol
    if (argc > 1) {
        QString url = argv[1];
        QtListenServer::handleUrl(url);
    }

    MainWindow w;
    w.setWindowTitle("Qt Sample");
    w.show();

    while(w.IsRunning())
    {
        app.processEvents();
    }
    return 0;
}
