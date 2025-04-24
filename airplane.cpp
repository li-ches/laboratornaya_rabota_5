#include "airplane.h"
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

// Хранение данных: имя самолета -> маршрут (города)
static map<string, vector<string>> planesData;
// Хранение данных: город -> имена самолетов, которые через него проходят
static map<string, set<string>> townsData;

CommandType parseCommand(const string& cmd) {
    if (cmd == "CREATE_PLANE") return CommandType::CREATE_PLANE;
    if (cmd == "PLANES_FOR_TOWN") return CommandType::PLANES_FOR_TOWN;
    if (cmd == "TOWNS_FOR_PLANE") return CommandType::TOWNS_FOR_PLANE;
    if (cmd == "PLANES") return CommandType::PLANES;
    return CommandType::UNKNOWN;
}

void createPlane(const string& planeName, const vector<string>& towns) {
    // Удаляем старые данные, если самолет уже существует
    if (planesData.count(planeName)) {
        // Удаляем из townsData упоминания этого самолета
        for (const auto& town : planesData[planeName]) {
            townsData[town].erase(planeName);
            if (townsData[town].empty()) {
                townsData.erase(town);
            }
        }
    }
    
    // Добавляем новые данные
    planesData[planeName] = towns;
    for (const auto& town : towns) {
        townsData[town].insert(planeName);
    }
}

set<string> getPlanesForTown(const string& town) {
    if (townsData.count(town)) {
        return townsData.at(town);
    }
    return {};
}

vector<string> getTownsForPlane(const string& planeName) {
    if (planesData.count(planeName)) {
        return planesData.at(planeName);
    }
    return {};
}

map<string, vector<string>> getAllPlanes() {
    return planesData;
}

void processCommand(const string& command) {
    istringstream iss(command);
    string cmd;
    iss >> cmd;
    
    switch (parseCommand(cmd)) {
        case CommandType::CREATE_PLANE: {
            string planeName;
            iss >> planeName;
            vector<string> towns;
            string town;
            while (iss >> town) {
                towns.push_back(town);
            }
            createPlane(planeName, towns);
            break;
        }
        case CommandType::PLANES_FOR_TOWN: {
            string town;
            iss >> town;
            auto planes = getPlanesForTown(town);
            if (planes.empty()) {
                cout << "Planes is absent" << endl;
            } else {
                for (const auto& plane : planes) {
                    cout << plane << " ";
                }
                cout << endl;
            }
            break;
        }
        case CommandType::TOWNS_FOR_PLANE: {
            string planeName;
            iss >> planeName;
            auto towns = getTownsForPlane(planeName);
            if (towns.empty()) {
                cout << "Towns is absent" << endl;
            } else {
                for (const auto& town : towns) {
                    auto planes = getPlanesForTown(town);
                    planes.erase(planeName); // Исключаем текущий самолет
                    cout << "Stop " << town << ": ";
                    if (planes.empty()) {
                        cout << "0";
                    } else {
                        for (const auto& p : planes) {
                            cout << p << " ";
                        }
                    }
                    cout << endl;
                }
            }
            break;
        }
        case CommandType::PLANES: {
            auto allPlanes = getAllPlanes();
            if (allPlanes.empty()) {
                cout << "Planes is absent" << endl;
            } else {
                for (const auto& [plane, towns] : allPlanes) {
                    cout << "PLANE " << plane << ": ";
                    for (const auto& town : towns) {
                        cout << town << " ";
                    }
                    cout << endl;
                }
            }
            break;
        }
        default:
            cout << "Unknown command" << endl;
    }
}
