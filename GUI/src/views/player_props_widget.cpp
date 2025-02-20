#include "views/player_props_widget.hpp"

PlayerPropertiesGroup::PlayerPropertiesGroup(QLabel* track_id, QLineEdit* xC, QLineEdit*yC, QLabel* pos, QObject* parent)
	:QObject(parent),
	trackID(track_id),
	xCoord(xC),
	yCoord(yC),
	position(pos)
{
}

void PlayerPropertiesGroup::AddWidget(std::string name, QWidget*)
{
}

void PlayerPropertiesGroup::currentClickedTrack(json currentObject)
{
	auto trackId = currentObject["track_id"];
	std::vector<double> coordinates = currentObject["coordinates"];
	if (currentObject.contains("position"))
	{
		auto pos = currentObject["position"];

		//position->setText(QString(pos));
	}
	if (trackId != nullptr)
	{
		int track_id = (int)(trackId);
		trackID->setText(std::to_string(track_id).c_str());
	}

	double x = coordinates[0], y = coordinates[1];
	/*x = round(x);
	y = round(y);*/
	xCoord->setText(const_cast<char*>(std::to_string(x).c_str()));
	yCoord->setText(const_cast<char*>(std::to_string(y).c_str()));
}
