#pragma once
using namespace std;

#include "crow.h"
#include <thread>

const int CREATED = 201;
const int BAD_REQUEST = 400;

enum Method
{
	GET,
	POST,
	PUT,
	PATCH,
	DEL
};

template <typename F>
class ep
{
public:
	ep(std::string uri, Method m, F p) : uri(uri), method(m), provider(p) {}
	std::string uri;
	Method method;
	F provider;
};

class RestServer
{
public:
	static RestServer *getInstance();

	template <typename Func>
	void addRoute(ep<Func> ep)
	{
		CROW_ROUTE(app, ep.uri.c_str())
		(ep.provider);
	}

	std::thread *start();

	~RestServer();

private:
	int port = 18080;
	RestServer();
	crow::SimpleApp app;
};
