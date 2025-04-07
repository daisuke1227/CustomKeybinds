#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Global pointer to the MoreOptionsLayer instance.
// You must initialize and manage this pointer appropriately elsewhere in your code.
MoreOptionsLayer* g_moreOptionsLayerInstance = nullptr;

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
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::Ref* obj) {
        // Use the globally managed MoreOptionsLayer instance.
        if (g_moreOptionsLayerInstance) {
            g_moreOptionsLayerInstance->onKeybindings(obj);
        }
    }
}
