#include <views/player_state_mod.hpp>
#include <iostream>


PlayerStateModifierGroup::PlayerStateModifierGroup(QCheckBox* _highlight, QLineEdit* annotText, QComboBox* position, QCheckBox* _activateAnnotation, QPushButton* _updateButton, QObject *parent)
	:QObject(parent),
	highlight(_highlight),
	annotationText(annotText),
	activateAnnotation(_activateAnnotation),
	positionInfo(position),
	updateButton(_updateButton),
	playerId(-1)
{
	_updateButton->connect(_updateButton, &QPushButton::click, this,
		&PlayerStateModifierGroup::updateState);
}

void PlayerStateModifierGroup::updateState()
{
	if (playerState.empty()) // this simply means there's no currently selected player Object.
		return;
	
	playerId = playerState["track_id"];
	
	// read the GUI state of a highlight
	if (highlight->isChecked())
	{
		json highlighObject;
		highlighObject["id"] = playerId;
		highlighObject["activate"] = true;

		stateObject["highlight"] = highlighObject;
	}

	if (!annotationText->text().isEmpty())
	{
		QString annTxt = annotationText->text();

		std::string annText(annTxt.begin(), annTxt.end());
		std::cout << annText << std::endl;
		json annotationObject;
		annotationObject["id"] = playerId;
		annotationObject["text"] = annText;
		annotationObject["activate"] = activateAnnotation->isChecked();

		stateObject["annotation"] = annotationObject;
	}

	int currentSelected = positionInfo->currentIndex();
	QString positionTxt = positionInfo->currentText();




}

json PlayerStateModifierGroup::getCurrentState()
{
	return stateObject;
}


