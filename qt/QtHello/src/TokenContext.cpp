#include "TokenContext.hpp"
#include "FileHandler.hpp"
#include <simdjson.h>

const std::string QtSample::TokenContext::s_RefreshTokenFile = "refreshToken.json";
const std::string QtSample::TokenContext::s_RefreshTokenFieldName = "refreshToken";
QtSample::TokenContext::TokenContext() :
	m_RefreshToken(""), m_AccessToken(""), m_LoggedIn(false)
{
}

QtSample::TokenContext::TokenContext(std::string_view refreshToken) :
	m_RefreshToken(refreshToken), m_AccessToken("")
{
	RenewRefreshToken();
	RenewAccessToken();
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

void QtSample::TokenContext::RenewRefreshToken()
{
	m_LoggedIn = IsTokenValid();
	// Ask for new refresh token
	SaveRefreshToken();
}
