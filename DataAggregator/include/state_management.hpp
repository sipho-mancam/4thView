#pragma once
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class StateManager
{
public:
	
	StateManager();

	void addDistance(int distanceID, json distanceData);
	void addAnnotation(int playerID, json annotationData);
	void addHighlight(int playerID, json highlightData);
	void addPosition(int playerID, json playerPosition);
	void updatePlottedPlayers(json eventData);

	json updateTrackData(json track);
	void updateState(json stateInfo);

private:
	std::map<int, json> distances;
	std::map<int, json> playerAnnotions;
	std::map<int, json> playerHighlight;
	std::map<int, json> playerPositions;
	std::map<int, int> teamAssignment;

	std::vector<json> plottedPlayers;
	json kickerObject;
	double fieldWidth, fieldHeight;

	void __addStateInfo__(std::map<int, json>& _map, int id, json& data);
};