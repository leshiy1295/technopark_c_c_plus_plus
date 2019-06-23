#include <iostream>

#include <list>

using namespace std;

struct Observer;

struct Model {
  virtual ~Model() {
    observers_.clear();
  };

  void AddObserver(Observer* observer) {
    observers_.push_back(observer);
  }

  void RemoveObserver(Observer* observer) {
    observers_.remove(observer);
  }

  void SetData(int new_data) {
    // store data
    Notify();
  }

  void Notify();

  std::list<Observer*> observers_;
};


struct Observer {
  virtual void OnModelChanched()  = 0;
};

struct ConcreteObserver1 : public Observer {
  void OnModelChanched() override {
    std::cout << "hello from model 1\n";
  }
};

struct ConcreteObserver2 : public Observer {
  void OnModelChanched() override {
    std::cout << "hello from model 2\n";
  }
};

struct ConcreteObserver3 : public Observer {
  void OnModelChanched() override {
    std::cout << "hello from model 3\n";
  }
};

void Model::Notify() {
  for(auto observer: observers_)
    observer->OnModelChanched();
}

int main() {
  Model model;

  Observer* observer1 = new ConcreteObserver1();
  Observer* observer2 = new ConcreteObserver2();
  Observer* observer3 = new ConcreteObserver3();

  model.AddObserver(observer1);
  model.AddObserver(observer2);
  model.AddObserver(observer3);

  model.SetData(5);
  return 0;
}
