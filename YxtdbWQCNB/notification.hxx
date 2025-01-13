#include"notification.h"

namespace snotification {
    // Добавить новое уведомление
    void AddNotification(const std::string& text, float duration = 5.0f, float speedMultiplier = 1.0f) {
        float windowWidth = ImGui::GetIO().DisplaySize.x; // Ширина окна
        ImVec2 initialPos = ImVec2(windowWidth - 260.0f, -30.0f); // Начальная позиция за пределами видимой области
        float targetY = 10.0f; // Целевая позиция

        // Смещаем новое уведомление ниже последнего
        if (!notifications.empty()) {
            const Notification& lastNotification = notifications.back();
            targetY = lastNotification.targetY + 40.0f; // Смещение по высоте уведомления
        }

        notifications.push_back(Notification{ text, std::chrono::steady_clock::now(), duration / speedMultiplier, speedMultiplier, initialPos, true, targetY });
    }

    // Рекурсивное обновление целевых позиций уведомлений
    void UpdateTargetPositions(int index) {
        if (index >= notifications.size()) return; // Базовый случай: конец списка
        notifications[index].targetY -= 40.0f; // Смещаем целевую позицию
        UpdateTargetPositions(index + 1); // Обрабатываем следующий элемент
    }

    // Рекурсивная отрисовка уведомлений
    void RenderNotificationRecursive(int index) {
        if (index < 0) return;

        auto now = std::chrono::steady_clock::now();
        Notification& notification = notifications[index];
        float elapsed = std::chrono::duration<float>(now - notification.startTime).count();

        // Если время жизни истекло, удаляем уведомление
        if (elapsed > notification.duration) {
            notifications.erase(notifications.begin() + index);

            // Обновить целевые позиции оставшихся уведомлений
            UpdateTargetPositions(index);

            // Обрабатываем следующий элемент
            RenderNotificationRecursive(index - 1);
            return;
        }

        // Плавная анимация движения к целевой позиции
        float moveSpeed = 300.0f; // Скорость движения (пиксели в секунду)
        if (notification.position.y < notification.targetY) {
            notification.position.y += moveSpeed * ImGui::GetIO().DeltaTime;
            if (notification.position.y > notification.targetY) {
                notification.position.y = notification.targetY; // Зафиксировать на целевой позиции
            }
        }
        else if (notification.position.y > notification.targetY) {
            notification.position.y -= moveSpeed * ImGui::GetIO().DeltaTime;
            if (notification.position.y < notification.targetY) {
                notification.position.y = notification.targetY; // Зафиксировать на целевой позиции
            }
        }

        // Рассчитываем прозрачность (альфа-канал) от 1.0 до 0.0
        float alpha = 1.0f - (elapsed / notification.duration);

        // Устанавливаем прозрачность перед отрисовкой окна
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 260.0f, notification.position.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(250, 30)); // Размер уведомления
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha); // Устанавливаем прозрачность

        ImGui::Begin(("Notification##" + std::to_string(index)).c_str(), nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings);

        // Вывод текста уведомления
        ImGui::Text("%s", notification.text.c_str());
        ImGui::End();

        ImGui::PopStyleVar(); // Восстанавливаем стиль

        // Обрабатываем следующий элемент
        RenderNotificationRecursive(index - 1);
    }

    // Отрисовка уведомлений
    void RenderNotifications() {
        if (!notifications.empty()) {
            RenderNotificationRecursive(static_cast<int>(notifications.size()) - 1);
        }
    }

}

