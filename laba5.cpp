#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

const int MAX_CELLS = 1920; // Максимальное количество ячеек
const int NUM_ZONES = 2;
const int NUM_SHELVES_PER_ZONE = 6;
const int NUM_SECTIONS_PER_SHELF = 4;
const int NUM_SHELVES_PER_SECTION = 4;

// Структура для хранения информации о товаре в ячейке
struct Item {
    string name;
    int amount;
};

// Класс для управления складом
class Warehouse {
private:
    map<string, vector<Item>> cells; // Контейнер ячеек и содержащихся в них товаров

public:
    void add(string itemName, int amount, const string& address) {
        auto& items = cells[address]; // получаем ссылку на вектор товаров в данной ячейке
        int summ = 0;
        for (const auto& item : items) {
            summ += item.amount;
        }
        // ищем товар с таким же именем
        for (auto& item : items) {
            if (item.name == itemName) {
                // если товар найден, увеличиваем его количество
                if (summ + amount <= 10) {
                    item.amount += amount;
                    return;
                }
                else {
                    cout << "Невозможно добавить товар " << itemName << " в ячейку " << address << ". Превышен лимит в " << 10 << " единиц." << endl;
                    return;
                }
            }
        }
        // если товар не найден, добавляем новый
        if (summ + amount <= 10) {
            items.push_back({itemName, amount});
        }
        else {
            cout << "Ячейка " << address << " заполнена. Невозможно добавить товар " << itemName << "." << endl;
        }
    }

    void remove(string itemName, int amount, const string& address) {
        auto& items = cells[address];
        for (auto it = items.begin(); it != items.end(); it++) {
            if (it->name == itemName) {
                if (it->amount >= amount) {
                    it->amount -= amount;
                    if (it->amount == 0) {
                        items.erase(it);
                    }
                    return;
                }
                else {
                    cout << "В ячейке " << address << " недостаточно товара " << itemName << "." << endl;
                    return;
                }
            }
        }
        cout << "В ячейке " << address << " нет товара " << itemName << "." << endl;
    }

    void info() {
        int totalItems = 0;
        int totalCells = NUM_ZONES * NUM_SHELVES_PER_ZONE * NUM_SECTIONS_PER_SHELF * NUM_SHELVES_PER_SECTION;
        int occupiedCells = 0;

        for (const auto& cell : cells) {
            if (!cell.second.empty()) {
                occupiedCells++;
                for (const auto& item : cell.second) {
                    totalItems += item.amount;
                }
            }
        }

        cout << "Общая загрузка склада: " << (static_cast<double>(totalItems) / (totalCells * 10)) * 100 << "%" << endl;
        cout << "Загрузка зоны A: " << (static_cast<double>(totalItems) / (totalCells * 10)) * 100 << "%" << endl;

        cout << "Содержимое ячеек:" << endl;
        for (const auto& cell : cells) {
            if (!cell.second.empty()) {
                cout << "Ячейка " << cell.first << ": ";
                for (const auto& item : cell.second) {
                    cout << item.name << " (" << item.amount << "), ";
                }
                cout << endl;
            }
        }

        cout << "Пустые ячейки:" << endl;
        for (int i = 0; i < NUM_ZONES; i++) {
            for (int j = 0; j < NUM_SHELVES_PER_ZONE; j++) {
                for (int k = 0; k < NUM_SECTIONS_PER_SHELF; k++) {
                    for (int l = 0; l < NUM_SHELVES_PER_SECTION; l++) {
                        string address = string(1, 'A' + i) + to_string(j + 1) + to_string(k + 1) + to_string(l + 1);
                        if (cells.find(address) == cells.end() || cells.at(address).empty()) {
                            cout << address << ", ";
                        }
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    Warehouse warehouse;
    string input;
    while (true) {
        cout << "Введите команду: ";
        getline(cin, input);
        stringstream ss(input);
        string command;
        ss >> command;
        if (command == "EXIT") {
            break;
        } else if (command == "ADD" || command == "REMOVE") {
            string itemName, address;
            int amount;
            ss >> itemName >> amount >> address;
            if (command == "ADD") {
                warehouse.add(itemName, amount, address);
            } else {
                warehouse.remove(itemName, amount, address);
            }
        } else if (command == "INFO") {
            warehouse.info();
        } else {
            cout << "Неизвестная команда. Попробуйте еще раз." << endl;
        }
    }
    return 0;
}
