#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayerMod, PlayLayer) {
public:
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        this->schedule(schedule_selector(PlayLayerMod::checkJumpscare), 1.0f);
        return true;
    }

    void checkJumpscare(float dt) {
        if (rand() % 13 == 0) {
            jumpscare();
        }
    }

    void jumpscare() {
        auto pl = this;
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto spriteTop = CCSprite::create("topbanner.png"_spr);
        auto spriteBottom = CCSprite::create("bottombanner.png"_spr);

        float scaleXTop = winSize.width / spriteTop->getContentSize().width;
        float scaleYTop = winSize.height / 2 / spriteTop->getContentSize().height;
        spriteTop->setScaleX(scaleXTop);
        spriteTop->setScaleY(scaleYTop);

        float scaleXBottom = winSize.width / spriteBottom->getContentSize().width;
        float scaleYBottom = winSize.height / 2 / spriteBottom->getContentSize().height;
        spriteBottom->setScaleX(scaleXBottom);
        spriteBottom->setScaleY(scaleYBottom);

        spriteTop->setZOrder(999999);
        spriteBottom->setZOrder(999999);

        spriteTop->setPosition(ccp(winSize.width + spriteTop->getContentSize().width * scaleXTop / 2,
                                   winSize.height - (spriteTop->getContentSize().height * scaleYTop / 2)));
        spriteBottom->setPosition(ccp(-spriteBottom->getContentSize().width * scaleXBottom / 2,
                                      spriteBottom->getContentSize().height * scaleYBottom / 2));

        pl->addChild(spriteTop);
        pl->addChild(spriteBottom);

        spriteTop->runAction(
            CCEaseSineInOut::create(
                CCMoveTo::create(0.2f, ccp(winSize.width / 2,
                                           winSize.height - (spriteTop->getContentSize().height * scaleYTop / 2)))
            )
        );
        spriteBottom->runAction(
            CCSequence::create(
                CCDelayTime::create(0.7f),
                CCEaseSineInOut::create(
                    CCMoveTo::create(0.2f, ccp(winSize.width / 2,
                                               spriteBottom->getContentSize().height * scaleYBottom / 2))
                ),
                nullptr
            )
        );

        if (auto audio = FMODAudioEngine::sharedEngine()) {
            audio->playEffect("tidalwave.mp3");
        }

        spriteTop->runAction(
            CCSequence::create(
                CCDelayTime::create(1.4f),
                CCFadeOut::create(0.2f),
                CCRemoveSelf::create(),
                nullptr
            )
        );

        spriteBottom->runAction(
            CCSequence::create(
                CCDelayTime::create(1.4f),
                CCFadeOut::create(0.2f),
                CCRemoveSelf::create(),
                nullptr
            )
        );
    }
};
