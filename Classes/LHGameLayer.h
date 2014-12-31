//
//  LHGameLayer.h
//  LHCocosFirstGame
//
//  Created by huji on 14-11-21.
//
//

#ifndef __LHCocosFirstGame__LHGameLayer__
#define __LHCocosFirstGame__LHGameLayer__

#include "cocos2d.h"
#include "FlyObject.h"

#define FOCUS_ANI_TIME 5.0f

#define TAG_OVER 0
#define TAG_SCORE 1

class LHGameLayer : public cocos2d::Layer{
public:
    LHGameLayer();
    virtual ~LHGameLayer();
    CREATE_FUNC(LHGameLayer);
    void update(float delta);
    std::function<void(int tag,int score,int tap)> onEventHappen;
    void start();
private:
    cocos2d::Size _mapSize;
    
    void focusOn(cocos2d::Vec2 center,float len,bool animate);
    void focusOn(cocos2d::Vec2 center,float len,bool animate,cocos2d::CallFunc* complete);
    
    cocos2d::Vec2 _focusCenter;
    float _focusLen;
    
    //game
    void popOut(float x,float y,cocos2d::Vec2 gravity,cocos2d::Vec2 speed,float as,int tag,bool isdiff);
    void gen();
    bool isOutScreen(FlyObject *fly);
    void tapOnFly(FlyObject *fly);
    void gameOver(FlyObject *fly);
    
    cocos2d::Vector<FlyObject*> flys;
    cocos2d::Vector<FlyObject*> tapedFlys;
    cocos2d::Sprite* tap;
    int score;
};

#endif /* defined(__LHCocosFirstGame__LHGameLayer__) */
