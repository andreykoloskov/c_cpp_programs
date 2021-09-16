#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>

struct BusesForStopResponse {
    bool stop_enable;
    std::string stop;
    std::vector<std::string> bases;
};


std::ostream& operator << (std::ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
    bool bus_enable;
    std::string bus;
    std::vector<std::pair<std::string, std::vector<std::string>>> stops;
};

std::ostream& operator << (std::ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
    bool bus_enable;
    std::vector<std::pair<std::string, std::vector<std::string>>> bases;
};

std::ostream& operator << (std::ostream& os, const AllBusesResponse& r);
