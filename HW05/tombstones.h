/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <stdexcept>


template <typename T> class Pointer;
template <typename T> void free(Pointer<T>& obj);

template <typename T>
class Pointer {
public:
    Pointer<T>();                             // default constructor
    Pointer<T>(Pointer<T>&);                        // copy constructor
    Pointer<T>(T*);                             // bootstrapping constructor
        // argument should always be a call to new
    ~Pointer<T>();                              // destructor
    T& operator*() const;                   // deferencing
    T* operator->() const;                  // field dereferencing
    Pointer<T>& operator=(const Pointer<T>&);
    /* pointer arithmetic */
    Pointer<T>& operator+=(int num);
    Pointer<T>& operator-=(int num);
    Pointer<T>& operator++();
    Pointer<T>& operator--();
    Pointer<T>& operator++(int num);
    Pointer<T>& operator--(int num);
    Pointer<T>& operator+(int num);
    Pointer<T>& operator-(int num);
    /* pointer comparsion */
    bool operator<(const Pointer& other);
    bool operator>(const Pointer& other);
    bool operator<=(const Pointer& other);
    bool operator>=(const Pointer& other);
//    Pointer<T>& operator=(const Pointer<T>&);       // assignment
    friend void free<T>(Pointer<T>&);           // delete pointed-at object
        // This is essentially the inverse of the new inside the call to
        // the bootstrapping constructor.
    // equality comparisons:
    bool operator==(const Pointer<T>&) const;
    bool operator!=(const Pointer<T>&) const;
    bool operator==(const int) const;
        // true iff Pointer is null and int is zero
    bool operator!=(const int) const;
        // false iff Pointer is null and int is zero
private:
    // value is the pointee that this pointer pointed at.
    T* value;
    // count: how many pointers are actually pointing at this object.
    int count;
    /* didFree: check if this object is already free. default value is false.
                if this object is already freed and free again, it will
                raise a runtime exception,
                if this object is not freed yet, it will be freed and
                change didFree into "true".
     */
    bool didFree;
    /*
     refptr: this is used in the situation, if one is already constructed, e.g.
     Pointer<T> A = Pointer<T> B or Pointer<T> A = new Pointer<T> B.
     Then, instead of build another Pointer struct and deep copy all the
     information from B into A, I make a simple refptr attributes, and let
     this pointer A to reference B. So whenever, we want to free the actual
     object "*T", we use refptr to trace back to the actual tombstone and find
     the pointer "T* value" and then free.
     */
    Pointer<T>* refptr;
    int index;
    /*
     Some private functions:
     freed(): ask if this Pointer is freed. Two actions will happen.
              first, if this Pointer is actual stored value pointer, "T* value",
              check if "didFree" is true, otherwise, trace back by using refptr,
              to see if this reference pointer, "refptr", is actually freed.
              mutltiple might happened.
              e.g.
                   Pointer<T> A        ---> T* "actual pointer"
                   Pointer<T> B.refptr ---> Pointer<T> A
                   Pointer<T> C.refptr ---> Pointer<T> B
             if we want to know whether Pointer C is freeed or not, we will trace
             back to A and find if A.didFree == true.
     */
    bool freed();
    /*
     addCount(): same mechanism as freed(). I always add one counting reference to
                 to the very first Pointer and make other just reference.
     In this case:
             e.g.
             Pointer<T> A        ---> T* "actual pointer"
             Pointer<T> B.refptr ---> Pointer<T> A
             Pointer<T> C.refptr ---> Pointer<T> B
             when B and C, A.count will always add one.
     */
    void addCount();
    /*
     getValue(): same to get the value of pointer address, "T*", from itself, or from
                 refptr.
     */
    T* getValue() const;
};


template <typename T>
Pointer<T>::Pointer(){
    value = nullptr;
    count = 0;
    didFree = false;
    refptr = nullptr;
    index = 0;
    
}
template <typename T>
Pointer<T>::Pointer(Pointer<T>& ref){
    if (ref.freed()) {
        throw std::runtime_error("This can potentially use dangling pointer!");
    } else {
        refptr = &ref;
        index = 0;
        ref.addCount();
    }
}


template <typename T>
Pointer<T>::Pointer(T* ptr) {
    value = ptr;
    count = 1;
    index = 0;
    didFree = false;
    refptr = nullptr;
}

template <typename T>
Pointer<T>::~Pointer<T>() {
    if (value != nullptr) {
        throw std::runtime_error("This pointer didn't free, potential memory leak!");
    }
}


template <typename T>
bool Pointer<T>::freed(){
    if (refptr == nullptr) {
        return didFree;
    } else {
        return (*refptr).freed();
    }
}
template <typename T>
void Pointer<T>::addCount(){
    if (refptr == nullptr) {
        count++;
    } else {
        (*refptr).addCount();
    }
}

template <typename T>
T& Pointer<T>::operator*() const {
    if (refptr == nullptr) {
        if (value == nullptr) {
            throw std::runtime_error("Danger: potentially dereference a nullptr!");
        }
        return *(value+index);
    } else {
        T* tmp = (*refptr).getValue();
        if (tmp == nullptr) {
            throw std::runtime_error("Danger: potentially dereference a nullptr!");
        }
        return *(tmp+index);
    }
}

template <typename T>
T* Pointer<T>::operator->() const {
    if (refptr == nullptr) {
        return value+index;
    } else {
        return (*refptr).getValue()+index;
    }
}

template <typename T>
T* Pointer<T>::getValue() const {
    if (refptr == nullptr) {
        return value;
    } else {
        return (*refptr).getValue();
    }
}

/************* Pointer Arithmetic Section ********************/
template <typename T>
Pointer<T>& Pointer<T>::operator+(int num) {
    index += num;
    return &this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator+=(int num) {
    index += num;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator-(int num) {
    index -= num;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator-=(int num) {
    index -= num;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator++() {
    index += 1;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator--() {
    index += 1;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator++(int num) {
    index += 1;
    return *this;
}

template <typename T>
Pointer<T>& Pointer<T>::operator--(int num) {
    index += 1;
    return *this;
}

/*************************************************************/
/************* Pointer Comparsion Section ********************/

template <typename T>
bool Pointer<T>::operator<(const Pointer& other) {
    return index < other.index;
}

template <typename T>
bool Pointer<T>::operator>(const Pointer& other) {
    return  index > other.index;
}

template <typename T>
bool Pointer<T>::operator<=(const Pointer& other) {
    return  index <= other.index;
}

template <typename T>
bool Pointer<T>::operator>=(const Pointer& other) {
    return  index >= other.index;
}

/*************************************************************/

template <typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer<T>& other) {
    return new Pointer<T>(other);
}

template <typename T>
void free(Pointer<T>& ptr) {
    if (ptr.freed()) {
        throw std::runtime_error("This pointer has already been freed!");
    } else {
        if (ptr.refptr == nullptr) {
            if (ptr.value == nullptr) {
                throw std::runtime_error("This pointer hasn't initialized yet!");
            } else {
                delete(ptr.value);
                ptr.didFree = true;
            }
            
        } else {
            free(*(ptr.refptr));
        }
        
    }
}

template <typename T>
bool Pointer<T>::operator==(const Pointer<T>& other) const {
    T* left = getValue();
    T* right = other.getValue();
    return left == right;
}

template <typename T>
bool Pointer<T>::operator!=(const Pointer<T>& other) const {
    T* left = getValue();
    T* right = other.getValue();
    return left != right;
}
template <typename T>
bool Pointer<T>::operator==(const int num) const {
    T* value = getValue();
    return (value == nullptr && num == 0);
}

template <typename T>
bool Pointer<T>::operator!=(const int num) const {
    value = getValue();
    return !(value == nullptr && num == 0);
}


#endif // __TOMBSTONES_H__
