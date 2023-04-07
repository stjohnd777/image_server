#pragma once

#include "crow.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>

using namespace std;
using namespace crow;

class WebServer
{

public:
	enum Metod
	{
		GET,
		POST,
		PUT,
		PATCH,
		DEL
	};

	WebServer(int port, std::string publicDir) : port(port), PUBLIC_HTML_DIR(publicDir)
	{
		std::stringstream ss;
		ss << "http://localhost:" << port << "/";
		addWebRoutes();
	}

	std::thread *start()
	{
		auto t = new thread([&]()
							{ app.port(port).multithreaded().run(); });
		t->detach();
		return t;
	}

	void addWebRoutes()
	{

		CROW_ROUTE(app, "/")
		([&](const request &req, response &res)
		 { sendHTML(res, "index"); });

		// Route param
		CROW_ROUTE(app, "/css/<string>")
		([&](const request &req, response &res, string filename)
		 { sendCSS(res, filename); });

		CROW_ROUTE(app, "/js/<string>")
		([&](const request &req, response &res, string filename)
		 { sendScript(res, filename); });

		CROW_ROUTE(app, "/image/<string>")
		([&](const request &req, response &res, string filename)
		 { sendImage(res, filename); });

		CROW_ROUTE(app, "/json/<string>")
		([&](const request &req, response &res, string filename)
		 { sendJSON(res, filename); });
	}

protected:

	void sendFile(response &res, string relativePath, string contentType)
	{

		// TODO,
		std::string path = PUBLIC_HTML_DIR + "\\" + relativePath;

		ifstream in(path, ifstream::in);
		if (in)
		{
			stringstream contents;
			contents << in.rdbuf();
			in.close();
			res.write(contents.str());

			res.set_header("Content-Type", contentType);
			res.code = 200;
		}
		else
		{
			res.write("404 file not found");
			res.code = 404;
			res.write("404 NOT FOUND");
		}
		res.end();
	}

	/*
	 * read file contents from file:path
	 */
	std::string ReadTextFile(std::string path)
	{
		stringstream contents;

		ifstream in(path);
		if (in)
		{
			contents << in.rdbuf();
			in.close();
		}
		return contents.str();
	}

	/*
	 * if string ends str end with suffix return true otherwise false
	 */
	static bool endsWith(const std::string &str, const std::string &suffix)
	{
		return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
	}

	void sendCSS(response &res, string relativePath)
	{
		sendFile(res, "css/" + relativePath, "text/css");
	}

	void sendHTML(response &res, string relativePath)
	{
		sendFile(res, relativePath + ".html", "text/html");
	}

	void sendImage(response &res, string relativePath)
	{
		sendFile(res, "image/" + relativePath, "image/jpeg");
	}

	void sendScript(response &res, string relativePath)
	{
		sendFile(res, "js/" + relativePath, "text/javascript");
	}

	void sendJSON(response &res, string relativePath)
	{
		sendFile(res, "json/" + relativePath, "text/json");
	}

	void useExtension(response &res, string relativePath)
	{
		if (endsWith(relativePath, ".js"))
		{
			sendScript(res, relativePath);
			return;
		}
		if (endsWith(relativePath, ".css"))
		{
			sendCSS(res, relativePath);
			return;
		}
		if (endsWith(relativePath, ".html"))
		{
			sendHTML(res, relativePath);
			return;
		}
		if (endsWith(relativePath, ".jpeg"))
		{
			sendImage(res, relativePath);
			return;
		}
		sendFile(res, relativePath, "");
	}

private:
	crow::SimpleApp app;
	int port = 8080;
	std::string PUBLIC_HTML_DIR;
};
