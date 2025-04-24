#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <functional>
#include <algorithm>
#include <limits>

using namespace std;

struct client {
    string ticket;
    int duration;
};

class Queue {
private:
    int NumberOfWindows;
    vector<queue<client>> windows;
    vector<client> visitors;

public:
    Queue(int numWindows): NumberOfWindows(numWindows), windows(numWindows) {}

    void enqueue(int duration, int ticketNumber) {
        string ticket = "T" + to_string(ticketNumber);
        visitors.push_back({ticket, duration});
        cout << ticket << endl;
    }

    void distribute() {
        vector<int> load(NumberOfWindows, 0);

        // Сортировка посетителей по убыванию продолжительности для оптимизации распределения максимальной нагрузки
        sort(visitors.begin(), visitors.end(), [](client &a, client &b) {
            return a.duration > b.duration;
        });

        for (client &v : visitors) {
            int minWindow = distance(load.begin(), min_element(load.begin(), load.end()));
            load[minWindow] += v.duration;
            windows[minWindow].push(v);
        }

        for (int i = 0; i < NumberOfWindows; ++i) {
            cout << "Окно " << i + 1 << " (";
            int time = 0;
            bool NotPrinted = true;
            while (!windows[i].empty()) {
                if (!NotPrinted) {
                    cout << ", ";
                }
                NotPrinted = false;
                client v = windows[i].front(); 
                windows[i].pop();
                cout << v.ticket;
                time += v.duration;
            }
            cout << ") " << time << " минут" << endl;
        }
    }
};

int main() {

    int NumberOfWindows;

    // Проверка на корректный ввод количества окон
    while (true) {
        cout << "Введите кол-во окон: ";
        cin >> NumberOfWindows;

        if (cin.fail() || NumberOfWindows <= 0) { // Проверка на некорректный ввод
            cin.clear(); // Очистка флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование некорректного ввода
            cout << "Ошибка! Пожалуйста, введите положительное число." << endl;
        } else {
            break; // Корректный ввод окон
        }
    }

    Queue queue(NumberOfWindows);

    string command;
    int ticketNumber = 1;

    while (true) {
        cin >> command;

        if (command == "ENQUEUE") {
            int duration;

            // Убираем сообщение о вводе продолжительности обслуживания
            cin >> duration;

            if (cin.fail() || duration <= 0) { // Проверка на некорректный ввод
                cin.clear(); // Очистка флага ошибки
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование некорректного ввода
                cout << "Ошибка! Продолжительность должна быть положительным числом." << endl;
                continue; // Возврат к началу цикла для повторного ввода
            }

            queue.enqueue(duration, ticketNumber);
            ticketNumber++;
            
        } else if (command == "DISTRIBUTE") {
            if (ticketNumber != 1) {
                queue.distribute();
                break;
            } else{
                cout << "Ошибка! Вы не добавили никого в очередь!" << endl;
                break;
            }

        } else if (command == "exit") {
            return 0;

        } else {
            cout << "Ошибка! Неизвестная команда." << endl;
        }
    }

    return 0;
}
