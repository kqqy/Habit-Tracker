// ============================================================
// main.cpp — 使用者介面層 (User Interface)
//
// 職責：
//   1. 處理與使用者的互動（顯示選單、讀取輸入）。
//   2. 獲取當前系統日期。
//   3. 呼叫 HabitManager 執行對應的業務邏輯。
// ============================================================
#include <iostream>
#include <string>
#include <time.h>
#include <iomanip>
#include "HabitManager.h"

/**
 * @brief 獲取當前系統日期
 * @return 格式為 "YYYY-MM-DD" 的字串
 */
std::string getCurrentDate() {
    time_t t = time(nullptr);
    struct tm now;
	localtime_r(&t, &now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &now);
    return std::string(buf);
}

/**
 * @brief 顯示主選單
 */
void displayMenu() {
    std::cout << "\n======= 習慣養成打卡器 =======\n";
    std::cout << "1. 顯示所有習慣\n";
    std::cout << "2. 新增習慣\n";
    std::cout << "3. 進行今日打卡\n";
    std::cout << "4. 刪除習慣\n";
    std::cout << "5. 離開系統\n";
    std::cout << "請選擇功能 (1-5)：";
}

int main() {
    // 設定編碼（Windows 系統上處理中文輸出）
    system("chcp 65001 > nul"); 

    HabitManager manager;
    std::string today = getCurrentDate();
    int choice;

    std::cout << "=== 歡迎使用習慣養成打卡器 ===\n";
    std::cout << "今天是：" << today << std::endl;

    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "請輸入有效的數字！\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 5) break;

        switch (choice) {
            case 1:
                manager.displayAll();
                break;
            case 2: {
                std::cout << "請輸入習慣名稱：";
                std::string name;
                std::cin.ignore();
                std::getline(std::cin, name);
                if (!name.empty()) {
                    manager.addHabit(name);
                } else {
                    std::cout << "名稱不能為空！\n";
                }
                break;
            }
            case 3: {
                manager.displayAll();
                std::cout << "請輸入要打卡的習慣 ID：";
                int id;
                std::cin >> id;
                manager.performCheckIn(id, today);
                break;
            }
            case 4: {
                manager.displayAll();
                std::cout << "請輸入要刪除的習慣 ID：";
                int id;
                std::cin >> id;
                manager.removeHabit(id);
                break;
            }
            default:
                std::cout << "無效的選項，請重新輸入。\n";
                break;
        }
    }

    std::cout << "再見！記得每天堅持打卡喔！\n";
    return 0;
}
