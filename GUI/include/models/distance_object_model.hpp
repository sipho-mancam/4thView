#pragma once
#include <QObject>
#include <nlohmann/json.hpp>
#include <map>

using json = nlohmann::json;

class DistanceObjectModel : public QObject
{
	Q_OBJECT
public:
	DistanceObjectModel(QObject* parent = nullptr);
	~DistanceObjectModel() = default;

public slots:
	void updateFrameData(json frameData);
	void updatePreviewData(json previewData);
	void addDistanceObject(json distanceObject);
	void deleteDistanceObject(long long id);

signals:
	void distanceObjectsUpdatedSig(json distanceObjects);
	void distancePreviewObjectReadySig(json distanceObject);
	void clearPreviewObject();

private:
	std::map<long long, json> distanceObjects;
	json previewData;
	json frameData;
	json distanceObject;
	long long idCount;

};