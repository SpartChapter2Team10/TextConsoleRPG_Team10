#pragma once

template <typename T>
class Singleton
{
protected:
    static T* _Instance;

    Singleton();
    virtual ~Singleton();

public:
    static T* GetInstance();
};

template <typename T>
T* Singleton<T>::_Instance = nullptr;

template <typename T>
Singleton<T>::Singleton()
{
}

template <typename T>
Singleton<T>::~Singleton()
{
}

template <typename T>
T* Singleton<T>::GetInstance()
{
    // Implementation needed
    return nullptr;
}
