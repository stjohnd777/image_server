#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>

#include <nlohmann/json.hpp>
#include "rest_server.h"

 

#include <thread>


using namespace std;
 


 
 
RestServer* RestServer::getInstance() {
	static RestServer* INSTANCE = new RestServer();
	return INSTANCE;
}

RestServer::RestServer() {

	CROW_ROUTE(app, "/phact") ( []() {return "alive"; });

	// get all tracks
	std::function<string()>  tracksProvider = [&]() {
		std::stringstream ss;
		for (auto aTrack : GlobalData::tracks) {
			ss << aTrack->toCSV() << endl;
		}
		// Remove all stored elements from the circular_buffer.
		// GlobalData::tracks.clear();
		return ss.str();
		};

	CROW_ROUTE(app, "/tracks/") (tracksProvider);


	// all tracks
	CROW_ROUTE(app, "/tracks/last/<int>")
		([&](int lastN) {
		std::stringstream ss;
		int counter = 0;
		auto it = GlobalData::tracks.rbegin();
		while (it != GlobalData::tracks.rend()){
			ss << (*it)->toCSV() << endl;
			it++;
			counter++;
			if (counter == lastN) {
				break;
			}
		}
		return ss.str();
	});

	// all tracks in buffer with provided trackId (birthTime,idx)
	CROW_ROUTE(app, "/tracks/birthtime/<double>/idx/<int>")
		([&](double birthTime, int idx) {
				// filter on trackId
			std::vector<std::shared_ptr<TrackDTO>> tracks;
			std::copy_if(begin(GlobalData::tracks), end(GlobalData::tracks),std::back_inserter(tracks),
				[&](std::shared_ptr<TrackDTO> spDto) {
					return spDto->getBirthID() == idx && spDto->getBirthTime() == birthTime;
				});
			// return csv
			std::stringstream ss;
			for (auto aTrack : tracks) {
				ss << aTrack->toCSV() << endl;
			}
			return ss.str();
		});

}

std::thread* RestServer::start() {
	auto t = new thread([&]() {
		app.port(port).multithreaded().run();
		});
	t->detach();
	return t;
	//app.port(18080).multithreaded().run();
}

RestServer::~RestServer()
{
}

 

