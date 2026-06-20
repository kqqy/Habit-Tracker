#ifndef HABIT_H
#define HABIT_H

#include <string>

class Habit {
private:
    // ── 資料屬性 ──────────────────────────────────────────────
    int         id;            ///< 習慣的唯一識別碼
    std::string name;          ///< 習慣名稱
    int         currentStreak; ///< 目前連續打卡天數
    int         bestStreak;    ///< 歷史最佳連續打卡天數
    std::string lastDate;      ///< 最後打卡日期（格式: "YYYY-MM-DD"；從未打卡則為空字串）

    // ── 私有靜態輔助方法 ───────────────────────────────────────
    static std::string getPreviousDay(const std::string& date);

public:
    // ── 建構子 ────────────────────────────────────────────────
    Habit(int id, const std::string& name, int currentStreak,
          int bestStreak, const std::string& lastDate);

    Habit(int id, const std::string& name);

    // ── 核心業務方法 ──────────────────────────────────────────
    void checkIn(const std::string& todayDate);

    // ── Getters ────────────────────
    int         getId()            const;
    std::string getName()          const;
    int         getCurrentStreak() const;
    int         getBestStreak()    const;
    std::string getLastDate()      const;

    // ── Setters ───────────────────────────────────────────────
    void setName(const std::string& newName);

    // 序列化輔助方法
    std::string toCSV() const;
    std::string toString() const;
};

#endif // HABIT_H
