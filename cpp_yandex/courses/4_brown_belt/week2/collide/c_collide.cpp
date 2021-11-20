#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

// ���������� ������ Unit, Building, Tower � Fence ���, ����� ��� ������������� ��
// GameObject � ������������� ��� ���������.

template<typename T>
class Collider : public GameObject {
public:
  virtual bool Collide(const GameObject& that) const override final;
  virtual bool CollideWith(const Unit& that) const override final;
  virtual bool CollideWith(const Building& that) const override final;
  virtual bool CollideWith(const Tower& that) const override final;
  virtual bool CollideWith(const Fence& that) const override final;
};


template<typename T>
bool Collider<T>::Collide(const GameObject& that) const {
  return that.CollideWith(static_cast<const T&>(*this));
}

#define COLLIDE_WITH(Type)                                                                  \
  template<typename T>                                                                      \
  bool Collider<T>::CollideWith(const Type& that) const {                                   \
    return geo2d::Collide(static_cast<const T&>(*this).GetPosition(), that.GetPosition());  \
  }

COLLIDE_WITH(Unit)
COLLIDE_WITH(Building)
COLLIDE_WITH(Tower)
COLLIDE_WITH(Fence)


class Unit final : public Collider<Unit> {
public:
  explicit Unit(geo2d::Point geometry) : geometry_(geometry) { }
  geo2d::Point GetPosition() const { return geometry_; }

private:
  geo2d::Point geometry_;
};

class Building final : public Collider<Building> {
public:
  explicit Building(geo2d::Rectangle geometry) : geometry_(geometry) { }
  geo2d::Rectangle GetPosition() const { return geometry_; }

private:
  geo2d::Rectangle geometry_;
};

class Tower final : public Collider<Tower> {
public:
  explicit Tower(geo2d::Circle geometry) : geometry_(geometry) { }
  geo2d::Circle GetPosition() const { return geometry_; }

private:
  geo2d::Circle geometry_;
};

class Fence final : public Collider<Fence> {
public:
  explicit Fence(geo2d::Segment geometry) : geometry_(geometry) { }
  geo2d::Segment GetPosition() const { return geometry_; }

private:
  geo2d::Segment geometry_;
};


bool Collide(const GameObject& first, const GameObject& second) {
  return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
  // ����-���� ���������� ��������, ����� �� ������� ����� ��� ���� �����-�� �������,
  // � �� ����� �������� �� �� �����, ��������, ��������� ����� ������ ��� �����.
  // �� ����� ��� ��������, ������ ���� �� �� ������������ �� � ����� �� ������������.
  using namespace geo2d;

  const vector<shared_ptr<GameObject>> game_map = {
    make_shared<Unit>(Point{3, 3}),
    make_shared<Unit>(Point{5, 5}),
    make_shared<Unit>(Point{3, 7}),
    make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
    make_shared<Tower>(Circle{Point{9, 4}, 1}),
    make_shared<Tower>(Circle{Point{10, 7}, 1}),
    make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
  };

  for (size_t i = 0; i < game_map.size(); ++i) {
    Assert(
      Collide(*game_map[i], *game_map[i]),
      "An object doesn't collide with itself: " + to_string(i)
    );

    for (size_t j = 0; j < i; ++j) {
      Assert(
        !Collide(*game_map[i], *game_map[j]),
        "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
      );
    }
  }

  auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT( Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT( Collide(*new_warehouse, *game_map[3]));
  ASSERT( Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT( Collide(*new_defense_tower, *game_map[3]));
  ASSERT( Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestAddingNewObjectOnMap);
  return 0;
}