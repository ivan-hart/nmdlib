#ifndef SINGLETON_HPP
#define SINGLETON_HPP

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

/// @brief A function to be called by inherited classes to retriev the only instance of said class for the lifetime of the application
/// @tparam T The inherited class's type
/// @return The inherited class's single instance
template <typename T>
inline T &Singleton<T>::GetInstance()
{
    static T instance;
    return instance;
}

#endif