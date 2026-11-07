#pragma once

#include <memory>

class Scene;

class SceneFactory final {
public:
    SceneFactory() = delete;

    static std::unique_ptr<Scene> createSignature();
    static std::unique_ptr<Scene> createFiona();
    static std::unique_ptr<Scene> createClassroom();
    static std::unique_ptr<Scene> createForest();
    static std::unique_ptr<Scene> createSolarSystem();
    static std::unique_ptr<Scene> createLightingTest();
    static std::unique_ptr<Scene> createLitSolarSystem();
    static std::unique_ptr<Scene> createNightForest();

private:
    static void addSkybox(Scene& scene);
};
