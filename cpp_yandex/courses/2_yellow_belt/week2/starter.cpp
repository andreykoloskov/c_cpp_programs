/*
В курсе «Белый пояс по С++» у нас была задача «Автобусные остановки — 1». В этой задаче вам будет дано её правильное решение, целиком содержащееся внутри функции main. Вам надо будет выполнить декомпозицию этого решения на заранее заданные блоки так, чтобы получившаяся программа так же корректно решала задачу. Условие задачи «Автобусные остановки — 1» приведено ниже.

Исходные файлы
Авторское решение этой задачи содержится в файле
Кроме того, вам дан файл
который содержит заготовки классов и функций. Не меняя функцию main, вам надо реализовать эти классы и функции так, чтобы получившаяся программа решала задачу «Автобусные остановки — 1».

Как будет тестироваться ваша программа
Автоматическая проверяющая система заменит в вашей программе функцию main на ту, которая дана вам в файле starter.cpp, скомпилирует получившийся файл и прогонит на тестах для задачи «Автобусные остановки — 1».

Условие задачи «Автобусные остановки — 1»
Реализуйте систему хранения автобусных маршрутов. Вам нужно обрабатывать следующие запросы:

NEW_BUS busstop_countstop1stop2 ... — добавить маршрут автобуса с названием bus и stop_count остановками с названиями stop1, stop2, ...

BUSES_FOR_STOP stop — вывести названия всех маршрутов автобуса, проходящих через остановку stop.

STOPS_FOR_BUS bus — вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок.

ALL_BUSES — вывести список всех маршрутов с остановками.

Формат ввода
В первой строке ввода содержится количество запросов Q, затем в Q строках следуют описания запросов.

Гарантируется, что все названия маршрутов и остановок состоят лишь из латинских букв, цифр и знаков подчёркивания.

Для каждого запроса NEW_BUS bus stop_count stop1stop2 ... гарантируется, что маршрут bus отсутствует, количество остановок больше 0, а после числа stop_count следует именно такое количество названий остановок, причём все названия в каждом списке различны.

Формат вывода
Для каждого запроса, кроме NEW_BUS, выведите соответствующий ответ на него:

На запрос BUSES_FOR_STOP stop выведите через пробел список автобусов, проезжающих через эту остановку, в том порядке, в котором они создавались командами NEW_BUS. Если остановка stop не существует, выведите No stop.

На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, в котором они были заданы в соответствующей команде NEW_BUS. Описание каждой остановки stop должно иметь вид Stop stop: bus1bus2 ..., где bus1bus2 ... — список автобусов, проезжающих через остановку stop, в порядке, в котором они создавались командами NEW_BUS, за исключением исходного маршрута bus. Если через остановку stop не проезжает ни один автобус, кроме bus, вместо списка автобусов для неё выведите no interchange. Если маршрут bus не существует, выведите No bus.

На запрос ALL_BUSES выведите описания всех автобусов в алфавитном порядке. Описание каждого маршрута bus должно иметь вид Bus bus: stop1stop2 ..., где stop1stop2 ... — список остановок автобуса bus в порядке, в котором они были заданы в соответствующей команде NEW_BUS. Если автобусы отсутствуют, выведите No buses.

Предупреждение
Условие задачи выше содержит много важных деталей. Если вы не уверены в том, что не упустили ни одной, перечитайте условие ещё раз.
*/

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
