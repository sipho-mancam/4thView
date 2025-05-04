#pragma once
#include "ui_distance_widget.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum E_DISTANCE_TYPES
{
	DYNAMIC_DISTANCE = 0,// dynamic distance depends on the players positions, it is a distance line between two players
	FIXED_DISTANCE = 1 // Fixed distance is a distance line between two arbitrary points in the scene, it doesn't depend on the players positions
};

class DistanceDialog : public QDialog
{
	Q_OBJECT
public:
	explicit DistanceDialog(QWidget* parent = nullptr);
	~DistanceDialog();

	void dialogAccepted();
	void dialogRejected();
	void resetState();
	virtual void showEvent(QShowEvent* evnt);
	void setDistanceType(E_DISTANCE_TYPES type) { currentDistanceType = type; }

public slots:
	void selectedPlayer(int id);
	

signals:
	void distanceData(json data);
	void previewDistanceDataReady(json data);
	void clearDistancePreview();

private:
	Ui::DistanceD* ui;
	int currentPlayerId;
	int player1Id, player2Id;
	QColor lineColor;
	QGraphicsEllipseItem* player1Widget, *player2Widget;
	QGraphicsTextItem* player1Text, * player2Text;
	E_DISTANCE_TYPES currentDistanceType;
};