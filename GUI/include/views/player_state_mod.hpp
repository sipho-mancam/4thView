#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushbutton>
#include <QtWidgets/QCombobox>
#include <QtCore/qobject.h>
#include <nlohmann/json.hpp>
#include <kevents.hpp>


using json = nlohmann::json;

class PlayerStateModifierGroup : public QObject
{
	Q_OBJECT

public:
	PlayerStateModifierGroup(QCheckBox* highlight, QLineEdit* annotText, 
		QComboBox* position, QCheckBox* activateAnnotation, 
		QPushButton* _updateButton, QObject* parent=(nullptr));

	void updateState();
	json getCurrentState();

signals:
	/**
	* @brief
	* This function sends the state to the externals objects that care update the updates to 
	* the current GUI state of the player.
	*/
	void playerStateChanged(json newState);

public slots:
	/**
	* @brief 
	* This function receives the state from the click events of player objects and update the current 
	* GUI elements to reflect the current state of the player from the incoming frame.
	*/
	void laodCurrentState(json state);

	

private:
	QComboBox* positionInfo;
	QLineEdit* annotationText;
	QCheckBox* highlight, * activateAnnotation;
	QPushButton* updateButton;
	/**
	* @brief 
	* Player state represents the raw state coming from the tracker.
	* While state Object represents the state modification of an object, 
	* which is just a group of states lumped to together like
	* {
	*	highlight:{
	*	}
	*	Annotate: {
	*	}
	*	Position: {
	*	}
	* etc.
	* }
	*/
	json playerState, stateObject;
	int playerId;



};