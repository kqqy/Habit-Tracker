#include "Habit.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <stdexcept>
#include <cstdlib>

// 計算前一天，用於判斷是否「連續」
std::string Habit::getPreviousDay(const std::string& date) {
    if (date.length() != 10) return "";
    struct tm t = {0};
    
    int y = atoi(date.substr(0, 4).c_str());
    int m = atoi(date.substr(5, 2).c_str());
    int d = atoi(date.substr(8, 2).c_str());
    
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d - 1;
    mktime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
    return std::string(buf);
}

Habit::Habit(int id, const std::string& name, const std::string& category,
             int currentStreak, int bestStreak, const std::string& lastDate)
    : id(id), name(name), category(category), currentStreak(currentStreak),
      bestStreak(bestStreak), lastDate(lastDate) {}

Habit::Habit(int id, const std::string& name, const std::string& category)
    : id(id), name(name), category(category), currentStreak(0), bestStreak(0), lastDate("") {}

void Habit::checkIn(const std::string& todayDate) {
    if (!lastDate.empty() && todayDate < lastDate) {
        std::cout << "  [錯誤] 不能為最後打卡日期 (" << lastDate << ") 之前的日期補打卡。\n";
        return;
    }

    if (lastDate == todayDate) {
        std::cout << "  [已打卡] 「" << name << "」今日已完成打卡，繼續保持！\n";
        return;
    }

    std::string yesterday = getPreviousDay(todayDate);

    if (!lastDate.empty() && lastDate == yesterday) {
        currentStreak++;
        std::cout << "  [連續] 太棒了！「" << name << "」連續打卡 "
                  << currentStreak << " 天！\n";
    } else {
        if (!lastDate.empty()) {
            std::cout << "  [重置] 「" << name << "」連續紀錄中斷（上次: " << lastDate
                      << "），重新從第 1 天開始！\n";
        } else {
            std::cout << "  [開始] 「" << name << "」今天開始追蹤，加油！\n";
        }
        currentStreak = 1;
    }

    if (currentStreak > bestStreak) {
        bestStreak = currentStreak;
        std::cout << "  [新紀錄] 最佳紀錄更新為 " << bestStreak << " 天！\n";
    }

    lastDate = todayDate;
}

int         Habit::getId()            const { return id; }
std::string Habit::getName()          const { return name; }
std::string Habit::getCategory()      const { return category; }
int         Habit::getCurrentStreak() const { return currentStreak; }
int         Habit::getBestStreak()    const { return bestStreak; }
std::string Habit::getLastDate()      const { return lastDate; }

int Habit::getActiveStreak(const std::string& todayDate) const {
    if (lastDate.empty()) return 0;
    if (lastDate == todayDate) return currentStreak;
    std::string yesterday = getPreviousDay(todayDate);
    if (lastDate == yesterday) return currentStreak;
    return 0; // Streak broken
}

void Habit::setName(const std::string& newName) { name = newName; }
void Habit::setCategory(const std::string& newCategory) { category = newCategory; }

std::string Habit::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << name << "," << category << "," << currentStreak << ","
        << bestStreak << "," << lastDate;
    return oss.str();
}

std::string Habit::toString(const std::string& todayDate) const {
    std::ostringstream oss;
    int activeStreak = getActiveStreak(todayDate);
    oss << "  [ID:" << std::setw(2) << std::right << id << "] " 
        << std::setw(15) << std::left << name 
        << " (分類: " << category << ")\n";
    oss << "         連續天數: " << std::setw(3) << activeStreak << " 天"
        << " | 最佳紀錄: " << std::setw(3) << bestStreak << " 天"
        << " | 最後打卡: "
        << (lastDate.empty() ? "尚未打卡" : lastDate)
        << "\n";
    return oss.str();
}
