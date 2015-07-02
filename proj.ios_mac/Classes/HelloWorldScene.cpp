#include "HelloWorldScene.h"



USING_NS_CC;
using namespace SnakeSpace;

Scene* HelloWorld::createScene()
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
void HelloWorld::onEnter()
{
    log("2scene enter");
    Scene::onEnter();
}
void HelloWorld::onExit()
{
    log("2scene Exit");
    Scene::onExit();
}


//void HelloWorld::menuCloseCallback(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}
