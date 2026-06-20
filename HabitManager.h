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
    void displayAll() const;
    void addHabit(const std::string& name);
    void performCheckIn(int id, const std::string& todayDate);
    void removeHabit(int id);
};

#endif // HABITMANAGER_H
