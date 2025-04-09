#pragma once
#include "ui_distance_widget.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
};