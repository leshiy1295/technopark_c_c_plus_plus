#define MAKE_ADD_OVERLOAD(T)            \
    constexpr T add_overload(T x)       \
    {                                   \
        return x + 1;                   \
    }

MAKE_ADD_OVERLOAD(unsigned short)
MAKE_ADD_OVERLOAD(unsigned int)

template<typename T>
constexpr T add_val(T x)
{
    return add_overload(x);
}

template<int N>
struct adder
{
    static constexpr int value = add_val(N);
};

int main()
{
    return adder<1>::value;
}

