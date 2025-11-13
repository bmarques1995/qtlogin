#include "TokenContext.hpp"
#include "FileHandler.hpp"
#include <simdjson.h>
#include <QWidget>
#include "Application.hpp"

const std::string QtSample::TokenContext::s_RefreshTokenFile = "refreshToken.json";
const std::string QtSample::TokenContext::s_RefreshTokenFieldName = "refreshToken";
QtSample::TokenContext::TokenContext() :
	m_RefreshToken(""), m_AccessToken(""), m_LoggedIn(false), m_Fetcher(new Fetcher())
{
}

QtSample::TokenContext::TokenContext(std::string_view refreshToken) :
	m_RefreshToken(refreshToken), m_AccessToken(""), m_Fetcher(new Fetcher())
{
	RenewAccessToken();
}

QtSample::TokenContext::~TokenContext()
{
	delete m_Fetcher;
}

void QtSample::TokenContext::SetRefreshToken(std::string_view refreshToken)
{
	m_RefreshToken = refreshToken;
	m_LoggedIn = IsTokenValid();
	SaveRefreshToken();
}

void QtSample::TokenContext::SetAccessToken(std::string_view accessToken)
{
	m_AccessToken = accessToken;
}

void QtSample::TokenContext::RenewAccessToken()
{
	if (m_RefreshToken.empty())
	{
		return;
	}

	auto response = m_Fetcher->Fetch("http://localhost:7700/auth/renew_access_token", "POST", "", { { "Authorization", "Bearer " + m_RefreshToken } });
	auto app = Application::GetInstance();
	if(response->Code != 200)
	{
		app->NotifyLogout();
		return;
	}
	simdjson::dom::element obj;
	std::string strBody = response->Body->ToString();
	m_JsonProcessor.ProcessJsonText(strBody, &obj);

	m_RefreshToken = obj["refresh_token"].get_string().value().data();
	m_AccessToken = obj["access_token"].get_string().value().data();
	SaveRefreshToken();
	app->NotifyAccessTokenValid();
}

bool QtSample::TokenContext::IsLoggedIn() const
{
	return m_LoggedIn;
}

bool QtSample::TokenContext::IsTokenValid()
{
	return true;
}

void QtSample::TokenContext::SaveRefreshToken()
{
	std::string jsonRefreshToken = "{\"" + s_RefreshTokenFieldName + "\":\"" + m_RefreshToken + "\"}";
	FileHandler::WriteTextFile(s_RefreshTokenFile, jsonRefreshToken);
}

