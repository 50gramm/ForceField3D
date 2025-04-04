#include "Exception.hpp"
#include "JsonUtils.hpp"
#include "ForceFieldOp.hpp"


ForceFieldOp::ForceFieldOp(const Settings& settings)
	: visSettings(settings["ForceField"]["Visual"])
	, scene(visSettings)
	, charges(state, visSettings)
	, forceLines(state, visSettings)
	, equiPotFace(state, visSettings)
	, chargeManipulator(state, spaceNode, charges)
	, smartMovingOp(spaceNode)
{
	spaceNode.addChild(&charges);
	spaceNode.addChild(&forceLines);
	spaceNode.addChild(&equiPotFace);
	addChild(&cursor);
	addChild(&scene);
	addChild(&spaceNode);

	parseJson(parseJsonFile("{assets}/ffstart.json"));
}


bool ForceFieldOp::onKey(const KeyEvent& event)
{
	if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 's')
	{
		try {
			dumpToJsonFile(dumpToJson(), "{user}/ffsave.json");
			LOG_I("Current state saved.");
		} catch(std::exception& ex) {
			LOG_I("Failed to save state. Error:\n\t%s", ex.what());
		}
		return true;
	}
	else if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 'l')
	{
		try {
			parseJson(parseJsonFile("{user}/ffsave.json"));
			LOG_I("Previously saved state loaded.");
		} catch(std::exception& ex) {
			LOG_I("Failed to load state. Error:\n\t%s", ex.what());
		}
		return true;
	}
	else if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 'r')
	{
		parseJson(parseJsonFile("{assets}/ffstart.json"));
		return true;
	}
	else if(event.action == KeyEvent::ACTION_DOWN && event.keyCode == 127) // Del
	{
		state = {};
		return true;
	}

	return false;
}


Json ForceFieldOp::dumpToJson() const
{
	Json jSave;
	jSave["Version"] = 1;
	jSave["Matrix"] = dumpMatrixToJson(spaceNode.getPosition());
	jSave["ForceField"] = state.dumpToJson();
	return jSave;
}


void ForceFieldOp::parseJson(const Json& jSave)
{
	if(1 < jSave["Version"])
		throw Exception("The state was saved with newer version of program");

	spaceNode.setPosition(parseJsonMatrix(jSave["Matrix"]));
	state = ForceFieldState::parseJson(jSave["ForceField"]);
}
