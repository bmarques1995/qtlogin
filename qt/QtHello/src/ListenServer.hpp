#pragma once

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QLocalServer>
#include <QLocalSocket>
#include "JsonProcessor.hpp"

namespace QtSample
{
	class ListenServer
	{
	public:
		ListenServer(int argc, char* argv[]);
		~ListenServer();

		static void handleUrl(const QString& url);

	private:
		static const QString s_StaticHost;
		QLocalSocket m_Socket;
		QLocalServer m_Server;
		static JsonProcessor s_JsonProcessor;
	};
}