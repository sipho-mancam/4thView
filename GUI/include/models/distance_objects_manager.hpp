#pragma once
#include <QtCore/QObject>


/**
* @brief
* This class manages the distance objects, by listening for 3 events
* 1. Init Add Distance,
* 2. Add Player 1
* 3. Add Player 2 
*/

class DistanceObjectsManager : public QObject
{
	Q_OBJECT
};