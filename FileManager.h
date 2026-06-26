#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "Habit.h"

class FileManager {
private:
    std::string filename;

public:
    explicit FileManager(const std::string& filename = "habits.csv");
    std::vector<Habit> loadFromFile() const;
    void saveToFile(const std::vector<Habit>& habits) const;
    void logCheckIn(int id, const std::string& name, const std::string& date) const;
    std::map<int, std::set<std::string>> loadCheckInHistory() const;
};

#endif // FILEMANAGER_H
