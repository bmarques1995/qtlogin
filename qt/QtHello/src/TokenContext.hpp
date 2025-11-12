#pragma once
#include <string>

namespace QtSample
{
	class TokenContext
	{
	public:
		TokenContext();
		TokenContext(std::string_view refreshToken);

		void SetRefreshToken(std::string_view refreshToken);
		void SetAccessToken(std::string_view accessToken);

		void RenewAccessToken();

		bool IsLoggedIn() const;

		static const std::string s_RefreshTokenFile;
		static const std::string s_RefreshTokenFieldName;
	private:
		bool IsTokenValid();
		void SaveRefreshToken();
		void RenewRefreshToken();

		bool m_LoggedIn;
		std::string m_RefreshToken;
		std::string m_AccessToken;
	};
}