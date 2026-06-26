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

#ifdef _WIN32
#include <windows.h>
#endif

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
 * @brief 獲取昨天系統日期
 * @return 格式為 "YYYY-MM-DD" 的字串
 */
std::string getYesterdayDate() {
    time_t t = time(nullptr) - 24 * 3600;
    struct tm yesterday;
    localtime_r(&t, &yesterday);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &yesterday);
    return std::string(buf);
}

/**
 * @brief 顯示主選單
 */
void displayMenu() {
    std::cout << "\n======= 習慣養成打卡器 =======\n";
    std::cout << "1. 顯示所有習慣\n";
    std::cout << "2. 新增習慣\n";
    std::cout << "3. 進行習慣打卡 (含補卡)\n";
    std::cout << "4. 修改習慣名稱\n";
    std::cout << "5. 刪除習慣\n";
    std::cout << "6. 顯示統計分析\n";
    std::cout << "7. 離開系統\n";
    std::cout << "請選擇功能 (1-7)：";
}

int main() {
    // 設定編碼（Windows 系統上處理中文輸入與輸出）
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif

    HabitManager manager;
    std::string today = getCurrentDate();
    std::string yesterday = getYesterdayDate();
    int choice;

    std::cout << "=== 歡迎使用習慣養成打卡器 ===\n";
    std::cout << "今天是：" << today << std::endl;

    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break;
            }
            std::cout << "請輸入有效的數字！\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 7) break;

        switch (choice) {
            case 1: {
                std::cout << "\n請選擇篩選方式：\n";
                std::cout << "1. 顯示全部\n";
                std::cout << "2. 篩選「運動」\n";
                std::cout << "3. 篩選「學習」\n";
                std::cout << "4. 篩選「健康」\n";
                std::cout << "5. 篩選「工作」\n";
                std::cout << "6. 輸入自訂分類篩選\n";
                std::cout << "請選擇 (1-6)：";
                int filterChoice;
                std::cin >> filterChoice;
                std::cin.ignore(1000, '\n');

                if (filterChoice == 1) {
                    manager.displayAll(today);
                } else if (filterChoice == 2) {
                    manager.displayAll(today, "運動");
                } else if (filterChoice == 3) {
                    manager.displayAll(today, "學習");
                } else if (filterChoice == 4) {
                    manager.displayAll(today, "健康");
                } else if (filterChoice == 5) {
                    manager.displayAll(today, "工作");
                } else if (filterChoice == 6) {
                    std::cout << "請輸入要篩選的分類名稱：";
                    std::string cat;
                    std::getline(std::cin, cat);
                    manager.displayAll(today, cat);
                } else {
                    std::cout << "無效的選擇，顯示所有習慣。\n";
                    manager.displayAll(today);
                }
                break;
            }
            case 2: {
                std::cin.ignore(1000, '\n');
                std::cout << "請輸入習慣名稱：";
                std::string name;
                std::getline(std::cin, name);
                if (name.empty()) {
                    std::cout << "名稱不能為空！\n";
                    break;
                }

                std::cout << "請選擇習慣分類：\n";
                std::cout << "1. 運動\n";
                std::cout << "2. 學習\n";
                std::cout << "3. 健康\n";
                std::cout << "4. 工作\n";
                std::cout << "5. 輸入自訂分類\n";
                std::cout << "請選擇 (1-5)：";
                int catChoice;
                std::cin >> catChoice;
                std::cin.ignore(1000, '\n');

                std::string category = "一般";
                if (catChoice == 1) category = "運動";
                else if (catChoice == 2) category = "學習";
                else if (catChoice == 3) category = "健康";
                else if (catChoice == 4) category = "工作";
                else if (catChoice == 5) {
                    std::cout << "請輸入自訂分類名稱：";
                    std::getline(std::cin, category);
                    if (category.empty()) category = "一般";
                }
                
                manager.addHabit(name, category);
                break;
            }
            case 3: {
                manager.displayAll(today);
                std::cout << "請輸入要打卡的習慣 ID：";
                int id;
                std::cin >> id;
                std::cin.ignore(1000, '\n');

                std::cout << "請選擇打卡日期：\n";
                std::cout << "1. 今日打卡 (" << today << ")\n";
                std::cout << "2. 昨天補卡 (" << yesterday << ")\n";
                std::cout << "3. 輸入自訂日期打卡 (格式: YYYY-MM-DD)\n";
                std::cout << "請選擇 (1-3)：";
                int dateChoice;
                std::cin >> dateChoice;
                std::cin.ignore(1000, '\n');

                std::string checkInDate = today;
                if (dateChoice == 2) {
                    checkInDate = yesterday;
                } else if (dateChoice == 3) {
                    std::cout << "請輸入日期 (YYYY-MM-DD)：";
                    std::getline(std::cin, checkInDate);
                    if (checkInDate.length() != 10) {
                        std::cout << "  [錯誤] 日期格式不正確！\n";
                        break;
                    }
                }

                manager.performCheckIn(id, checkInDate);
                break;
            }
            case 4: {
                manager.displayAll(today);
                std::cout << "請輸入要修改名稱的習慣 ID：";
                int id;
                std::cin >> id;
                std::cin.ignore(1000, '\n');

                std::cout << "請輸入新的習慣名稱：";
                std::string newName;
                std::getline(std::cin, newName);
                if (!newName.empty()) {
                    manager.renameHabit(id, newName);
                } else {
                    std::cout << "名稱不能為空！\n";
                }
                break;
            }
            case 5: {
                manager.displayAll(today);
                std::cout << "請輸入要刪除的習慣 ID：";
                int id;
                std::cin >> id;
                std::cin.ignore(1000, '\n');
                manager.removeHabit(id);
                break;
            }
            case 6: {
                manager.displayStats(today);
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
