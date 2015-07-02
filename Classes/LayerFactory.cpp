#include"LayerFactory.h"
using SnakeSpace::GameCtrlLayer_Classic;
using SnakeSpace::GameCtrlLayer_Creazy;
using SnakeSpace::OverLayerBase;
using SnakeSpace::HelpLayer;

extern const int TAG_CLASSIC;
extern const int TAG_CRAZY;
extern const int TAG_OVER_CLASSIC;
extern const int TAG_OVER_CRAZY;
extern const int TAG_HELP;

cocos2d::Layer* LayerFactory::createForLayerTag()
{
	int layerTag = UserDefault::getInstance()->getIntegerForKey("GAMELAYER");
	if (TAG_CLASSIC == layerTag)
	{
		return GameCtrlLayer_Classic::create();
	}
	else if (TAG_CRAZY == layerTag)
	{
		return GameCtrlLayer_Creazy::create();
	}
	else if (TAG_OVER_CLASSIC == layerTag||TAG_OVER_CRAZY==layerTag)
	{
		return OverLayerBase::create();
	}
	else if (TAG_HELP == layerTag)
	{
		return HelpLayer::create();
	}

}