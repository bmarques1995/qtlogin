#pragma once
#include <QApplication>
#include "MainWindow.hpp"
#include "ListenServer.hpp"
#include "TokenContext.hpp"
#include "JsonProcessor.hpp"

namespace QtSample
{
	class Application
	{
	public:
		Application(int argc, char* argv[]);
		~Application();

		void Run();

		static Application* GetInstance();

		void NotifyLogin(std::string refreshToken, std::string accessToken);

		void NotifyLogout();

		void NotifyAccessTokenValid();

	private:
		static Application* s_Instance;
		static bool s_InstanceInitialized;

		JsonProcessor m_JsonProcessor;
		MainWindow* m_MainWindow;
		QApplication* m_App;
		ListenServer* m_ListenServer;
		TokenContext* m_TokenContext;
	};
}
