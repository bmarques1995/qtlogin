#include "Application.hpp"
#include "FileHandler.hpp"
#include "CURLStarter.hpp"

QtSample::Application* QtSample::Application::s_Instance = nullptr;
bool QtSample::Application::s_InstanceInitialized = false;

QtSample::Application::Application(int argc, char* argv[])
{
    if(!s_InstanceInitialized)
	{
		s_Instance = this;
		s_InstanceInitialized = true;
	}
    else
    {
        throw std::runtime_error("Application already initialized");
    }

    CURLStarter::InitCurl();
#ifndef WIN32
    CURLStarter::SetCertificateLocation("/etc/ssl/certs/ca-certificates.crt");
#endif
    m_App = new QApplication(argc, argv);
    m_App->setStyle("Fusion");

    m_MainWindow = new MainWindow();
    m_MainWindow->setWindowTitle("Qt Sample");
    m_MainWindow->show();

    m_ListenServer = new ListenServer(argc, argv);
    
    // If this instance started from protocol
    if (argc > 1) {
        QString url = argv[1];
        QtSample::ListenServer::handleUrl(url);
    }

    if(FileHandler::FileExists(TokenContext::s_RefreshTokenFile))
    {
	    simdjson::dom::element jsonInput;
		m_JsonProcessor.ProcessJsonFile(TokenContext::s_RefreshTokenFile, &jsonInput);
        std::string refreshToken = jsonInput["refreshToken"].get_c_str().value();
        m_TokenContext = new TokenContext(refreshToken);
	}
    else
    {
		m_TokenContext = new TokenContext();
    }

    
}

QtSample::Application::~Application()
{
    delete m_TokenContext;
    delete m_MainWindow;
    delete m_ListenServer;
    CURLStarter::ShutdownCurl();
	delete m_App;
}

void QtSample::Application::Run()
{
    while (m_MainWindow->IsRunning())
    {
        m_App->processEvents();
    }
}

QtSample::Application* QtSample::Application::GetInstance()
{
    return s_Instance;
}

void QtSample::Application::NotifyLogin(std::string refreshToken, std::string accessToken)
{
    qDebug() << "refresh token: " << refreshToken << " access token: " << accessToken;   
    m_MainWindow->LoginAction();
	m_TokenContext->SetRefreshToken(refreshToken);
    m_TokenContext->SetAccessToken(accessToken);
}

void QtSample::Application::NotifyLogout()
{
    m_MainWindow->LogoutAction();
}

void QtSample::Application::NotifyAccessTokenValid()
{
    m_MainWindow->LoginAction();
}
