//
//  smart_pointer_tests.cpp
//  smart_pointer
//
//  Created by Mathieu Barnachon on 26/12/16.
//  Copyright © 2016 Styleshoots. All rights reserved.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "smart_pointer.h"


TEST_CASE( "Smart pointer", "[smart pointer standard]" ) {
    smart_pointer<int> sptr(new int(1));
 
    SECTION( "Allocation" ) {
        REQUIRE( sptr.get() != 0 );
    }
    
    SECTION( "Access" ) {
        REQUIRE( *sptr == 1 );
    }
    
    SECTION( "Reset" ) {
        sptr.reset();
        REQUIRE( sptr.get() == 0 );
    }
    
    SECTION( "Reset with value" ) {
        sptr.reset( new int(3) );
        REQUIRE( sptr.get() != 0 );
        REQUIRE( *sptr == 3 );
    }
    
    SECTION( "Copy construction" ) {
        sptr.reset( new int(1) );
        smart_pointer<int> sptr_copy = sptr;
        REQUIRE( sptr_copy.get() != 0 );
        REQUIRE( *sptr_copy == 1 );
    }
}


TEST_CASE( "Smart pointer reference counting", "[smart pointer reference counting]" ) {
    smart_pointer<int> sptr(new int(1));
    smart_pointer<int> sptr2 = sptr;
    smart_pointer<int> sptr3 = sptr2;
    
    SECTION( "Allocation" ) {
        REQUIRE( sptr.get() != 0 );
    }
    
    SECTION( "Access" ) {
        REQUIRE( *sptr == 1 );
    }
    
    SECTION( "Reset" ) {
        sptr.reset();
        REQUIRE( sptr.get() == 0 );
        REQUIRE( sptr2.get() != 0 );
        REQUIRE( sptr3.get() != 0 );
    }
    
    SECTION( "Reset with value" ) {
        sptr.reset( new int(3) );
        REQUIRE( sptr.get() != 0 );
        REQUIRE( *sptr == 3 );
        REQUIRE( sptr2.get() != 0 );
        REQUIRE( *sptr2 == 1 );
        REQUIRE( sptr3.get() != 0 );
        REQUIRE( *sptr3 == 1 );
    }
    
    SECTION( "Copy construction" ) {
        smart_pointer<int> sptr_copy = sptr;
        REQUIRE( sptr_copy.get() != 0 );
        REQUIRE( *sptr_copy == 1 );
        REQUIRE( *sptr_copy == *sptr2 );
        REQUIRE( *sptr_copy == *sptr3 );
    }
}

struct Foo {
    Foo(int n = 0) : bar(n) {}
    ~Foo() {}
    int getBar() const { return bar; }
    int bar;
};

TEST_CASE( "Smart pointer global usage", "[smart pointer global usage]" ) {
    Foo* raw = new Foo(10);
    smart_pointer<Foo> ptr1(raw);
    smart_pointer<Foo> ptr2(raw);
    REQUIRE( ptr1.get() != 0 );
    REQUIRE( ptr2.get() != 0 );
    REQUIRE( ptr1.get() == ptr2.get() );
    
    SECTION( "Same value" ) {
        REQUIRE( ptr1->getBar() == 10 );
        REQUIRE( ptr2->getBar() == 10 );
    }
    
    SECTION( "Change value" ) {
        ptr2->bar = 22;
        REQUIRE( ptr1->getBar() == 22 );
        REQUIRE( ptr2->getBar() == 22 );
    }
    
    SECTION( "Distinct reset" ) {
        ptr1.reset();
        REQUIRE( ptr1.get() == 0);
        REQUIRE( ptr2.get() != 0);
    }
}



