#include "RenderManager.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

RenderManager::RenderManager() {}

void RenderManager::EnsureShadowResources() {
    if (shadowMaps.size() >= Lights.size()) return;
    shadowMaps.resize(Lights.size());
    lightSpaceMats.resize(Lights.size(), glm::mat4(1.0f));

    for (size_t i = 0; i < Lights.size(); i++) {
        ShadowData& sd = shadowMaps[i];
        if (sd.FBO) continue;

        glGenFramebuffers(1, &sd.FBO);
        glGenTextures(1, &sd.depthMap);
        glBindTexture(GL_TEXTURE_2D, sd.depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1,1,1,1};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, sd.FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sd.depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void RenderManager::RenderShadowPass(size_t i, Shader& shadowShader) {
    LightEntity* L = Lights[i];
    ShadowData& sd = shadowMaps[i];

    glm::mat4 lightProj, lightView;
    if (L->Type == LightType::Global) {
        float nearP = 1.0f, farP = 50.0f, s = 20.0f;
        lightProj = glm::ortho(-s, s, -s, s, nearP, farP);
        glm::vec3 eye = -glm::normalize(L->getDirection()) * 10.0f;
        lightView = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    } else { // Spot
        float nearP = 0.1f, farP = L->getRadius();
        float fovy = glm::degrees(2.0f * acosf(L->getCutOff()));
        lightProj = glm::perspective(glm::radians(fovy), 1.0f, nearP, farP);
        lightView = glm::lookAt(L->Position, L->Position + L->getDirection(), glm::vec3(0,1,0));
    }

    glm::mat4 lightSpace = lightProj * lightView;
    lightSpaceMats[i] = lightSpace;

    shadowShader.use();
    shadowShader.setMat4("lightSpaceMatrix", lightSpace);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, sd.FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto* mesh : Meshes) mesh->Draw(shadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManager::RenderAll(Shader& sceneShader, Shader& shadowShader) {
    if (!ActiveCamera || Lights.empty()) return;
    EnsureShadowResources();

    for (size_t i = 0; i < Lights.size(); i++) {
        if (Lights[i]->Type == LightType::Global || Lights[i]->Type == LightType::Spot)
            RenderShadowPass(i, shadowShader);
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = ActiveCamera->GetView();
    glm::mat4 proj = ActiveCamera->GetProjection();

    sceneShader.use();
    sceneShader.setMat4("view", view);
    sceneShader.setMat4("projection", proj);
    sceneShader.setVec3("viewPos", ActiveCamera->Position);

    sceneShader.setInt("numLights", (int)Lights.size());
    for (size_t i = 0; i < Lights.size(); i++) {
        std::string p = "lights[" + std::to_string(i) + "].";
        LightEntity* L = Lights[i];
        sceneShader.setInt (p+"type", (int)L->Type);
        sceneShader.setVec3(p+"position", L->Position);
        sceneShader.setVec3(p+"direction", L->getDirection());
        sceneShader.setVec3(p+"color", L->getColor());
        sceneShader.setFloat(p+"intensity", L->getIntensity());
        sceneShader.setFloat(p+"radius", L->getRadius());
        sceneShader.setFloat(p+"cutOff", L->getCutOff());
        sceneShader.setFloat(p+"outerCutOff", L->getOuterCutOff());

        sceneShader.setMat4("lightSpaceMatrix", lightSpaceMats[i]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadowMaps[i].depthMap);
        sceneShader.setInt("shadowMap", 3);
    }

    for (auto* mesh : Meshes) mesh->Draw(sceneShader);
}

void RenderManager::OnResize(int w, int h) {
    windowWidth = w; windowHeight = h;
    glViewport(0, 0, w, h);
    if (ActiveCamera) ActiveCamera->SetAspect(float(w) / float(h));
}
