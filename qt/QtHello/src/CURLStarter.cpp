#include "CURLStarter.hpp"
#include <curl/curl.h>

size_t QtSample::CURLStarter::s_ChunkSize = 1024;
std::string QtSample::CURLStarter::s_CertificateLocation = "";

void QtSample::CURLStarter::InitCurl()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void QtSample::CURLStarter::ShutdownCurl()
{
    curl_global_cleanup();
}

void QtSample::CURLStarter::SetCurlChunkSize(size_t size)
{
    s_ChunkSize = size;
}

void QtSample::CURLStarter::SetCertificateLocation(std::string location)
{
    s_CertificateLocation = location;
}

size_t QtSample::CURLStarter::GetCurlChunkSize()
{
    return s_ChunkSize;
}

std::string QtSample::CURLStarter::GetCertificateLocation()
{
    return s_CertificateLocation;
}
