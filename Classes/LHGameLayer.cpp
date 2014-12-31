//
//  LHGameLayer.cpp
//  LHCocosFirstGame
//
//  Created by huji on 14-11-21.
//
//

#include "LHGameLayer.h"
#include "PHSprite.h"
#include "LHMacros.h"

#define LHVS Director::getInstance()->getVisibleSize()
#define LHVO Director::getInstance()->getVisibleOrigin()

#define PDMargin 64
#define GASF 1.5f

USING_NS_CC;
LHGameLayer::LHGameLayer(){
    _mapSize = Size(MAXFLOAT, MAXFLOAT);
}

LHGameLayer::~LHGameLayer(){
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}

void LHGameLayer::start(){
    auto lis = EventListenerTouchOneByOne::create();
    lis->onTouchBegan = [this](Touch* tmpTouch, Event*){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("tap.wav");
        Vec2 tp = tmpTouch->getLocation();
        tp.subtract(LHVO);
        tap->setPosition(tp);
        tap->setVisible(true);
        FlyObject *taped = nullptr;
        for (int i=flys.size()-1;i>=0;i--){
            auto fly = flys.at(i);
            if (fly->getASprite()->getBoundingBox().containsPoint(tp)) {
                taped = fly;
                break;
            }
        }
        if (taped) {
            this->tapOnFly(taped);
        }
        return true;
    };
    lis->onTouchMoved = [this](Touch* tmpTouch, Event*){
    };
    lis->onTouchEnded = [this](Touch*, Event*){
        tap->setVisible(false);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, this);
    
    tap = Sprite::createWithSpriteFrameName("tap.png");
    tap->setVisible(false);
    addChild(tap,3);
    
    score = 0;
    gen();
    
    scheduleUpdate();
}

void LHGameLayer::tapOnFly(FlyObject *fly){
    tapedFlys.pushBack(fly);
    flys.eraseObject(fly);
    fly->getASprite()->stopAllActions();
    
    fly->getASprite()->setScale(0.8f);
    FadeTo *fd = FadeTo::create(30.0f, 1);
    CallFunc *call = CallFunc::create([fly,this](){
        fly->getASprite()->removeFromParent();
        tapedFlys.eraseObject(fly);
    });
    auto sq = Sequence::create(fd,call, NULL);
    fly->getASprite()->runAction(sq);
    
    if (fly->isdiff) {
        score++;
        onEventHappen(TAG_SCORE,score,0);
    }else{
        gameOver(fly);
    }
}

void LHGameLayer::gameOver(FlyObject *tfly){
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("over.wav");
    
    for (auto fly : flys) {
        fly->getASprite()->stopAllActions();
    }
    unscheduleUpdate();
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    onEventHappen(TAG_OVER,score,tfly->tag);
}

void LHGameLayer::gen(){
    int MAXO = 4;
    float baseXHSpeed = 150;
    float baseYHSpeed = 300;
    
    float baseYVHSpeedD = LHVS.height/3;
    float baseYVHSpeedH = LHVS.height/2;
    
    float baseXVHSpeedD = LHVS.width/2;
    float baseXVHSpeedH = LHVS.width/2;
    
    float margin = 0;
    
    Size vs = Size(LHVS.width+2*margin, LHVS.height+2*margin);
    int df = arc4random()%MAXO;
    int r = arc4random()%2==0?1:-1;
    int tag = r*(arc4random()%MAXTAG+1);
    for (int i = 0 ; i<MAXO;i++) {
        Vec2 speed;
        Vec2 gravity;
        
        int p = arc4random()%4;
        float x = -margin;
        float y = -margin;
        if (p==0) {
            x = vs.width * CCRANDOM_0_1();
            if (x<vs.width/2) {
                speed.x = baseXHSpeed * CCRANDOM_0_1();
            }else{
                speed.x = - baseXHSpeed * CCRANDOM_0_1();
            }
            speed.y = baseYVHSpeedD + baseYVHSpeedH * CCRANDOM_0_1();
            gravity.y=-2;
        }else if (p==1){
            x = vs.width;
            y = vs.height * CCRANDOM_0_1();
            if (y<vs.height/2) {
                speed.y = baseYHSpeed * CCRANDOM_0_1();
            }else{
                speed.y = - baseYHSpeed * CCRANDOM_0_1();
            }
            speed.x = -baseXVHSpeedD - baseXVHSpeedH*CCRANDOM_0_1();
            gravity.x=1;
        }else if (p==2){
            x = vs.width * CCRANDOM_0_1();
            y = vs.height;
            if (x<vs.width/2) {
                speed.x = baseXHSpeed * CCRANDOM_0_1();
            }else{
                speed.x = - baseXHSpeed * CCRANDOM_0_1();
            }
            speed.y = -baseYVHSpeedD - baseYVHSpeedH * CCRANDOM_0_1();
            gravity.y=2;
        }else{
            y = vs.height * CCRANDOM_0_1();
            if (y<vs.height/2) {
                speed.y = baseYHSpeed * CCRANDOM_0_1();
            }else{
                speed.y = - baseYHSpeed * CCRANDOM_0_1();
            }
            speed.x = baseXVHSpeedD + baseXVHSpeedH*CCRANDOM_0_1();
            gravity.x=-1;
        }
        
        float as = 0;
        as = CCRANDOM_0_1() * 360;
        
        popOut(x, y, gravity, speed, as, i==df?tag:(-tag),i==df);
    }
}

void LHGameLayer::popOut(float x, float y, cocos2d::Vec2 gravity, cocos2d::Vec2 speed,float as,int tag,bool isdiff){
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gen.wav");
    
    auto fly = FlyObject::create();
    auto psp = Sprite::createWithSpriteFrameName(StringUtils::format("%d.png",tag));
    fly->setASprite(psp);
    fly->getASprite()->setPositionX(x);
    fly->getASprite()->setPositionY(y);
    fly->getASprite()->setRotation(arc4random()%360);
    fly->speed = speed;
    fly->gravity = gravity;
    fly->ix = x;
    fly->iy = y;
    fly->tag = tag;
    fly->isdiff = isdiff;
    
    if (as>0) {
        float du = 0.1;
        RotateBy *rb = RotateBy::create(du, as*du);
        RepeatForever *rp = RepeatForever::create(rb);
        fly->getASprite()->runAction(rp);
    }
    
    addChild(psp,2);
    flys.pushBack(fly);
}

void LHGameLayer::update(float delta){
    if(flys.size()>0){
        FlyObject *erase = nullptr;
        for (int i =0; i<flys.size(); i++) {
            auto fly = flys.at(i);
            Vec2 dis = fly->speed;
            dis.scale(delta);
            
            Vec2 dspeed = fly->gravity;
            Vec2 po = fly->getASprite()->getPosition();
            po.subtract(Vec2(fly->ix, fly->iy));
            po.dot(dspeed);
            float height = po.length()*GASF;
            dspeed.scale(delta*height);
            fly->speed.add(dspeed);
            
            dis.add(fly->getASprite()->getPosition());
            fly->getASprite()->setPosition(dis);
            
            if (isOutScreen(fly)) {
                erase = fly;
            }
        }
        if (erase) {
            if (erase->isdiff) {
                gameOver(erase);
            }else{
                erase->getASprite()->removeFromParent();
                flys.eraseObject(erase);
            }
        }
    }else{
//        score--;
//        onEventHappen(TAG_SCORE,score);
        gen();
    }
}

bool LHGameLayer::isOutScreen(FlyObject *fly){
    Vec2 po = fly->getASprite()->getPosition();
    float margin = PDMargin+10;
    Size vs = LHVS;
    vs.width += margin*2;
    vs.height += margin*2;
    if (po.x<-margin||po.x>vs.width) {
        return true;
    }
    if (po.y<-margin||po.y>vs.height){
        return true;
    }
    return false;
}

void LHGameLayer::focusOn(cocos2d::Vec2 center, float len, bool animate, cocos2d::CallFunc *complete){
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    float toscale = 1.0f;
    if (len<vs.width) {
        toscale = vs.width/len;
    }else if(len>vs.height){
        toscale = vs.height/len;
    }
    
    Vec2 sccenter = Vec2(vs.width/2+vo.x, vs.height/2 + vo.y);
    
    float tox = sccenter.x - center.x*toscale;
    float toy = sccenter.y - center.y*toscale;
    
    if (toy>vo.y) {
        toy = vo.y;
    }else if (toy<vs.height-toscale*_mapSize.height+vo.y){
        toy = vs.height-toscale*_mapSize.height+vo.y;
    }
    
    if (tox>vo.x) {
        tox = vo.x;
    }else if (tox<vs.width-toscale*_mapSize.width+vo.x){
        tox = vs.width-toscale*_mapSize.width+vo.x;
    }
    
    if (animate) {
        auto scale = ScaleTo::create(FOCUS_ANI_TIME, toscale);
        auto mv = MoveTo::create(FOCUS_ANI_TIME, Vec2(tox, toy));
        auto conc = Spawn::create(scale,mv, NULL);
        if (complete==nullptr) {
            this->runAction(conc);
        }else{
            auto sq = Sequence::create(conc,complete, NULL);
            this->runAction(sq);
        }
    }else{
        this->setPosition(tox, toy);
        this->setScale(toscale);
    }
    
    _focusCenter = Vec2(center.x, (sccenter.y - toy)/toscale);
    _focusLen = len;
}

void LHGameLayer::focusOn(cocos2d::Vec2 center, float len, bool animate){
    this->focusOn(center, len, animate, nullptr);
}