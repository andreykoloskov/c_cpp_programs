#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
  // Реализуйте эту функцию
  string operation_code;
  is >> operation_code;
  if (operation_code == "NEW_BUS") {
    q.type = QueryType::NewBus;
    is >> q.bus;
    int stop_count;
    is >> stop_count;
    vector<string>& stops = q.stops;
    stops.resize(stop_count);
    for (string& stop : stops) {
      is >> stop;
    }
  } else if (operation_code == "BUSES_FOR_STOP") {
    q.type = QueryType::BusesForStop;
    is >> q.stop;
  } else if (operation_code == "STOPS_FOR_BUS") {
    q.type = QueryType::StopsForBus;
    is >> q.bus;
  } else if (operation_code == "ALL_BUSES") {
    q.type = QueryType::AllBuses;
  }
  return is;
}

struct BusesForStopResponse {
    bool stop_enable;
    string stop;
    vector<string> bases;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.stop_enable == false) {
        os << "No stop";
    } else {
        for (const string& bus : r.bases) {
            os << bus << " ";
        }
    }
    return os;
}

struct StopsForBusResponse {
    bool bus_enable;
    string bus;
    vector<pair<string, vector<string>>> stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    if (r.bus_enable == false) {
        os << "No bus";
    } else {
        int i = 0;
        for (const auto &stop : r.stops) {
            os << "Stop " << stop.first << ": ";
            if (stop.second.size() == 0) {
                os << "no interchange";
            } else {
                for (const string &other_bus : stop.second) {
                    os << other_bus << " ";
                }
            }
            if (++i < r.stops.size())
                os << endl;
        }
    }
    return os;
}

struct AllBusesResponse {
    bool bus_enable;
    vector<pair<string, vector<string>>> bases;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    if (r.bus_enable == false) {
        os << "No buses";
    } else {
        int i = 0;
        for (const auto& bus_item : r.bases) {
            os << "Bus " << bus_item.first << ": ";
            for (const string& stop : bus_item.second) {
                os << stop << " ";
            }
            if (++i < r.bases.size())
                os << endl;
        }
    }
  return os;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
    // Реализуйте этот метод
    buses_to_stops[bus] = stops;
    for (auto& stop : stops) {
      stops_to_buses[stop].push_back(bus);
    }
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
      BusesForStopResponse bs;
      bs.stop_enable = stops_to_buses.count(stop);
      bs.stop = stop;
      if (bs.stop_enable)
          bs.bases = stops_to_buses.at(stop);
      return bs;
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
      StopsForBusResponse sb;
      sb.bus_enable = buses_to_stops.count(bus);
      if (sb.bus_enable) {
          vector<string> stops = buses_to_stops.at(bus);
          for (auto& stop : stops) {
              if (stops_to_buses.count(stop)) {
                  vector<string> buses = stops_to_buses.at(stop);
                  vector<string> new_buses;
                  for (auto& new_bus : buses)
                      if (bus != new_bus)
                        new_buses.push_back(new_bus);
                  sb.stops.push_back(make_pair(stop, new_buses));
              }
          }
      }

      return sb;
  }

  AllBusesResponse GetAllBuses() const {
      AllBusesResponse ab;
      ab.bus_enable = !buses_to_stops.empty();
      if (ab.bus_enable) {
          for (auto& bus_item : buses_to_stops) {
              ab.bases.push_back(make_pair(bus_item.first, bus_item.second));
          }
      }

      return ab;
  }

private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}
