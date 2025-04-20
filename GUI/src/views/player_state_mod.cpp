#include <views/player_state_mod.hpp>
#include <iostream>


PlayerStateModifierGroup::PlayerStateModifierGroup(QCheckBox* _highlight, QLineEdit* annotText, 
	QCheckBox* _activateAnnotation, QComboBox* position, 
	QPushButton* _updateButton, QObject *parent)

	:QObject(parent),
	highlight(_highlight),
	annotationText(annotText),
	activateAnnotation(_activateAnnotation),
	positionInfo(position),
	updateButton(_updateButton),
	playerId(-1)
{
	_updateButton->connect(_updateButton, &QPushButton::clicked, this,
		&PlayerStateModifierGroup::updateState);
}

void PlayerStateModifierGroup::updateState()
{
	if (playerState.empty()) // this simply means there's no currently selected player Object.
		return;
	
	playerId = playerState["track_id"];
	
	// read the GUI state of a highlight
	
	
		json highlighObject;
		highlighObject["id"] = playerId;
		highlighObject["activate"] = highlight->isChecked();
		highlighObject["set"] = highlight->isChecked();
		stateObject["highlight"] = highlighObject;
	

	if (!annotationText->text().isEmpty())
	{
		std::string annText = annotationText->text().toStdString();
		json annotationObject;
		annotationObject["id"] = playerId;
		annotationObject["text"] = annText;
		annotationObject["activate"] = activateAnnotation->isChecked();
		annotationObject["set"] = true;
		stateObject["annotation"] = annotationObject;
	}
	else {
		/**
		* @brief
		* This is the state that clears an annotation, when you set the text to empty
		*/
		json annotationObject;
		annotationObject["id"] = playerId;
		annotationObject["set"] = false;
		stateObject["annotation"] = annotationObject;
	}

	int currentSelected = positionInfo->currentIndex();
	std::string positionTxt = positionInfo->currentText().toStdString();
	KEvents::EPLAYER_POSITIONS pos = KEvents::__str2PlayerPosition__(positionTxt);

	if (pos != KEvents::EPLAYER_POSITIONS::C_ERROR)
	{
		json positionObject;
		positionObject["id"] = playerId;
		positionObject["position"] = pos;
		positionObject["set"] = true;
		stateObject["position"] = positionObject;
	}
	
	Q_EMIT playerStateChanged(stateObject);
}

void PlayerStateModifierGroup::laodCurrentState(json plState)
{
	playerState = plState;

	if (!playerState.contains("track_id"))
		return;

	resetWidgets();
	// Does the player have a highlight status?
	if (playerState.contains("highlight"))
	{
		bool highlght = playerState["highlight"];
		highlight->setChecked(highlght);
	}

	if (playerState.contains("annotation"))
	{
		json annotateObject = playerState["annotation"];
		std::string annText = annotateObject["text"];
		bool activate = annotateObject["activate"];

		activateAnnotation->setChecked(activate);
		annotationText->setText(QString(const_cast<char*>(annText.c_str())));
	}
	if (playerState.contains("position"))
	{
		KEvents::EPLAYER_POSITIONS pos = playerState["position"]["position"];
		
		int idx = -1;
		if (pos == KEvents::EPLAYER_POSITIONS::FIELDER)
		{
			idx = 0;
		}
		else if (pos == KEvents::EPLAYER_POSITIONS::BOWLER)
		{
			idx = 1;
		}
		else if (pos == KEvents::EPLAYER_POSITIONS::BATMAN_A)
		{
			idx = 2;
			
		}
		else if (pos == KEvents::EPLAYER_POSITIONS::BATMAN)
		{
			idx = 3;
		}
		else if (pos == KEvents::EPLAYER_POSITIONS::UMPIRE)
		{
			idx = 4;
		}

		if (idx >= 0)
		{
			positionInfo->setCurrentIndex(idx);
		}
	}

}

json PlayerStateModifierGroup::getCurrentState()
{
	return stateObject;
}

void PlayerStateModifierGroup::resetWidgets()
{
	highlight->setChecked(false);
	annotationText->setText(QString(""));
	activateAnnotation->setChecked(false);
	positionInfo->setCurrentIndex(0);
}


