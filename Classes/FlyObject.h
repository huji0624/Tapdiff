//
//  FlyObject.h
//  LHCocosFirstGame
//
//  Created by huji on 14-11-24.
//
//

#ifndef __LHCocosFirstGame__FlyObject__
#define __LHCocosFirstGame__FlyObject__

#include "cocos2d.h"

class FlyObject : public cocos2d::Ref{
public:
    bool init();
    
    virtual ~FlyObject(){
        CC_SAFE_RELEASE(aSprite);
    }
    
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite*, aSprite, ASprite);
    cocos2d::Vec2 speed;
    cocos2d::Vec2 gravity;
    float ix;
    float iy;
    int tag;
    bool isdiff;
    CREATE_FUNC(FlyObject);
};

#endif /* defined(__LHCocosFirstGame__FlyObject__) */
