//
//  ThirdPartyHelper.cpp
//  LHCocosFirstGame
//
//  Created by huji on 6/19/14.
//
//

#include "ThirdPartyHelper.h"
#include "cocos2d.h"
#include "NDKHelper.h"

#define LHOPENTIME "opentime"
#define SCREEN_AD_TIME 3

void ThirdPartyHelper::setAd(int tag){
    if (tag == SET_AD_SCREEN) {
        const char *key = "admobscreenad";
        int time = UserDefault::getInstance()->getIntegerForKey(key, SCREEN_AD_TIME);
        if (time !=1 ) {
            UserDefault::getInstance()->setIntegerForKey(key, time-1);
            return;
        }else{
            UserDefault::getInstance()->setIntegerForKey(key, SCREEN_AD_TIME);
        }
    }
    
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCString::create(StringUtils::format("%d",tag)), "setAd");
    SendMessageWithParams("setAd", dic);
}

void ThirdPartyHelper::uploadScore(int score){
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCString::create(StringUtils::format("%d",score)), "value");
    dic->setObject(CCString::create("best"), "cat");
    SendMessageWithParams("leaderBoardControl", dic);
}

void ThirdPartyHelper::showLeaderBoard(){
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCString::create("best"), "cat");
    SendMessageWithParams("leaderBoardControl", dic);
}

void ThirdPartyHelper::showReviewAlertWhenOpenTime(int opentime){
    UserDefault *ud = UserDefault::getInstance();
    int cu = ud->getIntegerForKey(LHOPENTIME, 0);
    cu ++;
    ud->setIntegerForKey(LHOPENTIME, cu);
    if (cu == opentime) {
        ThirdPartyHelper::showReviewAlert();
    }
}

void ThirdPartyHelper::showReviewAlert(){
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCString::create("Review"), "title");
    dic->setObject(CCString::create("Give up a review on store."), "message");
    dic->setObject(CCString::create("Ok"), "ok");
    dic->setObject(CCString::create("Cancel"), "cancel");
    SendMessageWithParams("showReviewAlert", dic);
}

void ThirdPartyHelper::goReview(){
    SendMessageWithParams("goReview", nullptr);
}