#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Optional: Global pointer to MoreOptionsLayer instance.
// This is only needed if you plan to use the MoreOptionsLayer instance elsewhere.
MoreOptionsLayer* g_moreOptionsLayerInstance = nullptr;

// Expose CCKeyboardDispatcher::dispatchKeyboardMSG
extern "C" {
    GEODE_EXPORT bool CCKeyboardDispatcher_dispatchKeyboardMSG(
        cocos2d::enumKeyCodes key,
        bool down,
        bool repeat
    ) {
        return cocos2d::CCKeyboardDispatcher::get()->dispatchKeyboardMSG(key, down, repeat);
    }
}

// Expose CCKeyboardDispatcher::updateModifierKeys
extern "C" {
    GEODE_EXPORT void CCKeyboardDispatcher_updateModifierKeys(
        bool shift,
        bool ctrl,
        bool alt,
        bool cmd
    ) {
        cocos2d::CCKeyboardDispatcher::get()->updateModifierKeys(shift, ctrl, alt, cmd);
    }
}

// Stub for MoreOptionsLayer_onKeybindings.
// This function is intentionally left empty because MoreOptionsLayer::onKeybindings is deprecated
// and its functionality is not intended to be used.
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::CCObject* /*obj*/) {
        // No operation performed.
    }
}

// Optional: Hook into MoreOptionsLayer's initialization using Geode's $modify macro.
// This hook will capture the instance when the game creates the MoreOptionsLayer.
// If you don't need this functionality, you can remove the following block.
class $modify(MoreOptionsLayer) {
public:
    bool init() {
        bool ret = MoreOptionsLayer::init();
        if (ret) {
            g_moreOptionsLayerInstance = this;
        }
        return ret;
    }
};
