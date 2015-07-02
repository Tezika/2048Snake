#pragma once
#include"cocos2d.h"
#include"GameCtrlLayer.h"
#include"OverLayer.h"

class LayerFactory{
public:
	LayerFactory();
	~LayerFactory();
	static cocos2d::Layer* createForLayerTag();

};
