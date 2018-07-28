#include "responses.h"


std::ostream& operator << (std::ostream& os, const BusesForStopResponse& r) {
    if (r.stop_enable == false) {
        os << "No stop";
    } else {
        for (const std::string& bus : r.bases) {
            os << bus << " ";
        }
    }
    return os;
}

std::ostream& operator << (std::ostream& os, const StopsForBusResponse& r) {
    if (r.bus_enable == false) {
        os << "No bus";
    } else {
        int i = 0;
        for (const auto &stop : r.stops) {
            os << "Stop " << stop.first << ": ";
            if (stop.second.size() == 0) {
                os << "no interchange";
            } else {
                for (const std::string &other_bus : stop.second) {
                    os << other_bus << " ";
                }
            }
            if (++i < r.stops.size())
                os << std::endl;
        }
    }
    return os;
}

std::ostream& operator << (std::ostream& os, const AllBusesResponse& r) {
    if (r.bus_enable == false) {
        os << "No buses";
    } else {
        int i = 0;
        for (const auto& bus_item : r.bases) {
            os << "Bus " << bus_item.first << ": ";
            for (const std::string& stop : bus_item.second) {
                os << stop << " ";
            }
            if (++i < r.bases.size())
                os << std::endl;
        }
    }
    return os;
}

