#pragma once
#include <QObject>
#include <QRect>

class PlayerItemWidget;

class PlotterController : public QObject
{
	Q_OBJECT

public:
	PlotterController(const QRect& sceneRect, QObject* parent = nullptr);
	~PlotterController();

	PlayerItemWidget* createPlayerItem();
	void deletePlayer(int id);
	void sceneRectChangedSlot(const QRect& sceneRect);
	void clearAllPlotted();
	
signals:
	void playerCreated(PlayerItemWidget* player);
	void playerDeleted(int id);
	void clearAllPlottedPlayersSig();
	void playerCreatedSig(int trackId, std::vector<double>coordinates);

private:
	long playerIds;
	std::vector<PlayerItemWidget*> createdPlayers;
	QRect sceneRect;
	float centerX, centerY;
};