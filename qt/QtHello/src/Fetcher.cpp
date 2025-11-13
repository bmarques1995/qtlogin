#include "Fetcher.hpp"
#include "Fetcher.hpp"
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include "CURLStarter.hpp"

struct DownloaderInfo{
    size_t* CurrentChunk;
    std::vector<std::shared_ptr<QtSample::Chunk>>* Chunks;
};

const std::unordered_map<std::string, std::string> QtSample::Fetcher::s_MimeExtensionMap = {
    {"application/json", "json"},
    {"application/json; charset=utf-8", "json"},
    {"text/html", "html"},
    {"text/plain", "txt"},
    {"image/png", "png"},
    {"image/jpeg", "jpg"},
};

QtSample::Fetcher::Fetcher()
{
    m_CurlController = curl_easy_init();
    if(!m_CurlController)
        throw std::runtime_error("Failed to initialize downloader");
}

QtSample::Fetcher::~Fetcher()
{
    curl_easy_cleanup(m_CurlController);
}

std::shared_ptr<QtSample::HttpResponse> QtSample::Fetcher::Fetch(std::string_view url, std::string_view method, std::string_view body, const std::vector<std::pair<std::string, std::string>>& headers)
{

    std::shared_ptr<HttpResponse> bufferResponse;
    bufferResponse.reset(new HttpResponse());
    DownloaderInfo info;
    info.CurrentChunk = &m_CurrentChunk;
    info.Chunks = &m_Response;

    curl_easy_setopt(m_CurlController, CURLOPT_URL, url.data());
    curl_easy_setopt(m_CurlController, CURLOPT_CUSTOMREQUEST, method.data());
    curl_easy_setopt(m_CurlController, CURLOPT_POSTFIELDS, body.data());
    // Build curl_slist
    curl_slist* slist = nullptr;
    for (auto& [key, value] : headers) {
        std::string full = key + ": " + value;
        slist = curl_slist_append(slist, full.c_str());
    }

    curl_easy_setopt(m_CurlController, CURLOPT_HTTPHEADER, slist);
#ifndef WIN32
    curl_easy_setopt(m_CurlController, CURLOPT_CAINFO, CURLStarter::GetCertificateLocation().c_str());
#endif
    curl_easy_setopt(m_CurlController, CURLOPT_WRITEFUNCTION, Fetcher::WriteCallback);
    curl_easy_setopt(m_CurlController, CURLOPT_WRITEDATA, &info);
    
    // Perform the request
    m_ResponseCode = curl_easy_perform(m_CurlController);

    if (m_ResponseCode != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: "
            << curl_easy_strerror(m_ResponseCode) << std::endl;
    }

    // Check HTTP status code
    long http_code = 0;
    curl_easy_getinfo(m_CurlController, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        std::cerr << "Unexpected HTTP status: " << http_code << "\n";
        curl_slist_free_all(slist);
        curl_easy_cleanup(m_CurlController);
        return bufferResponse;
    }

    // Check MIME type
    curl_header* type;
    CURLHcode h;
    h = curl_easy_header(m_CurlController, "Content-Type", 0, CURLH_HEADER, -1, &type);
    bufferResponse->MimeType = type->value;
    bufferResponse->Code = http_code;

    curl_easy_cleanup(m_CurlController);

    PackMemoryChunks(&(bufferResponse->Body));
    curl_slist_free_all(slist);
    ClearChunks();
    
    return bufferResponse;
}

std::shared_ptr<QtSample::HttpResponse> QtSample::Fetcher::ProtectedFetch(std::string_view url, std::string_view accessToken, std::string_view method, std::string_view body, const std::vector<std::pair<std::string, std::string>>& headers)
{
    std::vector<std::pair<std::string, std::string>> allHeaders(headers);

    std::string urlString = url.data();
    std::stringstream accessTokenStream;
	accessTokenStream << "Bearer " << accessToken;
    std::string treatedToken = accessTokenStream.str();
    accessTokenStream.clear();
    allHeaders.push_back({"Authorization", treatedToken });

    return Fetch(urlString, method, body, allHeaders);
}

void QtSample::Fetcher::PackMemoryChunks(std::shared_ptr<QtSample::RawBuffer>* out_buffer)
{
    if (m_Response.empty() || out_buffer == nullptr)
        return;

    size_t total_size = 0;
    for (const auto& chunk : m_Response)
    {
        total_size += chunk->GetSize();
    }

    uint8_t* combined_data = new uint8_t[total_size];
    size_t offset = 0;
    for (const auto& chunk : m_Response)
    {
        memcpy(combined_data + offset, chunk->GetData(), chunk->GetSize());
        offset += chunk->GetSize();
    }

    (*out_buffer)->RecreateBuffer(combined_data, total_size);
    delete[] combined_data;
}

std::string QtSample::Fetcher::ReturnMappedMime(std::string_view mime)
{
    auto it = s_MimeExtensionMap.find(mime.data());
    if (it != s_MimeExtensionMap.end()) {
        return it->second;
    }
    return "";
}

void QtSample::Fetcher::ClearChunks()
{
    m_Response.clear();
    m_CurrentChunk = 0;
}

size_t QtSample::Fetcher::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t bufferSize = size * nmemb;
    std::shared_ptr<QtSample::Chunk> chunk;
    DownloaderInfo* buffer = static_cast<DownloaderInfo*>(userp);
    chunk.reset(new QtSample::Chunk((*buffer->CurrentChunk)));
    chunk->RecreateBuffer((uint8_t*)contents, bufferSize);
    buffer->Chunks->push_back(chunk);
    (*(buffer->CurrentChunk))++;
    return bufferSize;
}
