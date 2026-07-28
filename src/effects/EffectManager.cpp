#include "EffectManager.h"
#include "../core/Storage.h"

EffectManager FX;

EffectManager::EffectManager() {}

void EffectManager::begin() {
    if (effects.size() > 0) {
        String startupFx = Storage.settings.startupEffect;
        if (!setEffect(startupFx)) {
            setEffect(effects[0]->getName());
        }
    }
}

void EffectManager::addEffect(Effect* effect) {
    if (effect != nullptr) {
        effects.push_back(effect);
    }
}

void EffectManager::run() {
    if (currentEffectIndex >= 0 && currentEffectIndex < effects.size()) {
        effects[currentEffectIndex]->run();
    }
}

bool EffectManager::setEffect(const String& name) {
    for (int i = 0; i < effects.size(); i++) {
        if (name.equalsIgnoreCase(effects[i]->getName())) {
            if (currentEffectIndex >= 0) {
                effects[currentEffectIndex]->stop();
            }
            currentEffectIndex = i;
            effects[currentEffectIndex]->start();
            return true;
        }
    }
    return false;
}

bool EffectManager::nextEffect() {
    if (effects.empty()) return false;
    
    if (currentEffectIndex >= 0) {
        effects[currentEffectIndex]->stop();
    }
    
    currentEffectIndex++;
    if (currentEffectIndex >= effects.size()) {
        currentEffectIndex = 0;
    }
    
    effects[currentEffectIndex]->start();
    return true;
}

bool EffectManager::prevEffect() {
    if (effects.empty()) return false;
    
    if (currentEffectIndex >= 0) {
        effects[currentEffectIndex]->stop();
    }
    
    currentEffectIndex--;
    if (currentEffectIndex < 0) {
        currentEffectIndex = effects.size() - 1;
    }
    
    effects[currentEffectIndex]->start();
    return true;
}

Effect* EffectManager::getCurrentEffect() {
    if (currentEffectIndex >= 0 && currentEffectIndex < effects.size()) {
        return effects[currentEffectIndex];
    }
    return nullptr;
}

std::vector<String> EffectManager::getEffectList() {
    std::vector<String> list;
    for (auto* effect : effects) {
        list.push_back(effect->getName());
    }
    return list;
}
