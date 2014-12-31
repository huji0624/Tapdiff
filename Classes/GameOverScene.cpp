//------.cpp
#include "GameOverScene.h"
#include "UIText.h"
#include "LHMacros.h"
#include "LHShareButton.h"
#include "LHLeaderBoard.h"
#include "PlayScene.h"
#include "HelloWorldScene.h"
#include "LocalizedString.h"
#include "ThirdPartyHelper.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
Scene* GameOverScene::createScene(cocos2d::CCDictionary *dic)
{
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    layer->initWithColor(Color4B::WHITE);
    layer->initDict(dic);
    scene->addChild(layer);
    return scene;
}

GameOverScene::~GameOverScene(){
    ThirdPartyHelper::setAd(SET_AD_HID);
}

static float godelay = 0;

static void show(Node *node){
    node->setScale(0.01);
    DelayTime *d = DelayTime::create(godelay);
    ScaleTo *b = ScaleTo::create(0.1, 1.2);
    ScaleTo *s = ScaleTo::create(0.03, 1);
    Sequence *ac = Sequence::create(d,b,s, NULL);
    node->runAction(ac);
    godelay += 0.08f;
}

bool GameOverScene::initDict(cocos2d::CCDictionary *dic)
{
    ThirdPartyHelper::setAd(SET_AD_SHOW);
    ThirdPartyHelper::setAd(SET_AD_SCREEN);
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    godelay = 0;
    
    CCInteger *score = (CCInteger*)dic->objectForKey("score");
    CCInteger *tap = (CCInteger*)dic->objectForKey("tap");
    
    auto gameover = ui::Text::create(LHLocalizedCString("gameover"), Common_Font, 70);
    gameover->setPosition(Vec2(vs.width/2 + vo.x, vs.height/4*3 + vo.y));
    gameover->setColor(Color3B::BLACK);
    this->addChild(gameover);
    show(gameover);
    
    int best = UserDefault::getInstance()->getIntegerForKey("best", 0);
    if (score->getValue()>best) {
        best = score->getValue();
        UserDefault::getInstance()->setIntegerForKey("best", best);
        ThirdPartyHelper::uploadScore(best);
    }
    
    auto scorelabel = ui::Text::create(StringUtils::format("%s  %d",LHLocalizedCString("score"),score->getValue()), Common_Font, 50);
    scorelabel->setPosition(Vec2(vs.width/2 + vo.x, vs.height/2+scorelabel->getContentSize().height/2 + + vo.y));
    scorelabel->setColor(Color3B::BLACK);
    this->addChild(scorelabel);
    show(scorelabel);
    
    float spy = (scorelabel->getPositionY() + gameover->getPositionY())/2;
    float margin = 10;
    auto sp1 = Sprite::createWithSpriteFrameName(StringUtils::format("%d.png",tap->getValue()));
    sp1->setPosition(Vec2(vs.width/2 + vo.x - sp1->getContentSize().width/2 - margin, spy));
    addChild(sp1);
    show(sp1);
    auto sp2 = Sprite::createWithSpriteFrameName(StringUtils::format("%d.png",-tap->getValue()));
    sp2->setPosition(Vec2(vs.width/2 + vo.x + sp2->getContentSize().width/2 + margin, spy));
    addChild(sp2);
    show(sp2);
    
    auto bestlabel = ui::Text::create(StringUtils::format("%s  %d",LHLocalizedCString("best"),best), Common_Font, 50);
    bestlabel->setPosition(Vec2(vs.width/2 + vo.x, vs.height/2-bestlabel->getContentSize().height/2+ vo.y));
    bestlabel->setColor(Color3B::BLACK);
    this->addChild(bestlabel);
    show(bestlabel);
    
    float by = vs.height/6 + vo.y;
    float fs = 40;
    
    auto replay = ui::Button::create("replay.png");
    replay->setTitleFontSize(fs);
    replay->setPosition(Vec2(vs.width/2+ vo.x, vs.height/3+ vo.y));
    replay->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(PlayScene::createScene(nullptr));
        }
    });
    this->addChild(replay);
    show(replay);
    
    auto back = ui::Button::create("back.png");
    back->setPosition(Vec2(vs.width/4 + vo.x, by));
    back->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(HelloWorld::createScene());
        }
    });
    this->addChild(back);
    show(back);
    
    auto share = LHShareButton::defaultButton("share.png",LHLocalizedCString("appname"));
    share->setPosition(Vec2(vs.width/2 + vo.x, by));
    this->addChild(share);
    show(share);
    
    std::string lb = "lb.png";
    auto leader = LHLeaderBoard::defaultButton(lb);
    leader->setPosition(Vec2(vs.width/4*3 + vo.x, by));
    this->addChild(leader);
    show(leader);
    
    return true;
}