//------.h
#ifndef __PLAYSCENE__
#define __PLAYSCENE__
#include "cocos2d.h"
#include "LHGameLayer.h"
class PlayScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~PlayScene();
    virtual void update(float delta);
	CREATE_FUNC(PlayScene);
private:
    LHGameLayer *_gameLayer;
};
#endif