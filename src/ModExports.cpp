#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Global pointer to the MoreOptionsLayer instance.
// This pointer will be set when MoreOptionsLayer is initialized.
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

// Create an accessor subclass to expose MoreOptionsLayer::onKeybindings (which is private)
class MoreOptionsLayerAccessor : public MoreOptionsLayer {
public:
    using MoreOptionsLayer::onKeybindings; // make private onKeybindings accessible
};

// Expose MoreOptionsLayer::onKeybindings via our accessor class
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::CCObject* obj) {
        if (g_moreOptionsLayerInstance) {
            // Cast to our accessor subclass to bypass private access and call the function.
            static_cast<MoreOptionsLayerAccessor*>(g_moreOptionsLayerInstance)->onKeybindings(obj);
        }
    }
}

// Hook into MoreOptionsLayer's initialization using Geode's $modify macro
// to set the global instance pointer when the layer is created.
class $modify(MoreOptionsLayer, MoreOptionsLayer) {
public:
    bool init() {
        bool ret = MoreOptionsLayer::init();
        if (ret) {
            // Set the global pointer to this instance
            g_moreOptionsLayerInstance = this;
        }
        return ret;
    }
};
