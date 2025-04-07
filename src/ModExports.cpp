// IMPORTANT: Redefine 'private' as 'public' BEFORE any header that might include MoreOptionsLayer.
#define private public
#include <Geode/modify/MoreOptionsLayer.hpp>
#undef private

#include <cocos2d.h>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

// Global pointer to the MoreOptionsLayer instance.
// This will be set when MoreOptionsLayer is created via its init() method.
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

// Define an accessor subclass that exposes MoreOptionsLayer::onKeybindings.
// Because we redefined private as public when including the header,
// the formerly private onKeybindings method is now accessible.
class MoreOptionsLayerAccessor : public MoreOptionsLayer {
public:
    using MoreOptionsLayer::onKeybindings;
};

// Expose MoreOptionsLayer::onKeybindings via our accessor class.
extern "C" {
    GEODE_EXPORT void MoreOptionsLayer_onKeybindings(cocos2d::CCObject* obj) {
        if (g_moreOptionsLayerInstance) {
            // Cast the instance to our accessor subclass and call onKeybindings.
            static_cast<MoreOptionsLayerAccessor*>(g_moreOptionsLayerInstance)->onKeybindings(obj);
        }
    }
}

// Hook into MoreOptionsLayer's initialization using Geode's $modify macro.
// Use the single-parameter form to avoid redefining the class.
class $modify(MoreOptionsLayer) {
public:
    bool init() {
        bool ret = MoreOptionsLayer::init();
        if (ret) {
            // When the layer is initialized, store its instance in the global pointer.
            g_moreOptionsLayerInstance = this;
        }
        return ret;
    }
};
