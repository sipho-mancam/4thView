#include "models/distance_object_model.hpp"
#include <iostream>

DistanceObjectModel::DistanceObjectModel(QObject* parent)
	:QObject(parent),
	idCount(0)
{
}

void DistanceObjectModel::updatePreviewData(json previewData)
{
}

void DistanceObjectModel::addDistanceObject(json distanceObject)
{
	long long id = idCount++;
	distanceObject["objectId"] = id;
	distanceObjects[id] = distanceObject;
	emit distanceObjectsUpdatedSig(distanceObject);
	// Send an Event to the DA to tell them there's a new distance object that needs to be computed
}

void DistanceObjectModel::deleteDistanceObject(long long id)
{
	if (distanceObjects.contains(id))
	{
		distanceObjects.erase(id);
	}
	// Send an Event to the DA to tell them distance with ID must be deleted
	Q_EMIT deleteDistanceObjectSig(id);
}

void DistanceObjectModel::updateFrameData(json frameData)
{
	if (frameData.contains("distance_objects"))
	{
		json distanceObjects = frameData["distance_objects"];
		Q_EMIT computedDistanceUpdate(distanceObjects);
	}
}
