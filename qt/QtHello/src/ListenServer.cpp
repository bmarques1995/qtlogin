#include "ListenServer.hpp"
#include "Application.hpp"
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

const QString QtSample::ListenServer::s_StaticHost = "qtsample_singleton";
QtSample::JsonProcessor QtSample::ListenServer::s_JsonProcessor;

QtSample::ListenServer::ListenServer(int argc, char* argv[])
{
 
    s_JsonProcessor;
    m_Socket.connectToServer(s_StaticHost);

    if (m_Socket.waitForConnected(200)) {
        // Another instance is running
        if (argc > 1) {
            QString url = argv[1];
            m_Socket.write(url.toUtf8());
            m_Socket.flush();
            m_Socket.waitForBytesWritten(200);
        }
        // Exit, the existing instance will handle it
        exit(0);
    }

    // No instance found: become the main one
    QLocalServer::removeServer(s_StaticHost);
    m_Server.listen(s_StaticHost);

    QObject::connect(&m_Server, &QLocalServer::newConnection, [&]() {
        QLocalSocket* client = m_Server.nextPendingConnection();
        QObject::connect(client, &QLocalSocket::readyRead, [client]() {
            QString url = QString::fromUtf8(client->readAll());
            handleUrl(url);
            client->disconnectFromServer();
            });
        });

}

QtSample::ListenServer::~ListenServer()
{
}

void QtSample::ListenServer::handleUrl(const QString& url)
{
    QUrl qurl(url);
    if (qurl.scheme() != "qtsample")
        return;

    QUrlQuery query(qurl);
    QString data = query.queryItemValue("data");
    QString decoded = QUrl::fromPercentEncoding(data.toUtf8());

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(decoded.toUtf8(), &err);
    if (err.error == QJsonParseError::NoError && doc.isObject()) {
        simdjson::dom::element obj;
        s_JsonProcessor.ProcessJsonText(decoded.toStdString(), &obj);
        std::string refreshToken = obj["refresh"].get_string().value().data();
        std::string accessToken = obj["access"].get_string().value().data();
        auto app = Application::GetInstance();
        app->NotifyLogin(refreshToken, accessToken);
    }
    else {
        QMessageBox::warning(nullptr, "Error", "Invalid JSON payload.");
    }
}
