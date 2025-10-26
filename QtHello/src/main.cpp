#include "MainWindow.hpp"
#include "CmdLine.hpp"

#ifdef SAMPLE_WINDOWED
#include <windows.h>
#endif

#include <QApplication>

#ifdef SAMPLE_WINDOWED
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
#ifdef SAMPLE_WINDOWED
    int argc;
    std::vector<char*> argv;
    StartCmdLine(&argc, &argv);
    QApplication app(argc, argv.data());
#else
    QApplication app(argc, argv);
#endif

    MainWindow w;
    w.setWindowTitle("Qt Sample");
    w.show();
    while(w.IsRunning())
    {
        app.processEvents();
    }
    return 0;
}
