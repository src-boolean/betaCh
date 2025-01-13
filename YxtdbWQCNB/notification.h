#include <chrono>
#include <string>
#include <vector>

struct Notification {
    std::string text;
    std::chrono::steady_clock::time_point startTime;
    float duration; // В секундах
    float speedMultiplier; // Индивидуальный множитель скорости исчезновения
    ImVec2 position; // Текущее положение
    bool isEntering; // Флаг для анимации появления
    float targetY; // Целевая позиция по Y
};

std::vector<Notification> notifications;

