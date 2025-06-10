#include "state_management.hpp"
#include "types.hpp"



StateManager::StateManager()
	:fieldWidth(0),
	fieldHeight(0)
{
	json config = KEvents::__load_config__();
	fieldWidth = config["systemSettings"]["fieldDimensions"]["width"];
	fieldHeight = config["systemSettings"]["fieldDimensions"]["height"];
}

void StateManager::addDistance(int distanceID, json distanceData)
{
	__addStateInfo__(distances, distanceID, distanceData);
}

void StateManager::addAnnotation(int playerID, json annotationData)
{
	__addStateInfo__(playerAnnotions, playerID, annotationData);
}

void StateManager::addHighlight(int playerID, json highlightData)
{
	__addStateInfo__(playerHighlight, playerID, highlightData);
}

void StateManager::addPosition(int playerID, json playerPosition)
{
	__addStateInfo__(playerPositions, playerID, playerPosition);
}

void StateManager::updatePlottedPlayers(json payload)
{
	std::string instruction = payload["instruction"];
	// 1. We are creating a new player
	if (instruction == PI_ADD_PLAYER)
	{
		json playerData = payload["data"];
		plottedPlayers.push_back(playerData);
	}
	else if (instruction == PI_UPDATE_PLAYER)
	{
		// 2. We are modifying an existing player (Position)
		int trackId = payload["data"]["track_id"];
		std::vector<double> coordinates = payload["data"]["coordinates"];

		if (!kickerObject.empty() && kickerObject["id"] == trackId)
		{
			kickerObject["coordinates"] = coordinates;
			std::cout << "Kicker Coordinates updated: " << kickerObject << std::endl;
			return;
		}
		
		for (auto& player : plottedPlayers)
		{
			if (player["track_id"] == trackId)
			{
				player["coordinates"] = coordinates;
				break;
			}
		}
	}
	else if (instruction == PI_REMOVE_PLAYER)
	{
		// 3. We are removing a player
		int trackId = payload["track_id"];
		if (playerPositions.contains(trackId))
			playerPositions.erase(trackId);
		if (playerAnnotions.contains(trackId))
			playerAnnotions.erase(trackId);
		if (playerHighlight.contains(trackId))
			playerHighlight.erase(trackId);
	}
	else if (instruction == PI_CLEAR_ALL_PLAYERS)
	{
		// 4. We are clearing all players
		for (auto& player : plottedPlayers)
		{
			int trackId = player["track_id"];
			if (playerPositions.contains(trackId))
				playerPositions.erase(trackId);
			if (playerAnnotions.contains(trackId))
				playerAnnotions.erase(trackId);
			if (playerHighlight.contains(trackId))
				playerHighlight.erase(trackId);
		}
		plottedPlayers.clear();
		std::cout << "Cleared all players" << std::endl;
		kickerObject = json();
	}
	else if (instruction == PI_PLACE_KICKER)
	{
		int k_id = payload["data"]["kicker_id"];
		kickerObject["id"] = k_id;
		kickerObject["coordinates"] = payload["data"]["kicker_coordinates"];
		kickerObject["side_coordinates"] = payload["data"]["side_coordinates"];
		std::cout << "Placed kicker: " << kickerObject << std::endl;
	}
}

json StateManager::updateTrackData(json frame)
{
	if (frame["stream_type"] == KEvents::STREAM_TYPES::TRACKER)
	{
		json& tracks = frame["tracks"];

		// 1. Append Plotted Players
		for (auto& player : plottedPlayers)
		{
			tracks.push_back(player);
		}
		std::vector<json> processedTracks;
		for (auto& track : tracks)
		{
			try {
				int trackId = track["track_id"];
				if (playerHighlight.contains(trackId))
				{
					track["highlight"] = true;
					track["state"] = KEvents::STATES_DEF::HIGHLIGHT;
				}

				if (playerAnnotions.contains(trackId))
				{
					track["annotation"] = playerAnnotions[trackId];
					track["state"] = KEvents::STATES_DEF::ANNOTATION;
				}

				if (playerPositions.contains(trackId))
				{
					track["position"] = playerPositions[trackId];
				}
				if (teamAssignment.contains(trackId))
				{
					track["mode"] = teamAssignment[trackId];
				}
				else {
					json pos;
					pos["position"] = KEvents::EPLAYER_POSITIONS::FIELDER;
					pos["set"] = true;
					pos["activate"] = true;
					track["position"] = pos;
				}

				processedTracks.push_back(track);
			}
			catch (json::type_error& te) {
				
			}
			catch (std::exception& ex) {

			}	
		}
		frame["tracks"] = processedTracks;

		//4. Append Distance information
		if (!distances.empty())
		{
			std::vector<json> computedDistances;
			for (auto dist : distances)
			{
				json distData = dist.second;
				int distId = dist.first;
				int p1, p2;
				p1 = distData["player1"];
				p2 = distData["player2"];
				bool p1Found = false, p2Found = false;
				json t1, t2;
				for (auto& track : processedTracks)
				{
					if (track["track_id"] == p1)
					{
						p1Found = true;
						t1 = track;
					}
					else if (track["track_id"] == p2)
					{
						p2Found = true;
						t2 = track;
					}
					if (p1Found && p2Found)
						break;
				}
				if (p1Found && p2Found)
				{
					std::vector<double> coord1 = t1["coordinates"];
					std::vector<double> coord2 = t2["coordinates"];
					double dist = sqrt(pow((coord1[0] - coord2[0])*fieldWidth, 2) + pow((coord1[1] - coord2[1])*fieldHeight, 2));
					distData["distance"] = dist;
					distData["objectId"] = distId;
					computedDistances.push_back(distData);
				}
			}
			if (!computedDistances.empty())
			{
				frame["distance_objects"] = computedDistances;
			}	
		}
		/* Add Free-Kick Kicker information*/
		if (!kickerObject.empty())
		{
			std::vector<double> coordinates = kickerObject["coordinates"];
			std::vector<double> sideCoordinates = kickerObject["side_coordinates"];
			double dist = sqrt(pow((coordinates[0] - sideCoordinates[0]) * fieldWidth, 2) + pow((coordinates[1] - sideCoordinates[1]) * fieldHeight, 2));
			kickerObject["distance"] = dist;
		}

		frame["kicker"] = kickerObject;
	}
	
	return frame;
}

void StateManager::updateState(json stateInfo)
{
	//std::cout << stateInfo << std::endl;
	if (stateInfo["state_def"] == KEvents::STATES_DEF::ANNOTATION)
	{
		json data = stateInfo["data"];
		addAnnotation(data["id"], data);
		std::cout << "Added Annotation: " << data << std::endl;
 	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::HIGHLIGHT)
	{
		addHighlight(stateInfo["data"]["id"], stateInfo["data"]);
		std::cout << "Modified highlight: " << stateInfo["data"] << std::endl;
	}
	else if(stateInfo["state_def"] == KEvents::STATES_DEF::POSITION)
	{
		addPosition(stateInfo["data"]["id"], stateInfo["data"]);
		std::cout << "Modified Position: " << stateInfo["data"] << std::endl;
	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::DISTANCE)
	{
		addDistance(stateInfo["data"]["id"], stateInfo["data"]);
		std::cout << "Modified Distance: " << stateInfo["data"] << std::endl;
	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::PLAYER) 
	{
		json data = stateInfo["data"];
		std::cout << data << std::endl;
		updatePlottedPlayers(data);
		std::cout << "Modified Player: " << data << std::endl;
	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::TEAM)
	{
		teamAssignment[stateInfo["data"]["id"]] = stateInfo["data"]["team"];
		std::cout << "Modifed Team Information: " << stateInfo["data"] << std::endl;
	}

	
}

void StateManager::__addStateInfo__(std::map<int, json>& _map, int id, json& data)
{
	//To insert or remove an object from the list, we check the set flag of that data
	std::cout << data << std::endl;
	if (data["set"]) // this instruction says we must add this
	{
		_map[id] = data;
		return;
	}
	// other wise remove the object from the map
	if(_map.contains(id))
		_map.erase(id);
}
