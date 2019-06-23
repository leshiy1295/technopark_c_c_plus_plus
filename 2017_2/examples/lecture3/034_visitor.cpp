#include <iostream>
#include <string>

using namespace std;

struct Visitor;

struct Building {
  virtual void accept(Visitor* visitor) = 0;
};


struct City : public Building {
  void accept(Visitor* visitor) override;
};
struct Industry : public Building {
  void accept(Visitor* visitor) override;
};
struct House : public Building {
  void accept(Visitor* visitor) override;
};


struct Visitor {
  virtual void visit(City& ref) = 0;
  virtual void visit(Industry& ref) = 0;
  virtual void visit(House &ref) = 0;


  virtual void visit2(City* c) = 0;
  virtual void visit2(Industry* f) = 0;
  virtual void visit2(House* h) = 0;

  virtual ~Visitor() = default;
};

class ExportVisitor : public Visitor {
  void doForCity(City c) {}
  void doForIndustry(Industry f) {}
  void doForHouse(House h) {}
};

/*
ExportVisitor visitor;
for (Building* building: buildings) {
  if(auto building = dynamic_cast<City*>(Building)) {
    // do something for City
    visitor.doForCity(*building);
  } else if (auto building = dynamic_cast<Industry*>(Building)) {
    // do something for Industry
    visitor.doForIndustry(*building);
  }
}
*/

/*********************************************************************************/

void City::accept(Visitor *visitor) {
  visitor->visit(*this);
  visitor->visit2(this);
}

void Industry::accept(Visitor *visitor) {
  visitor->visit(*this);
  visitor->visit2(this);
}
void House::accept(Visitor *visitor) {
  visitor->visit(*this);
  visitor->visit2(this);
}

struct BuildingVisitor : public Visitor {
  void visit(City &ref) override { std::cout << "City visited\n"; };
  void visit(Industry &ref) override { std::cout << "Industry visited\n"; };
  void visit(House &ref) override { std::cout << "House visited\n"; };

  void visit2(City *c) override { std::cout << "City*\n"; }
  void visit2(Industry *f) override { std::cout << "Industry*\n"; }
  void visit2(House *h) override { std::cout << "House*\n"; }
};

struct ExportVisitor2 {
  void doForBuilding(Building* c) { std::cout << "Building*\n"; }
  void doForBuilding(City* c) { std::cout << "City*\n"; }
  void doForIndustry(Industry* f) { std::cout << "Industry*\n"; }
  void doForHouse(House h) { std::cout << "House*\n"; }
};


// --------------------------------
int main() {
//  {
//    ExportVisitor2 visitor;
//    Building* building = new City();
//    for (auto : )
//    visitor.doForBuilding(building);
//  }

  {
    City city;
    House house;
    Industry industry;
    Building *buildings[] = {&city, &house, &industry};
    for (auto building : buildings) {
      BuildingVisitor visitor;
      building->accept(&visitor);
    }
  }

  return 0;
}
