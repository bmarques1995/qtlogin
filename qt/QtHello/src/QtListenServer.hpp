#pragma once

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QLocalServer>
#include <QLocalSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
	class QtListenServer;
}
QT_END_NAMESPACE

class QtListenServer
{
public:
	QtListenServer(int argc, char* argv[]);
	~QtListenServer();

	static void handleUrl(const QString& url);

private:
	static const QString s_StaticHost;
	QLocalSocket m_Socket;
	QLocalServer m_Server;
};