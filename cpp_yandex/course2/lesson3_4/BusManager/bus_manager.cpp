#include "bus_manager.h"

void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops) {
    buses_to_stops[bus] = stops;
    for (auto& stop : stops) {
        stops_to_buses[stop].push_back(bus);
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const {
    BusesForStopResponse bs;
    bs.stop_enable = stops_to_buses.count(stop);
    bs.stop = stop;
    if (bs.stop_enable)
        bs.bases = stops_to_buses.at(stop);
    return bs;
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const {
    StopsForBusResponse sb;
    sb.bus_enable = buses_to_stops.count(bus);
    if (sb.bus_enable) {
        std::vector<std::string> stops = buses_to_stops.at(bus);
        for (auto& stop : stops) {
            if (stops_to_buses.count(stop)) {
                std::vector<std::string> buses = stops_to_buses.at(stop);
                std::vector<std::string> new_buses;
                for (auto& new_bus : buses)
                    if (bus != new_bus)
                        new_buses.push_back(new_bus);
                sb.stops.push_back(std::make_pair(stop, new_buses));
            }
        }
    }

    return sb;
}

AllBusesResponse BusManager::GetAllBuses() const {
    AllBusesResponse ab;
    ab.bus_enable = !buses_to_stops.empty();
    if (ab.bus_enable) {
        for (auto& bus_item : buses_to_stops) {
            ab.bases.push_back(std::make_pair(bus_item.first, bus_item.second));
        }
    }

    return ab;
}


