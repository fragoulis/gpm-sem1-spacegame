
#pragma once

template<class T>
class Singleton
{
public:
    /**
     * Returns the object's unique instance
     */
    static T& Instance()
    {
        if( 0 == pInstance ) {  
            pInstance = new T;
        }

        return *pInstance;
    }

    /**
     * Deletes the instance from memory
     */
    static void Destroy()
    {
        if( pInstance ) {
            delete pInstance;
            pInstance = 0;
        }
    }
    
protected:
    // Declare all auto constructor, copy constructor and
    // destructor, assignment operator protected
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator= (const Singleton&);

    // The object's unique instance
    static T* pInstance;

}; // end of Singleton class

// Initialize static pointer
template<class T>
T* Singleton<T>::pInstance = 0;