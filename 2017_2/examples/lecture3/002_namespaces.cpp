#include <iostream>

namespace N1 {
    class A {
    public:
        void print() {
            std::cout << "N1::A" << std::endl;
        }

        static void static_print() {
            std::cout << "N1::A" << std::endl;
        }
    };
}

namespace www {
    namespace example {
        namespace com {
            void print() {
                std::cout << "www::example::com::print()" << std::endl;
            }
        }
    }
}

namespace big {
    void foo1() {
        std::cout << "big::foo1()" << std::endl;
    }
}

namespace small {
    void foo() {
        std::cout << "small::foo()" << std::endl;
    }
}

namespace big {
    void foo2() {
        std::cout << "big::foo2()" << std::endl;
    }
}

struct A {
    static void print() {
        std::cout << "A::print()" << std::endl;
    }
};

namespace global_from_namespace {
    struct A {
        static void print() {
            std::cout << "local::A::print()" << std::endl;
        }
    };

    void print() {
        A::print();
        ::A::print(); // global namespace
    }
}

void bar(int a) {
    std::cout << "::bar(int)" << std::endl;
}

namespace global_names_search {
    void foo(int a) {
        std::cout << "global_names_search::foo(int)" << std::endl;
    }

    void print() {
        foo(5);
        bar(5);
    }

    // after print => not found
    void bar(float a) {
        std::cout << "global_names_search::bar(float)" << std::endl;
    }

    namespace inner_names_search {
        void foo(float a) {
            std::cout << "global_names_search::inner_names_search::foo(float)" << std::endl;
        }

        void print() {
            foo(5);
            bar(5);
        }

        // after print => not found
        void bar(float a) {
            std::cout << "global_names_search::inner_names_search::bar(float)" << std::endl;
        }
    }
}

void foo(int a) {
    std::cout << "::foo(int)" << std::endl;
}

namespace using_example_outer {
    void foo(int a) {
        std::cout << "using_example_outer::foo(int)" << std::endl;
    }

    namespace using_example_inner {
        void before_print() {
            foo(5);
        }

        using ::foo;

        void after_print() {
            foo(5);
        }
    }
}

namespace using_namespace_example_outer {
    void before_print() {
        std::cout << "using_namespace_example_outer::before_print()" << std::endl;
    }
    namespace with_using {
        void before_print() {
            std::cout << "using_namespace_example_outer::with_using::before_print()" << std::endl;
        }

        namespace inner {
            void before_print() {
                std::cout << "using_namepace_example_outer::with_using::inner::before_print()" << std::endl;
            }
        }

        using namespace std;

        namespace inner {
            void after_print() {
                cout << "using_namepace_example_outer::with_using::inner::after_print()" << endl;
            }
        }

        void after_print() {
            cout << "using_namespace_example_outer::with_using::after_print()" << endl;
        }
    }
    void after_print() {
        std::cout << "using_namespace_example_outer::after_print()" << std::endl;
    }
}

namespace outer {
    namespace inner1 {
        void foo(int a) {
            std::cout << "outer::inner1::foo(int)" << std::endl;
        }
    }

    namespace inner2 {
        void foo(float a) {
            std::cout << "outer::inner2::foo(float)" << std::endl;
        }

        void print_with_using_namespace() {
            using namespace inner1;
            foo(42);
        }

        void print_with_using_foo() {
            using inner1::foo;
            foo(42);
        }

        void print_with_explicit_foo() {
            inner1::foo(42);
            inner2::foo(42);
        }
    }
}

namespace ADL {
    namespace outer {
        namespace inner {
            struct SumObject {
                SumObject() {}
                SumObject(int a, int b) : a(a), b(b) {}
                void print() {
                    std::cout << a << " " << b << " " << std::endl;
                }
                int a;
                int b;
            };

            SumObject operator+(SumObject const &a, SumObject const &b) {
                std::cout << "ADL::outer::inner::operator+" << std::endl;
                return SumObject(a.a + b.a, a.b + b.b);
            }
        }

        inner::SumObject operator-(inner::SumObject const &a, inner::SumObject const &b) {
            std::cout << "ADL::outer::operator-" << std::endl;
            return inner::SumObject(a.a - b.a, a.b - b.b);
        }
    }
}

// to make struct Anon as static member of this file
namespace {
    struct NotStaticInAnonNamespace {
        static void foo() {
            std::cout << "NotStaticInAnonNamespace::foo()" << std::endl;
        }
    };
}

namespace long_outer {
    namespace long_middle {
        namespace long_inner {
            void print() {
                std::cout << "long_outer::long_middle::long_inner::print()" << std::endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // simple namespace
    N1::A::static_print();
    N1::A().print();

    // nested namespaces
    www::example::com::print();

    // namespace with divided implementation
    big::foo1();
    big::foo2();
    small::foo();

    // accessing global namespace
    global_from_namespace::print();

    // method names lookup
    global_names_search::print();
    global_names_search::inner_names_search::print();

    // using example
    using_example_outer::using_example_inner::before_print();
    using_example_outer::using_example_inner::after_print();

    // using namespace example
    using_namespace_example_outer::before_print();
    using_namespace_example_outer::with_using::before_print();
    using_namespace_example_outer::with_using::inner::before_print();
    using_namespace_example_outer::with_using::inner::after_print();
    using_namespace_example_outer::with_using::after_print();
    using_namespace_example_outer::after_print();

    // using from neighbor namespace
    outer::inner2::print_with_using_namespace();
    outer::inner2::print_with_using_foo();
    outer::inner2::print_with_explicit_foo();

    // operator from namespace - Koenig's search (ADL, Argument-dependent name lookup)
    ADL::outer::inner::SumObject a(1, 2);
    a.print();
    ADL::outer::inner::SumObject b(3, 4);
    b.print();
    ADL::outer::inner::SumObject c;
    c.print();
    c = a + b;
    c.print();
    /*a = c - b; // operator- not found in global and in operands namespaces
    a.print();*/
    c = ADL::outer::operator-(a, b);
    c.print();
    ADL::outer::inner::SumObject d = operator+(a, b);
    d.print();
    ADL::outer::inner::SumObject e = ADL::outer::inner::operator+(a, b);
    e.print();

    // nameless namespaces
    NotStaticInAnonNamespace::foo();

    // aliasing namespaces
    long_outer::long_middle::long_inner::print();
    namespace short_alias = long_outer::long_middle::long_inner;
    short_alias::print();

    {
        int k = 0;
        std::cout << "Scope defines new namespace. For example, k = " << k << std::endl;
        {
            int k = 42;
            std::cout << "Now we defined new k in inner scope => in inner namespace, so now k = " << k << std::endl;
        }
        std::cout << "Old k did not change: k = " << k << std::endl;
        {
            using std::cout;
            using std::endl;
            cout << "At least, prefer explicit namespace prefix or use aliases for long ones" << endl;
        }
        using namespace std;
        cout << "And don't use 'using namespace std' etc in header files!!!" << endl;
    }
    return 0;
}
