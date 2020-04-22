#include "johns_hopkins.h"
#include "data_parser.h"
#include <iostream>
#include <curl/curl.h>

struct memoryblob {
    char *memory;
    size_t size;
};

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memoryblob *mem = (struct memoryblob *)userp;

    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == nullptr) {
        std::cerr << "not enough memory (realloc returned NULL)" << std::endl;
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

std::string download_covid_19_data(const std::string& url) {
    std::string data;
    if (!curl_global_init(CURL_GLOBAL_DEFAULT)) {
        CURLcode res;
        CURL* h{curl_easy_init()};
        struct memoryblob chunk;
        chunk.memory = static_cast<char *>(malloc(1));
        chunk.size = 0;
        curl_easy_setopt(h, CURLOPT_URL, url.c_str());
        curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(h, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(h, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        res = curl_easy_perform(h);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            data.append(chunk.memory);
        }
        curl_easy_cleanup(h);
        curl_global_cleanup();
    }
    return data;
}

std::string john_hopkins_data_to_file(const std::string& src_file, const std::string& filename) {
    const auto csvdata{download_covid_19_data(src_file)};
    std::string delimitedData {re_delimit_to_pipes(csvdata)};

    std::ofstream file(filename);
    file << delimitedData;
    return delimitedData;
}
