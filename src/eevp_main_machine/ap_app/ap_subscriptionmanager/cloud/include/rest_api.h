#ifndef __SUBSCRIPTIONMANAGER_REST_API_H__
#define __SUBSCRIPTIONMANAGER_REST_API_H__

#include <curl/curl.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <unistd.h>

class RestApi {
public :
    RestApi();
    ~RestApi();

    bool init();
    bool init(std::string userid, std::string password);

    void clear();
    void set_debug_print();
    void set_delete();
    bool get_delete();

    void set_header_content(std::string _content, std::string _info);
    void set_server_info(std::string target_url);

    int get_request(std::string& url, std::string *response);
    int post_request(std::string& url, std::string& body, std::string *response);
    int download_file(std::string& url, std::string& filename, std::string *response);

private :
    bool is_init;
    bool is_set_url;
    bool is_delete;
    bool is_debug_print;

    CURL *api_handle;
    CURLcode curl_ret_code;
    std::string read_buffer;
    std::string url;

    struct curl_slist *headers;

    static size_t write_callback(void *_contents, size_t _size, size_t _nmemb, void *_userp);
    static size_t write_file_callback(void *_contents, size_t _size, size_t _nmemb, FILE *_stream);

    void set_header(std::string& header);
    void set_url(std::string& url);

    int request();
    std::string response();
};

#endif /// __SUBSCRIPTIONMANAGER_REST_API_H__
