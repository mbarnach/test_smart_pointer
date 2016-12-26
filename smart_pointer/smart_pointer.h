//
//  smart_pointer.h
//  smart_pointer
//
//  Created by Mathieu Barnachon on 25/12/16.
//  Copyright © 2016 Styleshoots. All rights reserved.
//

#ifndef smart_pointer_h
#define smart_pointer_h

#include <map>

template<class T>
class GlobalPointerHandler;

class ReferenceCounter {
public:
    int count; // Reference count
    
public:
    void AddOneReference()
    {
        // Increment the reference count
        count++;
    }
    
    int ReleaseOneReference()
    {
        // Decrement the reference count and
        // return the reference count.
        return --count;
    }
};


template<class T>
class smart_pointer {
public:
    smart_pointer(T* ptr = 0) : _ptr(ptr) {
        alloc(ptr);
    }
    
    smart_pointer(const smart_pointer<T> & other) {
        release();
        alloc(other._ptr);
        _ptr = other._ptr;
    }
    
    virtual ~smart_pointer() {
        release();
    }
    
    void reset(T* ptr = 0) {
        release();
        alloc(ptr);
        _ptr = ptr;
    }
    
    T const * const get() {
        return _ptr;
    }
    
    T& operator*() const {
        return *_ptr;
    }
    
    T* const operator->() const {
        return _ptr;
    }
    
private:
    void alloc(T* ptr) {
        if(ptr!=0) {
            if( GlobalPointerHandler<T>::instance().exists(ptr) ) {
                reference = GlobalPointerHandler<T>::instance().get(ptr).reference;
            } else {
                reference = new ReferenceCounter;
                GlobalPointerHandler<T>::instance().register_pointer(ptr, *this);
            }
            reference->AddOneReference();
        }
    }
    
    void release() {
        if( reference ) {
            reference->ReleaseOneReference();
            if(reference->count == 0) {
                GlobalPointerHandler<T>::instance().deregister_pointer(_ptr);
                delete _ptr;
                _ptr = 0;
                delete reference;
                reference = 0;
            }
        }
    }
    
private:
    T* _ptr = 0;
    ReferenceCounter* reference = 0;
};


template<class T>
class GlobalPointerHandler {
public:
    static GlobalPointerHandler& instance() {
        if( singleton == 0 ) {
            singleton = new GlobalPointerHandler();
        }
        return *singleton;
    }
    
    bool exists(T* ptr) {
        return pointer_register.find(ptr) != pointer_register.end();
    }
    
    void register_pointer(T* ptr, smart_pointer<T> & sptr) {
        pointer_register[ptr] = sptr;
    }
    
    void deregister_pointer(T* ptr) {
        typename std::map<T*, smart_pointer<T>>::iterator it = pointer_register.find(ptr);
        if( it != pointer_register.end()) {
            pointer_register.erase(it);
        }
    }
    
    smart_pointer<T>& get(T* ptr) {
        return pointer_register[ptr];
    }
    
private:
    GlobalPointerHandler() {}
    
    ~GlobalPointerHandler() {
        delete singleton;
    }
    
    static GlobalPointerHandler* singleton;
    
private:
    std::map<T*, smart_pointer<T>> pointer_register;
};

template<class T>
GlobalPointerHandler<T>* GlobalPointerHandler<T>::singleton = 0;


#endif /* smart_pointer_h */
