#ifndef HABITMANAGER_H
#define HABITMANAGER_H

#include <vector>
#include <string>
#include "Habit.h"
#include "FileManager.h"

class HabitManager {
private:
    std::vector<Habit> habits;
    FileManager        fileManager;

    int findIndexById(int id) const;
    int getNextId() const;

public:
    HabitManager();
    void displayAll(const std::string& todayDate, const std::string& categoryFilter = "") const;
    void addHabit(const std::string& name, const std::string& category);
    void performCheckIn(int id, const std::string& todayDate);
    void removeHabit(int id);
    bool renameHabit(int id, const std::string& newName);
    void displayStats(const std::string& todayDate) const;
};

#endif // HABITMANAGER_H
