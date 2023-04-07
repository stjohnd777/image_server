#pragma once
// #include "../pch.h"
#include <curl/curl.h>

#include <string>
#include <functional>
#include <thread>
#include <future>
#include <chrono>

class RestClient
{
public:
	RestClient();

	std::string getSync(std::string uri);
	std::thread *getAsync(std::string uri, std::function<void(std::string)> f);

	std::string postSync(std::string uri, const char *payload);
	std::thread *postAsync(std::string uri, const char *payload, std::function<void(std::string)> f);

protected:
	void validate(CURL *curl);
	void validate(CURLcode ret);
};
