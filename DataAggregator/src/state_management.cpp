#include "state_management.hpp"
#include "types.hpp"

StateManager::StateManager()
{
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

json StateManager::updateTrackData(json frame)
{
	if (frame["stream_type"] == KEvents::STREAM_TYPES::TRACKER)
	{
		//1. Update all the objects that need to be highlighted

		//2. Update all the objects that have an annotation set

		//3. Update all players that have their positions set

		//4. Append Distance information
	}
	
	return frame;
}

void StateManager::updateState(json stateInfo)
{
	if (stateInfo["state_def"] == KEvents::STATES_DEF::ANNOTATION)
	{
		json data = stateInfo["data"];
		addAnnotation(data["id"], data);
	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::HIGHLIGHT)
	{
		addHighlight(stateInfo["data"]["id"], stateInfo["data"]);
	}
	else if(stateInfo["state_def"] == KEvents::STATES_DEF::POSITION)
	{
		addPosition(stateInfo["data"]["id"], stateInfo["data"]);

	}
	else if (stateInfo["state_def"] == KEvents::STATES_DEF::DISTANCE)
	{
		addPosition(stateInfo["data"]["id"], stateInfo["data"]);
	}
}

void StateManager::__addStateInfo__(std::map<int, json>& _map, int id, json& data)
{
	//To insert or remove an object from the list, we check the set flag of that data
	if (data["set"]) // this instruction says we must add this
	{
		_map[id] = data;
		return;
	}
	// other wise remove the object from the map
	if(_map.contains(id))
		_map.erase(id);
}
