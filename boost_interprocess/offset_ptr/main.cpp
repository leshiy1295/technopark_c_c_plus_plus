#include <boost/interprocess/offset_ptr.hpp>

#include <iostream>

#pragma pack(push, 1)
struct example {
    int a; // 4
    boost::interprocess::offset_ptr<int> ptr; // 8
    int b; // 4
};

int main() {
    example ex;
    std::cout << static_cast<int>(ex.ptr.get_offset()) << "\n"; // offset=1 == nullptr
    assert(ex.ptr == nullptr);
    ex.ptr = &ex.a; // ex.ptr == -1
    std::cout << static_cast<int>(ex.ptr.get_offset()) << ", difference: " << (char *)&ex.a - (char *)&ex.ptr << "\n";
    ex.ptr = &ex.b; // ex.ptr == 1
    std::cout << static_cast<int>(ex.ptr.get_offset()) << ", difference: " << (char *)&ex.b - (char *)&ex.ptr << "\n";
}
