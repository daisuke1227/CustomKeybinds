#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

// Hack: Temporarily redefine 'private' as 'public' to expose MoreOptionsLayer's members.
#define private public
#include <Geode/modify/MoreOptionsLayer.hpp>
#undef private

#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Global pointer to the MoreOptionsLayer instance.
// This will be set when MoreOptionsLayer is initialized.
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

// Create an accessor subclass to expose MoreOptionsLayer::onKeybindings.
// Now that we've temporarily made private members public, we can use a using-declaration.
class MoreOptionsLayerAccessor : public MoreOptionsLayer {
public:
    using MoreOptionsLayer::onKeybindings; // Make the formerly private onKeybindings accessible.
};

// Expose MoreOptionsLayer::onKeybindings via our accessor class
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::CCObject* obj) {
        if (g_moreOptionsLayerInstance) {
            // Cast the instance to our accessor class and call onKeybindings.
            static_cast<MoreOptionsLayerAccessor*>(g_moreOptionsLayerInstance)->onKeybindings(obj);
        }
    }
}

// Hook into MoreOptionsLayer's initialization using Geode's $modify macro.
// This hook will be called when the game creates the MoreOptionsLayer, so we can store its instance.
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
