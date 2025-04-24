#ifndef AIRPLANE_H
#define AIRPLANE_H

#include <string>
#include <vector>
#include <map>
#include <set>

enum class CommandType {
    CREATE_PLANE,
    PLANES_FOR_TOWN,
    TOWNS_FOR_PLANE,
    PLANES,
    UNKNOWN
};

void processCommand(const std::string& command);
CommandType parseCommand(const std::string& cmd);

// Функции для работы с данными
void createPlane(const std::string& planeName, const std::vector<std::string>& towns);
std::set<std::string> getPlanesForTown(const std::string& town);
std::vector<std::string> getTownsForPlane(const std::string& planeName);
std::map<std::string, std::vector<std::string>> getAllPlanes();

#endif // AIRPLANE_H
