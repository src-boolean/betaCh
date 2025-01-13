#include <chrono>
#include <string>
#include <vector>

struct Notification {
    std::string text;
    std::chrono::steady_clock::time_point startTime;
    float duration; // � ��������
    float speedMultiplier; // �������������� ��������� �������� ������������
    ImVec2 position; // ������� ���������
    bool isEntering; // ���� ��� �������� ���������
    float targetY; // ������� ������� �� Y
};

std::vector<Notification> notifications;

