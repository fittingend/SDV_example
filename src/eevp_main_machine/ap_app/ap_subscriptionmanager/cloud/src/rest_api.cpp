#include "rest_api.h"

RestApi::RestApi() {
    this->is_debug_print = false;
    this->is_delete = false;

    if (this->is_debug_print) {
        this->set_debug_print();
    }

    this->api_handle = NULL;
    this->clear();

    return;
}

RestApi::~RestApi() {
    if (this->api_handle != NULL) {
        curl_easy_cleanup(this->api_handle);
        this->api_handle = NULL;
    }

    return;
}

bool
RestApi::init() {
    if ((this->api_handle = curl_easy_init())) {
        this->is_init = true;
    } else {
        this->is_init = false;
    }

    if (this->is_debug_print) {
        curl_easy_setopt(this->api_handle, CURLOPT_VERBOSE, true);
    }

    return this->is_init;
}

bool
RestApi::init(std::string userid, std::string password) {
    if ((this->api_handle = curl_easy_init())) {
        this->is_init = true;
    } else {
        this->is_init = false;
    }

    curl_easy_setopt(this->api_handle, CURLOPT_USERNAME, userid.c_str());
    curl_easy_setopt(this->api_handle, CURLOPT_PASSWORD, password.c_str());

    if (this->is_debug_print) {
        curl_easy_setopt(this->api_handle, CURLOPT_VERBOSE, true);
    }

    return this->is_init;
}

void
RestApi::clear() {
    this->headers       = NULL;

    this->read_buffer   = "";
    this->url           = "";

    this->is_init       = false;
    this->is_set_url    = false;

    if (this->api_handle != NULL ) {
        curl_easy_cleanup(this->api_handle);
        this->api_handle = NULL;
    }

    return;
}

void
RestApi::set_debug_print() {
    this->is_debug_print = true;
    return;
}

void
RestApi::set_delete() {
    if (!this->is_init) {
        return;
    }

    this->is_delete = true;

    return;
}

bool
RestApi::get_delete() {
    return this->is_delete;
}

void
RestApi::set_header_content(std::string _content, std::string _info) {
    std::string content = _content;
    content.append(": ");
    content.append(_info);

    this->set_header(content);

    return;
}

void
RestApi::set_server_info(std::string target_url) {
    this->set_url(target_url);

    return;
}

int
RestApi::get_request(std::string& url, std::string *response) {
    this->set_server_info(url);

    curl_easy_setopt(this->api_handle, CURLOPT_WRITEFUNCTION,   this->write_callback);
    curl_easy_setopt(this->api_handle, CURLOPT_WRITEDATA,      &this->read_buffer);
    curl_easy_setopt(this->api_handle, CURLOPT_HTTPHEADER,      this->headers);

    this->request();
    *response = this->response();

    return (int)this->curl_ret_code;
}

int
RestApi::post_request(std::string& url, std::string& body, std::string *response) {
    this->set_server_info(url);

    curl_easy_setopt(this->api_handle, CURLOPT_WRITEFUNCTION,   this->write_callback);
    curl_easy_setopt(this->api_handle, CURLOPT_WRITEDATA,      &this->read_buffer);
    curl_easy_setopt(this->api_handle, CURLOPT_HTTPHEADER,      this->headers);

    curl_easy_setopt(this->api_handle, CURLOPT_POST,            1L);
    curl_easy_setopt(this->api_handle, CURLOPT_POSTFIELDS,      body.c_str());
    curl_easy_setopt(this->api_handle, CURLOPT_POSTFIELDSIZE,   body.size());

    this->request();
    *response = this->response();

    return (int)this->curl_ret_code;
}

int
RestApi::download_file(std::string& url, std::string& filename, std::string *response) {
    FILE *fp;

    this->set_server_info(url);

    fp = fopen(filename.c_str(), "wb");
    curl_easy_setopt(this->api_handle, CURLOPT_WRITEFUNCTION,   this->write_file_callback);
    curl_easy_setopt(this->api_handle, CURLOPT_WRITEDATA,       fp);

    this->request();
    *response = this->response();

    fclose(fp);

    sync();
    sync();
    sync();

    return (int)this->curl_ret_code;
}

size_t
RestApi::write_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)ptr, size *nmemb);

    return size * nmemb;
}

size_t
RestApi::write_file_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);

    return written;
}

void
RestApi::set_header(std::string& header) {
    if (!this->is_init) {
        return;
    }

    this->headers = curl_slist_append(this->headers, header.c_str());

    return;
}

void
RestApi::set_url(std::string& url) {
    if (!this->is_init) {
        return;
    }

    this->is_set_url = true;
    this->url = url;

    return;
}

int
RestApi::request() {
    if (!this->is_init) {
        return -1;
    }

    if (!this->is_set_url) {
        return -1;
    }

    curl_easy_setopt(this->api_handle, CURLOPT_URL, this->url.c_str());

    this->read_buffer   = "";
    this->curl_ret_code = curl_easy_perform(this->api_handle);

    if (this->curl_ret_code != CURLE_OK) {
        this->read_buffer = std::string(curl_easy_strerror(this->curl_ret_code));
    }

    return (int)this->curl_ret_code;
}

std::string
RestApi::response() {
    return this->read_buffer;
}
