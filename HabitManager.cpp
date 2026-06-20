#include "HabitManager.h"
#include <iostream>
#include <algorithm>

HabitManager::HabitManager() : fileManager("habits.csv") {
    habits = fileManager.loadFromFile();
}

int HabitManager::findIndexById(int id) const {
    for (int i = 0; i < (int)habits.size(); ++i) {
        if (habits[i].getId() == id) return i;
    }
    return -1;
}

int HabitManager::getNextId() const {
    if (habits.empty()) return 1;
    int maxId = 0;
    for (size_t i = 0; i < habits.size(); ++i) {
        if (habits[i].getId() > maxId) maxId = habits[i].getId();
    }
    return maxId + 1;
}

void HabitManager::displayAll() const {
    std::cout << "\n  ==========================================\n";
    std::cout << "           目前追蹤的習慣列表\n";
    std::cout << "  ==========================================\n";

    if (habits.empty()) {
        std::cout << "  （目前沒有任何習慣，請選擇「新增習慣」開始！）\n";
    } else {
        for (size_t i = 0; i < habits.size(); ++i) {
            std::cout << habits[i].toString();
            std::cout << "  ------------------------------------------\n";
        }
    }
    std::cout << "  ==========================================\n\n";
}

void HabitManager::addHabit(const std::string& name) {
    int newId = getNextId();
    habits.push_back(Habit(newId, name));
    fileManager.saveToFile(habits);
    std::cout << "  [成功] 已新增習慣「" << name << "」(ID: " << newId << ")\n";
}

void HabitManager::performCheckIn(int id, const std::string& todayDate) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "  [錯誤] 找不到 ID 為 " << id << " 的習慣。\n";
        return;
    }
    habits[index].checkIn(todayDate);
    fileManager.saveToFile(habits);
}

struct IdMatcher {
    int targetId;
    IdMatcher(int id) : targetId(id) {}
    bool operator()(const Habit& h) const { return h.getId() == targetId; }
};

void HabitManager::removeHabit(int id) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "  [錯誤] 找不到 ID 為 " << id << " 的習慣。\n";
        return;
    }
    std::string removedName = habits[index].getName();
    habits.erase(std::remove_if(habits.begin(), habits.end(), IdMatcher(id)), habits.end());
    fileManager.saveToFile(habits);
    std::cout << "  [成功] 已刪除習慣「" << removedName << "」\n";
}
