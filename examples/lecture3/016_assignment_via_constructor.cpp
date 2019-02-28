#include <iostream>

class resource {};
typedef resource *resource_t;
resource_t acquire(const std::string &name) {
    return nullptr;
}
void release(resource_t r) {}

class T {
private:
    resource_t handle;
public:
    explicit T(const std::string &_name) : handle {::acquire(_name)} {
        std::cout << "T()" << std::endl;
    }

    T(T &&rhs) : handle{rhs.handle} {
        std::cout << "T(T &&)" << std::endl;
        rhs.handle = nullptr;
    }

    T &operator=(T &&rhs) {
        std::cout << "T &operator(T &&)" << std::endl;
        T copy{std::move(rhs)};
        std::swap(handle, copy.handle);
        return *this;
    }
    ~T() {
        std::cout << "~T()" << std::endl;
        ::release(handle);
    }
};

int main(int argc, char *argv[]) {
    T a("string");
    T b(std::move(a));
    b = std::move(a);
    std::cout << "end of scope" << std::endl;
    return 0;
}
