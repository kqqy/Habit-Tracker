#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

FileManager::FileManager(const std::string& filename) : filename(filename) {}

std::vector<Habit> FileManager::loadFromFile() const {
    std::vector<Habit> habits;
    std::ifstream file(filename.c_str());
    if (!file.is_open()) return habits;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (!line.empty() && line[line.length()-1] == '\r') line.erase(line.length()-1);
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        int id = atoi(token.c_str());

        std::string name;
        std::getline(ss, name, ',');

        std::getline(ss, token, ',');
        int currentStreak = atoi(token.c_str());

        std::getline(ss, token, ',');
        int bestStreak = atoi(token.c_str());

        std::string lastDate;
        std::getline(ss, lastDate, ',');
        if (!lastDate.empty() && (lastDate[lastDate.length()-1] == '\r' || lastDate[lastDate.length()-1] == '\n')) {
            lastDate.erase(lastDate.length()-1);
        }

        habits.push_back(Habit(id, name, currentStreak, bestStreak, lastDate));
    }
    file.close();
    return habits;
}

void FileManager::saveToFile(const std::vector<Habit>& habits) const {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) return;

    // 寫入 UTF-8 BOM (隱形標記)，讓 Excel 和記事本能正確辨識中文
    file << "\xEF\xBB\xBF";

    file << "id,name,current_streak,best_streak,last_date\n";
    for (size_t i = 0; i < habits.size(); ++i) {
        file << habits[i].toCSV() << "\n";
    }
    file.close();
}
