  
#include <iostream>
#include <cstring>

class StringIterator
{

public:
        StringIterator(const char * str) : 
                position(str), 
                end(str + strlen(str))  { }
        bool at_end() const { return position == end; }
        void next() { ++position; }
        char get_char() const { return *position; }

private:
        const char *position = nullptr;
        const char *end = nullptr;
};

void print(const StringIterator &iterator)
{
        for (StringIterator i = iterator; !i.at_end(); i.next())
        {
                std::cout << i.get_char();
        }
}

int main()
{
        StringIterator iterator("Hello world!\n");
        print(iterator);

        return 0;
}