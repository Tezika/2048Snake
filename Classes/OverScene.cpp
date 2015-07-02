#include"OverScene.h"
Scene* OverScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LayerFactory::createForLayerTag();

	if (NULL != layer)
	{
		// add layer as a child to scene
		scene->addChild(layer);
	}
	// return the scene
	return scene;
}