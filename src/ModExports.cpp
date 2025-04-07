#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Expose CCKeyboardDispatcher::dispatchKeyboardMSG
extern "C" {
    GEODE_EXPORT bool CCKeyboardDispatcher_dispatchKeyboardMSG(
        cocos2d::enumKeyCodes key,
        bool down,
        bool repeat
    ) {
        // Forward to the singleton instance’s method.
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

// Expose MoreOptionsLayer::onKeybindings
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(CCObject* obj) {
        // Since onKeybindings is an instance method, you need to decide how you want to expose it.
        // For example, you might forward it on a singleton instance if one exists,
        // or you could simply implement a dummy that calls the original implementation on an instance.
        // For this example, we assume there is a global MoreOptionsLayer instance:
        auto layer = MoreOptionsLayer::getInstance();
        if (layer) {
            layer->onKeybindings(obj);
        }
    }
}
