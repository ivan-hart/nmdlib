#include "nmdlib/singleton.h"

/// @brief A function to be called by inherited classes to retriev the only instance of said class for the lifetime of the application
/// @tparam T The inherited class's type
/// @return The inherited class's single instance
template <typename T>
inline T &Singleton<T>::GetInstance()
{
    static T instance;
    return instance;
}