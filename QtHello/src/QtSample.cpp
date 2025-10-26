// QtSample.cpp: define o ponto de entrada para o aplicativo.
//

#include "QtSample.hpp"

#ifdef SAMPLE_WINDOWED
#include <windows.h>
#endif

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <cstdint> // for uint32_t
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QFontDatabase>
#include "CmdLine.hpp"

class ClosableQWidget : public QWidget
{
public:
    ClosableQWidget(bool* runningTrace): m_RunningTrace(runningTrace) {}
protected:
    void closeEvent(QCloseEvent* event) override
    {
        *m_RunningTrace = false;
        QWidget::closeEvent(event); // Call base implementation
    }
private:
	bool* m_RunningTrace;
};

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

#ifdef APP_USES_LINUX
    int id = QFontDatabase::addApplicationFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QApplication::setFont(QFont(family));
#endif

    bool running = true;
    ClosableQWidget window(&running);
    window.setWindowTitle("Hello World Clicker");
    window.resize(640, 480);

    QLabel* label = new QLabel("Hello World");
    QPushButton* button = new QPushButton("Click Me");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);
    window.setLayout(layout);

    uint32_t counter = 0;

    // On button click, increment counter and update text
    QObject::connect(button, &QPushButton::clicked, [&]() {
        counter++;
        label->setText(QString("Clicked %1 times").arg(counter));
        });

    window.show();

    QNetworkAccessManager manager;

    QObject::connect(&manager, &QNetworkAccessManager::finished,
        [&](QNetworkReply* reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                qDebug() << "Response:" << data;
            }
            else {
                qDebug() << "Error:" << reply->errorString();
            }
            reply->deleteLater();
        });

    QNetworkRequest request(QUrl("http://127.0.0.1:8000/auth/echo_route"));
    request.setRawHeader("Origin", "QtApp");
    request.setRawHeader("Content-Type", "application/json");
    manager.sendCustomRequest(request, "POST", QByteArray("{\"msg\": \"Hello\"}"));

    // Manual event loop
    while (running) {
        app.processEvents();
        // Could add small sleep here if desired to avoid CPU spinning
    }

    return 0;
}
