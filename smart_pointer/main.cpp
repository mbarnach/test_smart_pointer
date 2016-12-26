//
//  main.cpp
//  smart_pointer
//
//  Created by Mathieu Barnachon on 25/12/16.
//  Copyright © 2016 Styleshoots. All rights reserved.
//

#include <iostream>
#include "smart_pointer.h"

struct Foo {
    Foo(int n = 0) noexcept : bar(n) {
        std::cout << "Foo: constructor, bar = " << bar << '\n';
    }
    ~Foo() {
        std::cout << "Foo: destructor, bar = " << bar << '\n';
    }
    int getBar() const noexcept { return bar; }
    int bar;
};

int main()
{
    smart_pointer<Foo> sptr(new Foo(1));
    std::cout << "The first Foo's bar is " << sptr->getBar() << "\n";
    
    // reset the smart pointer, hand it a fresh instance of Foo
    // (the old instance will be destroyed after this call)
    sptr.reset(new Foo);
    std::cout << "The second Foo's bar is " << sptr->getBar() << "\n";
    

    int* pInt = new int(42);
    smart_pointer<int> pShared(new int(42));
    
    std::cout << "Naked pointer " << *pInt << std::endl;
    // output("Shared pointer ", pShared); // compiler error
    std::cout << "Shared pointer with get() " << *(pShared.get()) << std::endl;
    
    delete pInt;
    
    
    // Crash
    std::cout << "avant" << std::endl;
    {
        Foo* raw = new Foo(10);
        smart_pointer<Foo> ptr1(raw);
        smart_pointer<Foo> ptr2(raw);
        std::cout << ptr1.get() << std::endl;
        std::cout << ptr2.get() << std::endl;
        std::cout << ptr1->getBar() << std::endl;
        std::cout << ptr2->getBar() << std::endl;
        ptr2->bar = 22;
        std::cout << ptr1->getBar() << std::endl;
        std::cout << ptr2->getBar() << std::endl;
        ptr1.reset();
        std::cout << ptr1.get() << std::endl;
        std::cout << ptr2.get() << std::endl;
    }
    std::cout << "apres" << std::endl;
    
    
    return 0;
}
