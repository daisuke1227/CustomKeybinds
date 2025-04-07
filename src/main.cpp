#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>

#include <unordered_set>

#include "../include/Keybinds.hpp"
#include "KeybindsLayer.hpp"

using namespace geode::prelude;
using namespace keybinds;

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/CCEGLView.hpp>

// Retain the modifications for CCEGLView since these do not cause binding issues.
class $modify(CCEGLView) {
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        enumKeyCodes keycode = enumKeyCodes::KEY_Unknown;

        switch (key) {
            case GLFW_KEY_LEFT_SHIFT:
                keycode = enumKeyCodes::KEY_LeftShift;
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                keycode = enumKeyCodes::KEY_RightShift;
                break;
        }

        if (keycode != enumKeyCodes::KEY_Unknown) {
            CCKeyboardDispatcher::get()->dispatchKeyboardMSG(keycode, action >= 1, action == 2);
        }
        CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
    }

    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods) {
        std::optional<MouseButton> mb;
        switch (button) {
            case 3: mb = MouseButton::Button4; break;
            case 4: mb = MouseButton::Button5; break;
            default: break;
        }
        if (mb) {
            Modifier modifiers = Modifier::None;
            if (mods & GLFW_MOD_SHIFT) {
                modifiers |= Modifier::Shift;
            }
            if (mods & GLFW_MOD_ALT) {
                modifiers |= Modifier::Alt;
            }
            if (mods & GLFW_MOD_CONTROL) {
                modifiers |= Modifier::Control;
            }
            if (auto bind = MouseBind::create(*mb, modifiers)) {
                if (PressBindEvent(bind, action == GLFW_PRESS).post() == ListenerResult::Stop) {
                    return;
                }
            }
        } else {
            return CCEGLView::onGLFWMouseCallBack(window, button, action, mods);
        }
    }
};
#endif

// NOTE: Removed modifications for CCKeyboardDispatcher and MoreOptionsLayer.
// Their functions (dispatchKeyboardMSG, updateModifierKeys, and onKeybindings)
// were causing errors due to missing binding addresses. The original implementations
// will be used instead.

class ControllerChecker : public CCObject {
protected:
    bool m_cached = false;

public:
    void checkController(float) {
        //TODO: remove the android thingy once/if zmx figures out how to get the controller state through the launcher
        bool controllerConnected = GEODE_ANDROID(true || ) PlatformToolbox::isControllerConnected();
        if (m_cached != controllerConnected) {
            m_cached = controllerConnected;
            if (m_cached) {
                BindManager::get()->attachDevice("controller"_spr, &ControllerBind::parse);
                #ifndef GEODE_IS_ANDROID
                Notification::create(
                    "Controller Attached",
                    CCSprite::createWithSpriteFrameName("controllerBtn_A_001.png")
                )->show();
                #endif
            } else {
                BindManager::get()->detachDevice("controller"_spr);
                Notification::create(
                    "Controller Detached",
                    CCSprite::createWithSpriteFrameName("controllerBtn_B_001.png")
                )->show();
            }
        }
    }

    ControllerChecker() {
        this->retain();
    }
};

$execute {
    // Check every second if a controller has been connected.
    Loader::get()->queueInMainThread([] {
        CCScheduler::get()->scheduleSelector(
            schedule_selector(ControllerChecker::checkController),
            new ControllerChecker(), 1.f, false
        );
    });
}

// Have to make a SettingValue even if it holds no value.
class DummySetting : public SettingBaseValue<int> {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const&, std::string const&, matjson::Value const&) {
        return Ok(std::make_shared<DummySetting>());
    };
    SettingNode* createNode(float width) override;
};

class ButtonSettingNode : public SettingValueNode<DummySetting> {
protected:
    bool init(std::shared_ptr<DummySetting>& setting, float width) {
        if (!SettingValueNodeV3::init(setting, width))
            return false;

        this->setContentSize({ width, 40.f });

        auto* sprite = ButtonSprite::create("Edit Keys", 0, false, "bigFont.fnt", "GJ_button_04.png", 24.5f, 0.4f);
        auto* btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(ButtonSettingNode::onOpen));
        auto* menu = CCMenu::create();
        menu->setPosition({ width / 2, 20.f });
        menu->addChild(btn);
        this->addChild(menu);

        return true;
    }
    void onOpen(CCObject*) {
        KeybindsLayer::create()->show();
    }
public:
    void updateState(CCNode* invoker) override {
        SettingValueNodeV3::updateState(invoker);
    }

    static ButtonSettingNode* create(std::shared_ptr<DummySetting> value, float width) {
        auto ret = new ButtonSettingNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

SettingNode* DummySetting::createNode(float width) {
    return ButtonSettingNode::create(std::static_pointer_cast<DummySetting>(shared_from_this()), width);
}

$execute {
    (void) Mod::get()->registerCustomSettingType("open-menu", &DummySetting::parse);
}
