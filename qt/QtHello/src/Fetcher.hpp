#pragma once

#include <memory>
#include <vector>
#include "Chunk.hpp"
#include <curl/curl.h>
#include <unordered_map>
#include <string>
#include <initializer_list>

namespace QtSample
{
    struct HttpResponse
    {
        uint32_t Code;
        std::shared_ptr<RawBuffer> Body;
        std::string MimeType;

        HttpResponse() : Code(0), MimeType("") {
            Body.reset(new RawBuffer());
        };
    };

    class Fetcher
    {
    public:
        Fetcher();
        ~Fetcher();        

        std::shared_ptr<QtSample::HttpResponse> Fetch(std::string_view url, std::string_view method = "GET", std::string_view body = "", const std::vector<std::pair<std::string, std::string>>& headers = {});
        std::shared_ptr<QtSample::HttpResponse> ProtectedFetch(std::string_view url, std::string_view accessToken, std::string_view method = "GET", std::string_view body = "", const std::vector<std::pair<std::string, std::string>>& headers = {});
        

        std::string ReturnMime();
        std::string ReturnMappedMime(std::string_view mime);
        
    private:
        void PackMemoryChunks(std::shared_ptr<QtSample::RawBuffer>* out_buffer);
        void ClearChunks();
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

        std::vector<std::shared_ptr<QtSample::Chunk>> m_Response;
        static const std::unordered_map<std::string, std::string> s_MimeExtensionMap;

        size_t m_CurrentChunk;
        CURL* m_CurlController;
        CURLcode m_ResponseCode;
    };
}
