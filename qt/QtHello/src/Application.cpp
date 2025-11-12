#include "Application.hpp"
#include "FileHandler.hpp"

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

    m_App = new QApplication(argc, argv);
    m_App->setStyle("Fusion");

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

    m_MainWindow = new MainWindow(m_TokenContext->IsLoggedIn());
    m_MainWindow->setWindowTitle("Qt Sample");
    m_MainWindow->show();
}

QtSample::Application::~Application()
{
    delete m_TokenContext;
    delete m_MainWindow;
    delete m_ListenServer;
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
