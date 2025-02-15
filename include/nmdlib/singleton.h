#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

/// @brief Singleton<> is meant to serve as a base class to help with children that impliment the singleton design pattern
/// @tparam T The type of the class, so for example, class Engine : public Singleton<Engine>
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

#endif