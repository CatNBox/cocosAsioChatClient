#include "cocosAsioChatClient.h"
#include "SimpleAudioEngine.h"

//layers
#include "mainScBGlayer.h"
#include "mainScMenuLayer.h"

USING_NS_CC;

Scene* cocosAsioChatClient::createScene()
{
    // 'scene' is an autorelease object
    auto scene = cocosAsioChatClient::create();
    
    // 'layer' is an autorelease object
    auto bgLayer = mainScBGLayer::create();
	auto menuLayer = mainScMenuLayer::create();
	menuLayer->setName("mainLayer");

    // add layer as a child to scene
    scene->addChild(bgLayer, 0);
	scene->addChild(menuLayer, 1);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool cocosAsioChatClient::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    return true;
}
