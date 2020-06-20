#ifndef SINGLETON
#define SINGLETON

template <typename T>
class Singleton
{
public:
    Singleton() {}
    virtual ~Singleton() {}

    static T* GetInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new T;
        }
        return (instance_);
    }

    inline static void Release()
    {
        SAFE_DELETE(instance_);
    }

private:
    static T* instance_;
};

template <typename T> T* Singleton<T>::instance_ = nullptr;

#endif // !SINGLETON
