#include "rest_client_loc.h"

#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <thread>
#include <functional>
#include <thread>
#include <future>

using namespace std;

n

	static string s_data;

static size_t onDataCallback(char *buf, size_t size, size_t nmemb, void *up)
{
	cout << s_data << endl;
	for (int c = 0; c < size * nmemb; c++)
	{
		s_data.push_back(buf[c]);
	}
	return size * nmemb;
}

// Post
struct WriteThis
{
	const char *readptr;
	size_t sizeleft;
};
static size_t read_callback(char *dest, size_t size, size_t nmemb, void *userp)
{
	struct WriteThis *wt = (struct WriteThis *)userp;
	size_t buffer_size = size * nmemb;

	if (wt->sizeleft)
	{
		/* copy as much as possible from the source to the destination */
		size_t copy_this_much = wt->sizeleft;
		if (copy_this_much > buffer_size)
			copy_this_much = buffer_size;
		memcpy(dest, wt->readptr, copy_this_much);

		wt->readptr += copy_this_much;
		wt->sizeleft -= copy_this_much;
		return copy_this_much; /* we copied this many bytes */
	}

	return 0; /* no more data left to deliver */
}

RestClient::RestClient()
{
}

std::string RestClient::getSync(std::string uri)
{

	CURL *curl = curl_easy_init();
	validate(curl);
	curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);

	std::string data;
	std::mutex m_mutex;
	{
		// do the nature of one static external variable
		// this call will allow on one thread in the critical section
		std::lock_guard<std::mutex> guard(m_mutex);
		// perform the transfer using curl_easy_perform.
		// It will then do the entire operation and won't
		// return until it is done (successfully or not).
		CURLcode res = curl_easy_perform(curl);

		// TODO Check COULD NOT CONNECT

		validate(res);
		data = s_data;

		// After the transfer has been made, you can set new options and
		// make another transfer, or if you're done, cleanup the session by
		// calling curl_easy_cleanup. If you want persistent connections,
		// you don't cleanup immediately, but instead run ahead and perform other
		// transfers using the same easy handle.
		curl_easy_cleanup(curl);
		s_data.clear();
	}
	return data;
}

std::thread *RestClient::getAsync(std::string uri, function<void(string raw)> f)
{

	auto t = new thread([&]() -> void
						{

				std::thread::id this_id = std::this_thread::get_id();

				CURL* curl = curl_easy_init();
				validate(curl);
				curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [&](char* buf, size_t size, size_t nmemb, void* up) {
					cout << s_data << endl;
					for (int c = 0; c < size * nmemb; c++) {
						s_data.push_back(buf[c]);
					}
					return size * nmemb;
					});

				std::string data;
				std::mutex m_mutex;
				{
					std::lock_guard<std::mutex> guard(m_mutex);
					CURLcode res = curl_easy_perform(curl);
					validate(res);
					curl_easy_cleanup(curl);
					data = s_data;
					s_data.clear();
					m_mutex.unlock();
				}
				f(data);
				cout << "getAsync thread done " << this_id << endl; });

	t->detach();
	return t;
}

std::string RestClient::postSync(std::string uri, const char *payload)
{

	// struct WriteThis wt;
	// wt.readptr = payload;
	// wt.sizeleft = strlen(payload);

	CURL *curl = curl_easy_init();
	validate(curl);

	/* First set the URL that is about to receive our POST. */
	curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());

	/* Now specify we want to POST data */
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"hi\" : \"there\"}"); // payload);

	/* we want to use our own read function */
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

	// curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "text/plain");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen("{\"hi\" : \"there\"}")); // payload));

	// curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &onDataCallback);
	// curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);

	std::string data;
	std::mutex m_mutex;
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		CURLcode res = curl_easy_perform(curl);
		validate(res);
		curl_easy_cleanup(curl);
		data = s_data;
		s_data.clear();
	}
	return data;
}

std::thread *RestClient::postAsync(std::string uri, const char *payload, std::function<void(std::string)> f)
{

	auto t = new thread([this, uri, payload, f]() -> void
						{

				std::thread::id this_id = std::this_thread::get_id();

				CURL* curl = curl_easy_init();
				validate(curl);
				curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(payload));
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [&](char* buf, size_t size, size_t nmemb, void* up) {
					cout << s_data << endl;
					for (int c = 0; c < size * nmemb; c++) {
						s_data.push_back(buf[c]);
					}
					return size * nmemb;
					});
				std::string data;
				std::mutex m_mutex;
				{
					std::lock_guard<std::mutex> guard(m_mutex);
					CURLcode res = curl_easy_perform(curl);
					//validate(res);
					curl_easy_cleanup(curl);
					data = s_data;
					s_data.clear();
				}
				f(data);
				cout << "postAsync thread done " << this_id << endl; });

	t->detach();
	return t;
}

void RestClient::validate(CURL *curl)
{
	if (!curl)
	{
		throw new std::runtime_error("curl_easy_init() failed");
	}
}

void RestClient::validate(CURLcode ret)
{
	if (ret != CURLE_OK)
	{
		std::stringstream ss;
		ss << "curl_easy_perform() failed " << curl_easy_strerror(ret) << std::endl;
		std::cout << ss.str();
		throw new std::runtime_error(ss.str());
	}
}
