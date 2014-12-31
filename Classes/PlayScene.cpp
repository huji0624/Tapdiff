//------.cpp
#include "PlayScene.h"
#include "ThirdPartyHelper.h"
#include "UIButton.h"
#include "UIText.h"
#include "LHMacros.h"
#include "LocalizedString.h"
#include "GameOverScene.h"
USING_NS_CC;
Scene* PlayScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = PlayScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

PlayScene::~PlayScene(){
}

void PlayScene::update(float delta){
    
}

bool PlayScene::initDict(cocos2d::CCDictionary *dic)
{
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    auto scoretext = ui::Text::create("0", Common_Font, 200);
    scoretext->setPosition(Vec2(vs.width/2,vs.height/2));
    scoretext->setColor(Color3B::GRAY);
    scoretext->setVisible(false);
    scoretext->setTouchEnabled(false);
    
    auto ready = ui::Text::create(LHLocalizedCString("ready"), Common_Font, 120);
    ready->setPosition(Vec2(vs.width/2+vo.x, vs.height/2+vo.y));
    ready->setColor(Color3B::BLACK);
    addChild(ready);
    auto dl = DelayTime::create(0.5);
    auto sd = ScaleTo::create(0.2, 1.5f);
    auto cc = CallFunc::create([ready](){
        ready->setString(LHLocalizedCString("go"));
    });
    auto su = ScaleTo::create(0.2, 1.0);
    auto dl2 = DelayTime::create(0.5);
    auto call = CallFunc::create([scoretext,ready,this](){
        auto sc = ScaleTo::create(0.1, 0.1);
        auto gocall = CallFunc::create([scoretext,ready,this](){
            ready->removeFromParent();
            _gameLayer->start();
            scoretext->setVisible(true);
        });
        auto sq = Sequence::create(sc,gocall, NULL);
        ready->runAction(sq);
    });
    auto sq = Sequence::create(dl,sd,cc,su,dl2,call, NULL);
    ready->runAction(sq);
    
    _gameLayer = LHGameLayer::create();
    _gameLayer->setPosition(vo.x,vo.y);
    addChild(_gameLayer);
    _gameLayer->onEventHappen = [scoretext](int tag,int score,int tap){
        if (tag == TAG_SCORE) {
            scoretext->setString(StringUtils::format("%d",score));
        }else if (tag == TAG_OVER){
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCInteger::create(score), "score");
            dict->setObject(CCInteger::create(tap), "tap");
            Director::getInstance()->replaceScene(GameOverScene::createScene(dict));
        }
    };
    
     _gameLayer->addChild(scoretext,1);
    
	return true;
}