#include "QtListenServer.hpp"
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

const QString QtListenServer::s_StaticHost = "qtsample_singleton";

QtListenServer::QtListenServer(int argc, char* argv[])
{
    
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

QtListenServer::~QtListenServer()
{
}

void QtListenServer::handleUrl(const QString& url)
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
        QJsonObject obj = doc.object();
        QString action = obj["action"].toString();
        QJsonObject item = obj["item"].toObject();
        qDebug() << "Received action:" << action << "item:" << item;

        QMessageBox::information(
            nullptr,
            "Received from Next.js",
            QString("Action: %1\nItem: %2\nPrice: %3")
            .arg(action)
            .arg(item["name"].toString())
            .arg(item["price"].toDouble())
        );
    }
    else {
        QMessageBox::warning(nullptr, "Error", "Invalid JSON payload.");
    }
}
