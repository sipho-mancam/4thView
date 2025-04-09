#include "models/distance_object_model.hpp"

DistanceObjectModel::DistanceObjectModel(QObject* parent)
	:QObject(parent)
{
}

void DistanceObjectModel::updatePreviewData(json previewData)
{
}

void DistanceObjectModel::updateFrameData(json frameData)
{
	json distanceObjects = frameData["distance_objects"];
	
}
