#pragma once
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckbox>
#include <QtWidgets/QLineEdit>
#include <nlohmann/json.hpp>
#include <string>
#include <map>

using json = nlohmann::json;

#define W_TRACK_ID "track_id_label"
#define W_COORD_X "player_x_coordinate"
#define W_COORD_Y "player_y_coordinate"
#define W_POSITION "player_field_position"

class PlayerPropertiesGroup : public QObject
{
	Q_OBJECT

public:
	PlayerPropertiesGroup(QLabel*, QLineEdit*, QLineEdit*, QLabel* position, QObject* parent = nullptr);

	void AddWidget(std::string name, QWidget*);

public slots:
	void currentClickedTrack(json currentObject);

private:
	std::map<std::string, QWidget*> widgetsMap;
	QLabel* trackID, *position;
	QLineEdit* xCoord, *yCoord;
};