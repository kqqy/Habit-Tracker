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
    std::getline(file, line); // 跳過標頭

    while (std::getline(file, line)) {
        if (!line.empty() && line[line.length()-1] == '\r') line.erase(line.length()-1);
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> fields;
        while (std::getline(ss, token, ',')) {
            fields.push_back(token);
        }

        if (fields.size() < 4) continue; // 無效行

        int id = atoi(fields[0].c_str());
        std::string name = fields[1];
        std::string category = "一般";
        int currentStreak = 0;
        int bestStreak = 0;
        std::string lastDate = "";

        if (fields.size() == 5) { // 舊版格式: id,name,current_streak,best_streak,last_date
            currentStreak = atoi(fields[2].c_str());
            bestStreak = atoi(fields[3].c_str());
            lastDate = fields[4];
        } else if (fields.size() >= 6) { // 新版格式: id,name,category,current_streak,best_streak,last_date
            category = fields[2];
            currentStreak = atoi(fields[3].c_str());
            bestStreak = atoi(fields[4].c_str());
            lastDate = fields[5];
        }

        if (!lastDate.empty() && (lastDate[lastDate.length()-1] == '\r' || lastDate[lastDate.length()-1] == '\n')) {
            lastDate.erase(lastDate.length()-1);
        }

        habits.push_back(Habit(id, name, category, currentStreak, bestStreak, lastDate));
    }
    file.close();
    return habits;
}

void FileManager::saveToFile(const std::vector<Habit>& habits) const {
    std::ofstream file(filename.c_str());
    if (!file.is_open()) return;

    // 寫入 UTF-8 BOM (隱形標記)，讓 Excel 和記事本能正確辨識中文
    file << "\xEF\xBB\xBF";

    file << "id,name,category,current_streak,best_streak,last_date\n";
    for (size_t i = 0; i < habits.size(); ++i) {
        file << habits[i].toCSV() << "\n";
    }
    file.close();
}

void FileManager::logCheckIn(int id, const std::string& name, const std::string& date) const {
    // 檢查檔案是否存在來決定是否寫入標頭
    std::ifstream checkFile("checkins.csv");
    bool exists = checkFile.good();
    checkFile.close();

    std::ofstream file("checkins.csv", std::ios::app);
    if (!file.is_open()) return;

    if (!exists) {
        // 寫入 UTF-8 BOM
        file << "\xEF\xBB\xBF";
        file << "id,name,date\n";
    }

    file << id << "," << name << "," << date << "\n";
    file.close();
}

std::map<int, std::set<std::string>> FileManager::loadCheckInHistory() const {
    std::map<int, std::set<std::string>> history;
    std::ifstream file("checkins.csv");
    if (!file.is_open()) return history;

    std::string line;
    // 讀取標頭
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (!line.empty() && line[line.length()-1] == '\r') line.erase(line.length()-1);
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string idToken, nameToken, dateToken;

        if (std::getline(ss, idToken, ',') &&
            std::getline(ss, nameToken, ',') &&
            std::getline(ss, dateToken, ',')) {
            
            int id = atoi(idToken.c_str());
            if (!dateToken.empty() && (dateToken[dateToken.length()-1] == '\r' || dateToken[dateToken.length()-1] == '\n')) {
                dateToken.erase(dateToken.length()-1);
            }
            history[id].insert(dateToken);
        }
    }
    file.close();
    return history;
}
