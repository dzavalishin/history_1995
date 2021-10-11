/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

#ifndef DEFALLOC_H
#define DEFALLOC_H

#include <malloc.h>  // NON-HP HACK
#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <algobase.h>

inline void* operator new(size_t, void* p) {return p;}

/* NON-HP HACK
 Default allocation function for small, medium, compact and large models.
*/

#ifndef __HUGE__

template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
        cerr << "out of memory" << endl; 
        exit(1);
    }
    return tmp;
}

template <class T>
inline void deallocate(T * buffer) { ::operator delete(buffer); }

#else

/* NON-HP HACK
 Default allocation functions for huge model.
*/

template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*) halloc( size, sizeof(T));
    if (tmp == 0) {
        cerr << "out of memory" << endl; 
        exit(1);
    }
    return tmp;
}


template <class T>
inline void deallocate(T * buffer) { hfree(buffer); }


#endif


template <class T>
inline void destroy(T* pointer) {
    pointer->~T();
}

inline void destroy(char*) {}
inline void destroy(unsigned char*) {}
inline void destroy(short*) {}
inline void destroy(unsigned short*) {}
inline void destroy(int*) {}
inline void destroy(unsigned int*) {}
inline void destroy(long*) {}
inline void destroy(unsigned long*) {}
inline void destroy(float*) {}
inline void destroy(double*) {}
inline void destroy(char**) {}
inline void destroy(unsigned char**) {}
inline void destroy(short**) {}
inline void destroy(unsigned short**) {}
inline void destroy(int**) {}
inline void destroy(unsigned int**) {}
inline void destroy(long**) {}
inline void destroy(unsigned long**) {}
inline void destroy(float**) {}
inline void destroy(double**) {}

inline void destroy(char*, char*) {}
inline void destroy(unsigned char*, unsigned char*) {}
inline void destroy(short*, short*) {}
inline void destroy(unsigned short*, unsigned short*) {}
inline void destroy(int*, int*) {}
inline void destroy(unsigned int*, unsigned int*) {}
inline void destroy(long*, long*) {}
inline void destroy(unsigned long*, unsigned long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
inline void destroy(char**, char**) {}
inline void destroy(unsigned char**, unsigned char**) {}
inline void destroy(short**, short**) {}
inline void destroy(unsigned short**, unsigned short**) {}
inline void destroy(int**, int**) {}
inline void destroy(unsigned int**, unsigned int**) {}
inline void destroy(long**, long**) {}
inline void destroy(unsigned long**, unsigned long**) {}
inline void destroy(float**, float**) {}
inline void destroy(double**, double**) {}


template <class T1, class T2>
inline void construct(T1 * p, const T2& value) {
    new (p) T1(value);
}



template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef ptrdiff_t size_type;
    typedef ptrdiff_t difference_type;
    pointer allocate(difference_type n) { 
        return ::allocate(n, (pointer)0);
    }
    void deallocate(pointer p) { ::deallocate(p); }
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { 
        return (const_pointer)&x; 
    }
    size_type init_page_size() { 
        return max(size_type(1), size_type(4096/sizeof(T))); 
    }
    size_type max_size() const { 
        return max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
    }
};

class allocator<void> {
public:
    typedef void* pointer;
};



#endif

