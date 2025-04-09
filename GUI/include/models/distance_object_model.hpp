#pragma once
#include <QObject>
#include <nlohmann/json.hpp>


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

signals:
	void distanceObjectsUpdatedSig(json distanceObjects);
	void distancePreviewObjectReadySig(json distanceObject);
	void clearPreviewObject();

private:
	json distanceObjects;
	json previewData;
	json frameData;
	json distanceObject;

};