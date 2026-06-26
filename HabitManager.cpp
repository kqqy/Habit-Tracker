#include "HabitManager.h"
#include <iostream>
#include <algorithm>
#include <ctime>

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

void HabitManager::displayAll(const std::string& todayDate, const std::string& categoryFilter) const {
    std::cout << "\n  ==========================================\n";
    if (categoryFilter.empty()) {
        std::cout << "           目前追蹤的習慣列表\n";
    } else {
        std::cout << "           目前追蹤的習慣列表 (分類: " << categoryFilter << ")\n";
    }
    std::cout << "  ==========================================\n";

    bool found = false;
    for (size_t i = 0; i < habits.size(); ++i) {
        if (categoryFilter.empty() || habits[i].getCategory() == categoryFilter) {
            std::cout << habits[i].toString(todayDate);
            std::cout << "  ------------------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        if (habits.empty()) {
            std::cout << "  （目前沒有任何習慣，請選擇「新增習慣」開始！）\n";
        } else {
            std::cout << "  （在此分類下找不到任何習慣。）\n";
        }
    }
    std::cout << "  ==========================================\n\n";
}

void HabitManager::addHabit(const std::string& name, const std::string& category) {
    int newId = getNextId();
    habits.push_back(Habit(newId, name, category));
    fileManager.saveToFile(habits);
    std::cout << "  [成功] 已新增習慣「" << name << "」(分類: " << category << ", ID: " << newId << ")\n";
}

void HabitManager::performCheckIn(int id, const std::string& todayDate) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "  [錯誤] 找不到 ID 為 " << id << " 的習慣。\n";
        return;
    }
    std::string prevLastDate = habits[index].getLastDate();
    habits[index].checkIn(todayDate);

    // 如果打卡日期更新了，代表打卡成功，寫入歷史日誌
    if (habits[index].getLastDate() != prevLastDate) {
        fileManager.saveToFile(habits);
        fileManager.logCheckIn(id, habits[index].getName(), todayDate);
    }
}

struct IdMatcher {
    int targetId;
    explicit IdMatcher(int id) : targetId(id) {}
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

bool HabitManager::renameHabit(int id, const std::string& newName) {
    int index = findIndexById(id);
    if (index == -1) {
        std::cout << "  [錯誤] 找不到 ID 為 " << id << " 的習慣。\n";
        return false;
    }
    std::string oldName = habits[index].getName();
    habits[index].setName(newName);
    fileManager.saveToFile(habits);
    std::cout << "  [成功] 已將習慣「" << oldName << "」重新命名為「" << newName << "」。\n";
    return true;
}

void HabitManager::displayStats(const std::string& todayDate) const {
    std::map<int, std::set<std::string>> history = fileManager.loadCheckInHistory();

    std::cout << "\n  ==========================================\n";
    std::cout << "               習慣打卡數據分析\n";
    std::cout << "  ==========================================\n";

    if (habits.empty()) {
        std::cout << "  （目前沒有任何習慣數據可供分析。）\n";
        std::cout << "  ==========================================\n\n";
        return;
    }

    int totalHabits = habits.size();
    int todayChecked = 0;
    for (size_t i = 0; i < habits.size(); ++i) {
        if (habits[i].getLastDate() == todayDate) {
            todayChecked++;
        }
    }

    std::cout << "  總追蹤習慣數：" << totalHabits << " 個\n";
    std::cout << "  今日打卡進度：" << todayChecked << " / " << totalHabits;
    if (totalHabits > 0) {
        std::cout << " (" << (todayChecked * 100 / totalHabits) << "%)";
    }
    std::cout << "\n  ------------------------------------------\n";

    // 取得最近 7 天的日期序列（從 6 天前到今天）
    struct DayInfo {
        std::string dateStr;
        std::string wkLabel;
    };
    std::vector<DayInfo> last7Days;
    time_t t = time(nullptr);
    for (int i = 6; i >= 0; --i) {
        time_t dayTime = t - i * 24 * 3600;
        struct tm dayTm;
        localtime_r(&dayTime, &dayTm);
        char dateBuf[11];
        strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", &dayTm);
        const char* weekdays[] = {"日", "一", "二", "三", "四", "五", "六"};
        DayInfo info;
        info.dateStr = dateBuf;
        info.wkLabel = weekdays[dayTm.tm_wday];
        if (i == 0) {
            info.wkLabel += "(今)";
        }
        last7Days.push_back(info);
    }

    for (size_t i = 0; i < habits.size(); ++i) {
        int id = habits[i].getId();
        int totalCheckIns = history[id].size();
        int activeStreak = habits[i].getActiveStreak(todayDate);
        int bestStreak = habits[i].getBestStreak();

        std::cout << "  習慣: " << habits[i].getName() << " (分類: " << habits[i].getCategory() << ")\n";
        std::cout << "  歷史累計: " << totalCheckIns << " 天 | 連續: " << activeStreak << " 天 | 最佳: " << bestStreak << " 天\n";
        
        // 繪製最近 7 天的 ASCII 打卡網格
        std::cout << "  最近 7 天打卡：\n  ";
        for (size_t d = 0; d < last7Days.size(); ++d) {
            std::cout << "[" << last7Days[d].wkLabel << "] ";
        }
        std::cout << "\n  ";
        for (size_t d = 0; d < last7Days.size(); ++d) {
            if (d == 6) {
                if (history[id].count(last7Days[d].dateStr)) {
                    std::cout << "   ■    ";
                } else {
                    std::cout << "   □    ";
                }
            } else {
                if (history[id].count(last7Days[d].dateStr)) {
                    std::cout << "  ■  ";
                } else {
                    std::cout << "  □  ";
                }
            }
        }
        std::cout << "\n  ------------------------------------------\n";
    }
    std::cout << "  ==========================================\n\n";
}
