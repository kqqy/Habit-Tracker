#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Habit.h"

class FileManager {
private:
    std::string filename;

public:
    explicit FileManager(const std::string& filename = "habits.csv");
    std::vector<Habit> loadFromFile() const;
    void saveToFile(const std::vector<Habit>& habits) const;
};

#endif // FILEMANAGER_H
