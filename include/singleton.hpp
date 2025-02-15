#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <iostream>
#include <memory>

template <typename T>
class Singleton
{
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    
public:
    Singleton( const Singleton& ) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& GetInstance();
};

template <typename T>
inline T &Singleton<T>::GetInstance()
{
    static T instance;
    return instance;
}

#endif