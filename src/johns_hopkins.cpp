#include "johns_hopkins.h"
#include "data_parser.h"

#include <curl/curl.h>
#include <iostream>

struct memoryblob {
    char *memory;
    size_t size;
};

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memoryblob *mem = (struct memoryblob *) userp;

    char *ptr = (char *) realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == nullptr) {
        std::cerr << "not enough memory (realloc returned NULL)" << std::endl;
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

std::string download_covid_19_data(const std::string &url) {
    std::string data;
    if (!curl_global_init(CURL_GLOBAL_DEFAULT)) {
        CURLcode res;
        CURL *h{curl_easy_init()};
        struct memoryblob chunk;
        chunk.memory = static_cast<char *>(malloc(1));
        chunk.size = 0;
        curl_easy_setopt(h, CURLOPT_URL, url.c_str());
        curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(h, CURLOPT_WRITEDATA, (void *) &chunk);
        curl_easy_setopt(h, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        res = curl_easy_perform(h);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            data.append(chunk.memory);
        }
        curl_easy_cleanup(h);
        curl_global_cleanup();
    }
    return data;
}

std::string john_hopkins_data_to_file(const std::string &src_file, const std::string &filename) {
    const auto csvdata{download_covid_19_data(src_file)};
    std::string delimitedData{re_delimit_to_pipes(csvdata)};

    std::ofstream file(filename);
    file << delimitedData;
    return delimitedData;
}

void
update_from_john_hopkins_if_required(const std::filesystem::path &src_url, const std::filesystem::path &trg_path,
                                     const std::vector<std::string> &src_files) {
    std::for_each(src_files.begin(), src_files.end(), [src_url, trg_path](const auto &file_name) {
        std::filesystem::path source{src_url};
        std::filesystem::path target{trg_path};
        source.append(file_name);
        target.append(file_name);

        if (std::filesystem::exists(target)) {
            std::filesystem::file_time_type file_time{last_write_time(target)};
            double difference_sec = difftime(time(nullptr),
                                             decltype(file_time)::clock::to_time_t(file_time)); //seconds to day
            if (difference_sec < (24 * 3600)) {
                return;
            }
            std::cerr << "Data is over one day old, reloading from John Hopkins." << std::endl;
        } else {
            std::cerr << "Downloading " << file_name << " from John Hopkins." << std::endl;
        }
        john_hopkins_data_to_file(source.string(), target.string());
    });
}
