#include"notification.h"

namespace snotification {
    
    void AddNotification(const std::string& text, float duration = 5.0f, float speedMultiplier = 1.0f) {
        float windowWidth = ImGui::GetIO().DisplaySize.x; 
        ImVec2 initialPos = ImVec2(windowWidth - 260.0f, -30.0f); 
        float targetY = 10.0f; 

        
        if (!notifications.empty()) {
            const Notification& lastNotification = notifications.back();
            targetY = lastNotification.targetY + 40.0f; 
        }

        notifications.push_back(Notification{ text, std::chrono::steady_clock::now(), duration / speedMultiplier, speedMultiplier, initialPos, true, targetY });
    }

    
    void UpdateTargetPositions(int index) {
        if (index >= notifications.size()) return; 
        notifications[index].targetY -= 40.0f; 
        UpdateTargetPositions(index + 1); 
    }

    
    void RenderNotificationRecursive(int index) {
        if (index < 0) return;

        auto now = std::chrono::steady_clock::now();
        Notification& notification = notifications[index];
        float elapsed = std::chrono::duration<float>(now - notification.startTime).count();

        
        if (elapsed > notification.duration) {
            notifications.erase(notifications.begin() + index);

            
            UpdateTargetPositions(index);

            
            RenderNotificationRecursive(index - 1);
            return;
        }

        
        float moveSpeed = 300.0f; 
        if (notification.position.y < notification.targetY) {
            notification.position.y += moveSpeed * ImGui::GetIO().DeltaTime;
            if (notification.position.y > notification.targetY) {
                notification.position.y = notification.targetY;
            }
        }
        else if (notification.position.y > notification.targetY) {
            notification.position.y -= moveSpeed * ImGui::GetIO().DeltaTime;
            if (notification.position.y < notification.targetY) {
                notification.position.y = notification.targetY; 
            }
        }

        
        float alpha = 1.0f - (elapsed / notification.duration);

        
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 260.0f, notification.position.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(250, 30)); 
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha); 

        ImGui::Begin(("Notification##" + std::to_string(index)).c_str(), nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings);

        
        ImGui::Text("%s", notification.text.c_str());
        ImGui::End();

        ImGui::PopStyleVar(); 

        
        RenderNotificationRecursive(index - 1);
    }

    
    void RenderNotifications() {
        if (!notifications.empty()) {
            RenderNotificationRecursive(static_cast<int>(notifications.size()) - 1);
        }
    }

}

