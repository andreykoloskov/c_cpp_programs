#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <algorithm>
#include <numeric>
#include <queue>


using namespace std;

/*
class Booking {
private:
    static const int SECONDS_PER_DAY = 86400;
    map<string, map<int64_t, int>> clients_count_;
    map<string, map<int64_t, int>> rooms_count_;
    int64_t current_time_;

public:
    Booking()
        : current_time_{0}
    {}

    ~Booking() {}

    void Book(int64_t time, string hotel_name, int client_id, int room_count)
    {
        current_time_ = time;
        ++clients_count_[hotel_name][time];
        rooms_count_[hotel_name][time] += room_count;
    }

    int64_t Clients(string hotel_name)
    {
        if (clients_count_.find(hotel_name) == clients_count_.end()) {
            return 0;
        }

        auto clients_in_hotel = clients_count_[hotel_name];
        int64_t clients = 0;
        auto low = clients_in_hotel.upper_bound(current_time_ - SECONDS_PER_DAY);
        for (auto i = low; i != clients_in_hotel.end(); ++i) {
            clients += i->second;
        }
        return clients;
        //return accumulate(low, clients_in_hotel.end(), clients,
        //    [](int64_t& a, auto& b) {
        //        return a + b.second;
        //    });
    }

    int64_t Rooms(string hotel_name)
    {
        if (rooms_count_.find(hotel_name) == rooms_count_.end()) {
            return 0;
        }

        auto rooms_in_hotel = rooms_count_[hotel_name];
        int64_t rooms = 0;
        auto low = rooms_in_hotel.upper_bound(current_time_ - SECONDS_PER_DAY);
        for (auto i = low; i != rooms_in_hotel.end(); ++i) {
            rooms += i->second;
        }
        return rooms;
        //return accumulate(low, rooms_in_hotel.end(), rooms,
        //    [](int64_t& a, auto& b) {
        //        return a + b.second;
        //    });
    }
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Booking booking;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
        int64_t time;
        string hotel_name;
        int client_id;
        int room_count;
        cin >> time >> hotel_name >> client_id >> room_count;
        booking.Book(time, hotel_name, client_id, room_count);
    }
    else if (query_type == "CLIENTS") {
        string hotel_name;
        cin >> hotel_name;
        cout << booking.Clients(hotel_name) << '\n';
    }
    else if (query_type == "ROOMS") {
        string hotel_name;
        cin >> hotel_name;
        cout << booking.Rooms(hotel_name) << '\n';
    }
  }

  return 0;
}
*/

struct Booking
{
    int64_t time;
    int client_id;
    int room_count;
};

class BookingInfo
{
public:
    void Book(const Booking& booking)
    {
        last_.push(booking);
        room_count_ += booking.room_count;
        ++clients_[booking.client_id];
    }

    int Clients(int64_t current_time)
    {
        Remove(current_time);
        return clients_.size();
    }

    int Rooms(int64_t current_time)
    {
        Remove(current_time);
        return room_count_;
    }

private:
    queue<Booking> last_;
    int room_count_ = 0;
    map<int, int> clients_;

    void Remove(int64_t current_time)
    {
        while (!last_.empty() && last_.front().time <= current_time - 86400) {
            const Booking& booking = last_.front();
            room_count_ -= booking.room_count;
            const auto stat_ = clients_.find(booking.client_id);
            if (--stat_->second == 0) {
                clients_.erase(stat_);
            }
            last_.pop();
        }
    }
};

class BookingWrapper {
public:
    void Book(int64_t time, const string& hotel_name,
        int client_id, int room_count) {
        current_time_ = time;
        hotels_[hotel_name].Book({ time, client_id, room_count });
    }
    int Clients(const string& hotel_name) {
        return hotels_[hotel_name].Clients(current_time_);
    }
    int Rooms(const string& hotel_name) {
        return hotels_[hotel_name].Rooms(current_time_);
    }

private:
    int64_t current_time_ = 0;
    map<string, BookingInfo> hotels_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BookingWrapper manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            string hotel_name;
            int client_id, room_count;
            cin >> time >> hotel_name >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        }
        else if (query_type == "CLIENTS") {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.Clients(hotel_name) << "\n";
        }
        else if (query_type == "ROOMS") {
            string hotel_name;
            cin >> hotel_name;
            cout << manager.Rooms(hotel_name) << "\n";
        }
    }

    return 0;
}