#pragma once
#include <string>
#include "Fetcher.hpp"
#include "JsonProcessor.hpp"

namespace QtSample
{
	class TokenContext
	{
	public:
		TokenContext();
		TokenContext(std::string_view refreshToken);

		~TokenContext();

		void SetRefreshToken(std::string_view refreshToken);
		void SetAccessToken(std::string_view accessToken);

		void RenewAccessToken();

		bool IsLoggedIn() const;

		static const std::string s_RefreshTokenFile;
		static const std::string s_RefreshTokenFieldName;
	private:
		bool IsTokenValid();
		void SaveRefreshToken();

		Fetcher* m_Fetcher;
		bool m_LoggedIn;
		std::string m_RefreshToken;
		std::string m_AccessToken;
		JsonProcessor m_JsonProcessor;
	};
}