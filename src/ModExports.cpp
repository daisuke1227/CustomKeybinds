#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Global pointer to the MoreOptionsLayer instance.
// You must initialize and update this pointer appropriately elsewhere in your project.
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

// Since MoreOptionsLayer::onKeybindings is a private member, we can bypass access control
// using a pointer-to-member function hack. This is not ideal in standard application code,
// but it is sometimes used in modding contexts.
typedef void (MoreOptionsLayer::*OnKeybindingsFnType)(cocos2d::CCObject*);
static OnKeybindingsFnType onKeybindingsFn = reinterpret_cast<OnKeybindingsFnType>(&MoreOptionsLayer::onKeybindings);

// Expose MoreOptionsLayer::onKeybindings
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::CCObject* obj) {
        if (g_moreOptionsLayerInstance) {
            // Call the private member function via the function pointer.
            (g_moreOptionsLayerInstance->*onKeybindingsFn)(obj);
        }
    }
}
