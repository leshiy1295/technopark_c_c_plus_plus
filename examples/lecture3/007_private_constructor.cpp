#include <iostream>

class BadSingleton {
private:
    static BadSingleton *_instance;
    int count;
public:
    static BadSingleton *getInstance() {
        if (!_instance) {
            _instance = new BadSingleton();
        }
        return _instance;
    }

    void inc() {
        ++count;
    }

    void print() {
        std::cout << _instance << " " << count << std::endl;
    }

    ~BadSingleton() {
        std::cout << "~BadSingleton()" << std::endl;
        _instance = NULL;
    }
private:
    BadSingleton() : count(0) {
        std::cout << "BadSingleton()" << std::endl;
    }
    BadSingleton(const BadSingleton &s) {
        std::cout << "BadSingleton(const BadSingleton &s)" << std::endl;
    }
    BadSingleton &operator=(BadSingleton &s) { 
        std::cout << "BadSingleton &operator=" << std::endl;
        return s;
    }
};

BadSingleton *BadSingleton::_instance = NULL;

class GoodSingleton {
private:
    int count;
public:
    static GoodSingleton &getInstance() {
        static GoodSingleton _instance;
        return _instance;
    }

    void inc() {
        ++count;
    }

    void print() {
        std::cout << count << std::endl;
    }
private:
    GoodSingleton() : count(0) {
        std::cout << "GoodSingleton()" << std::endl;
    }
    GoodSingleton(const GoodSingleton &s) {
        std::cout << "GoodSingleton(const BetterSingleron &s)" << std::endl;
    }
    GoodSingleton &operator=(GoodSingleton &s) {
        std::cout << "GoodSingleton &operator=" << std::endl;
        return s;
    }
    ~GoodSingleton() {
        std::cout << "~GoodSingleton()" << std::endl;
    }
};

class GoodSingleton;

int main(int argc, char *argv[]) {
    //BadSingleton s1;
    BadSingleton *s11(BadSingleton::getInstance());
    BadSingleton *s12 = BadSingleton::getInstance();
    s11->inc();
    s11->inc();
    s11->inc();
    s11->print();
    s12->print();
    s12->inc();
    s12->inc();
    s12->print();
    s11->print();
    BadSingleton::getInstance()->print();
    delete BadSingleton::getInstance(); // not comfortable

    //GoodSingleton s2;
    GoodSingleton &s21 = GoodSingleton::getInstance();
    GoodSingleton &s22(GoodSingleton::getInstance());
    s21.inc();
    s21.inc();
    s21.inc();
    s21.print();
    s22.print();
    s22.inc();
    s22.inc();
    s22.print();
    s21.print();
    GoodSingleton::getInstance().print();
    return 0;
}


template <class Type>
class Singleton {
public:
    static Type& GetInstance() {
      Type* instance = atomic_instance_.load();
      if (!instance) {
        std::lock_guard<std::mutex> lock(initialization_mutex_);
        instance = atomic_instance_.load();
        if (!instance) {
          instance = new Type;
          atomic_instance_.store(instance);
        }
      }

      return *instance;
    }

protected:
    Singleton() {}
    ~Singleton() {}

private:
    static std::mutex initialization_mutex_;
    static std::atomic<Type*> atomic_instance_;
};

template <class Type>
std::mutex Singleton<Type>::initialization_mutex_;
template <class Type>
std::atomic<Type*> Singleton<Type>::atomic_instance_;