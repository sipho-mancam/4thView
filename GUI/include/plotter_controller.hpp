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
	PlayerItemWidget* placeKicker();
	void deletePlayer(int id);
	void sceneRectChangedSlot(const QRect& sceneRect);
	void clearAllPlotted();
	void kickerRejected(int id);

	int getKickersId() { return kickersId; }
	
signals:
	void playerCreated(PlayerItemWidget* player);
	void playerDeleted(int id);
	void clearAllPlottedPlayersSig();
	void playerCreatedSig(int trackId, std::vector<double>coordinates);
	void placeKicker(int trackId, QPointF coordinates);
	void kickerRejectedSig(int id);

private:
	long playerIds;
	std::vector<PlayerItemWidget*> createdPlayers;
	std::map<int, PlayerItemWidget*> kickersMap;
	QRect sceneRect;
	float centerX, centerY;
	int kickersId;
	PlayerItemWidget* kickerItem;
};