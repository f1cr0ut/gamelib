#include<curl/curl.h>
#include"net.h"

using namespace core;

namespace {
	struct internal_net final {

		internal_net() {
			curl_global_init(CURL_GLOBAL_DEFAULT);
		}

		~internal_net() {
			curl_global_cleanup();
		}
	};
	static internal_net handle;


	size_t callback(void *contents, size_t size, size_t nmemb, void *userp){
		size_t realsize = size * nmemb;
		std::string * s = (std::string*)userp;
		s->append((char*)contents);
		return realsize;
	}
}

struct net::Impl {
	CURL *curl { nullptr };
	struct curl_slist *cookies { nullptr };
};

net::net() {
	auto p = std::unique_ptr<Impl>(new Impl);
	std::swap(p, impl);
	curl_global_init(CURL_GLOBAL_DEFAULT);
	impl->curl = curl_easy_init();
}

net::~net() {
	curl_easy_cleanup(impl->curl);
}

const std::string & net::get_response() const {
	return response;
}

void net::https_get(const char * url) {
	if( ! url) {
		return;
	}
	curl_easy_setopt(impl->curl, CURLOPT_URL, url);
	curl_easy_setopt(impl->curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(impl->curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(impl->curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(impl->curl, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(impl->curl, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_setopt(impl->curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_perform(impl->curl);
}

void net::https_post(const char * url, const char * data) {
	if( ! url || ! data) {
		return;
	}
	curl_easy_setopt(impl->curl, CURLOPT_URL, url);
	curl_easy_setopt(impl->curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(impl->curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(impl->curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(impl->curl, CURLOPT_ACCEPT_ENCODING, "");
	curl_easy_setopt(impl->curl, CURLOPT_WRITEFUNCTION, callback);
	curl_easy_setopt(impl->curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(impl->curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(impl->curl, CURLOPT_POSTFIELDSIZE, (long)strlen(data));
	curl_easy_perform(impl->curl);
}

void net::set_cookie(const char * str) {
	curl_easy_setopt(impl->curl, CURLOPT_COOKIE, str);
}

const std::string & net::response_cookies() {
	curl_easy_getinfo(impl->curl, CURLINFO_COOKIELIST, &impl->cookies);
	auto nc = impl->cookies;
	for (int i = 1; nc; ++i) {
		cookie += nc->data;
		nc = nc->next;
	}
	curl_slist_free_all(impl->cookies);
	return cookie;
}

void net::clear_cookie() {
	cookie.clear();
}

void net::clear_response() {
	response.clear();
}
