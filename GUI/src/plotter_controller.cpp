#include "plotter_controller.hpp"

#include "views/cricket_oval_widget.hpp"

PlotterController::PlotterController(const QRect& scRect, QObject* parent)
	:QObject(parent),
	playerIds(40),
	sceneRect(scRect),
	kickersId(-1)
{
	centerX = sceneRect.width() / 2;
	centerY = sceneRect.height() / 2;
}

PlotterController::~PlotterController()
{
	for (PlayerItemWidget* pl : createdPlayers)
	{
		delete pl;
	}
}

PlayerItemWidget* PlotterController::createPlayerItem()
{
	PlayerItemWidget* playerItem = new PlayerItemWidget(playerIds, { centerX, centerY }, PLAYER_TYPE::PLOTTED);
	createdPlayers.push_back(playerItem);
	playerIds += 1;
	Q_EMIT playerCreated(playerItem);
	Q_EMIT playerCreatedSig(playerItem->getTrackId(), { centerX, centerY });
	return playerItem;
}

PlayerItemWidget* PlotterController::placeKicker()
{
	if (kickersId != -1)
	{
		kickerRejected(kickersId);
	}

	PlayerItemWidget* playerItem = new PlayerItemWidget(playerIds, { centerX, centerY }, PLAYER_TYPE::PLOTTED);
	playerItem->setPlayerModifiers(PLAYER_MODIFIERS::KICKER_MODIFIER);
	kickerItem = playerItem;
	kickersId = playerIds;
	playerIds += 1;
	Q_EMIT playerCreated(playerItem);
	Q_EMIT placeKickerSig(playerItem->getTrackId(), QPointF(centerX, centerY));
	return playerItem;
}

void PlotterController::deletePlayer(int id)
{
	for (PlayerItemWidget* pl : createdPlayers)
	{
		if (*pl == id)
		{
			createdPlayers.erase(std::remove(createdPlayers.begin(), createdPlayers.end(), pl), createdPlayers.end());
			delete pl;
			Q_EMIT playerDeleted(id);
			break;
		}
	}
}

void PlotterController::sceneRectChangedSlot(const QRect& sceneRect)
{
	this->sceneRect = sceneRect;
	centerX = sceneRect.width() / 2;
	centerY = sceneRect.height() / 2;
}

void PlotterController::clearAllPlotted()
{
	for (PlayerItemWidget* pl : createdPlayers)
	{
		delete pl;	
	}
	createdPlayers.clear();
	playerIds = 40;
	Q_EMIT clearAllPlottedPlayersSig();
}

void PlotterController::kickerRejected(int id)
{
	Q_EMIT kickerRejectedSig(kickersId);
	kickersId = -1;
	if (kickerItem != nullptr)
	{
		delete kickerItem;
		kickerItem = nullptr;
	}
}
