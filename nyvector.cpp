#include <iostream>
using namespace std;
#include<iterator>
#include<cstddef>
#include<exception>

class accessdenied : public exception {
public:
    accessdenied() :errormsg("Invalid index item does not exist"){}
    string what() { return errormsg; }
private:
    string errormsg;
};

template <typename T>
class NYvector {
private:
    T* ptr;
    int size;
    int capacity;
public:
    struct iterator
    {
        using _Move_iterator_category = forward_iterator_tag;
        using _Get_difference_type = ptrdiff_t;
        using _Iter_value_t = T;
        using pointer = T*;
        using reference = T&;
        iterator(pointer pt): m_ptr(pt){}
        reference operator *()const 
        {
            return *m_ptr;
        }
        pointer operator->()
        { 
            return m_ptr; 
        }
        iterator& operator ++() 
        {
            m_ptr++;
            return *this; 
        }
        iterator& operator +(int n)
        {
            m_ptr+n;
            return *this;
        }
        
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend int operator -(const iterator& a, const iterator& b)
        {
            return a.m_ptr - b.m_ptr;
        }
        friend bool operator ==(const iterator& a, const iterator& b)
        {
            return a.m_ptr == b.m_ptr;
        }
        friend bool operator!=(const iterator& a, const iterator& b)
        {
            return a.m_ptr != b.m_ptr;
        }
        friend bool operator<(const iterator& a, const iterator& b)
        {
            return a.m_ptr < b.m_ptr;
        }
        friend bool operator>(const iterator& a, const iterator& b)
        {
            return a.m_ptr > b.m_ptr;
        }

    private:
        pointer m_ptr;
    };
    iterator begin() { return iterator(ptr); } 
    iterator give_me(int n) { return iterator(ptr + n); }
    iterator give_me(T* p) { return iterator(p); }
    iterator end() { return iterator(ptr+capacity); }
    NYvector(int n) :capacity(n),size(0) {
        ptr = new T[n];
    }
    NYvector(T* arr, int n) :size(n), capacity(n) {
        ptr = new T[n];
        for (int i = 0; i < n; i++)
        {
            ptr[i] = arr[i];
        }
    }
    T& operator[](int n) 
    {
        try
        {
            if (n<0 || n > capacity)
            {
                throw accessdenied();
            }
            else
                return ptr[n];
        }
        catch(accessdenied &ad)
        {
            cout << "exception occured" << endl;
            cout << ad.what() << endl;

        }
    }
    const int capacity_of() { return capacity; }
    const int size_of() { return size; }
    NYvector(const NYvector& other) 
    {
        capacity = other.capacity;
        size = other.size;
        ptr = new T[capacity];
        for (int i = 0; i < capacity; i++)
        {
            ptr[i] = other.ptr[i];
        }
    }
    NYvector& operator=(const NYvector& from)
    {
        if (this != &from)
        {
            size = from.size;
            capacity = from.capacity;
            delete[] ptr;
            ptr = new T[capacity];
            for (int i = 0; i < capacity; i++)
            {
                ptr[i] = from.ptr[i];
            }
            
            
        }
        return *this;
    }
    NYvector& operator=(const NYvector&& from)
    {
        if (this != &from)
        {
            size = from.size;
            capacity = from.capacity;
            delete[]ptr;
            ptr = new T[capacity];
            for (int i = 0; i < capacity; i++)
            {
                ptr[i] = from.ptr[i];
            }
            from.ptr = nullptr;
        }
        return *this;
    }
    void erase(iterator it)
    {
        try
        {
            if ((it >begin()||it==begin()) && it < end())
            {
                T* pt;
                pt= new T[size - 1];
                for (int i = 0; i < size; i++)
                {
                    if (i < it - begin()) { pt[i] = ptr[i]; }
                    else if (i>= it - begin()) { pt[i] = ptr[i + 1]; }
                }
                ptr = pt;
                pt = nullptr;
                size -= 1; capacity -= 1;
            }
            else
                throw accessdenied();
        }
        catch(accessdenied& ad){
            cout << "exception occured" << endl;
            cout << ad.what() << endl;
        }
    }
    void erase(iterator it1,iterator it2)
    {
        try
        {
            if (((it1 >begin()||it1== begin()) && it1 < end()) && ((it2 >begin() ||it2==begin()) && it2 < end()) && (it1<it2))
            {
                T* pt;
                pt = new T[size - (it2-it1)+1];
                for (int i = 0; i < size; i++)
                {
                    if (i < it1 - begin()) { pt[i] = ptr[i]; }
                    
                    else if (i >=it1 - begin()) { pt[i] = ptr[i +(it2-it1+1)]; }
                }
                ptr = pt;
                pt = nullptr;
                size -= (it2 - it1) + 1; capacity -= it2 - it1;
            }
            else
                throw accessdenied();
        }
        catch (accessdenied& ad) {
            cout << "exception occured" << endl;
            cout << ad.what() << endl;
        }
    }
    void insert(iterator it,T item)
    {
        try
        {
            if ((it > begin()||it==begin()) && it < end())
            {
                T* pt;
                pt = new T[size +1];
                for (int i = 0; i < it-begin(); i++)
                {
                    pt[i] = ptr[i];
                }
                pt[it - begin()] = item;
                for (int j = it-begin()+1; j < size + 1; j++)
                {
                    pt[j] = ptr[j-1];
                }
                ptr = pt;
                pt = nullptr;
                size += 1; capacity = size;
            }
            else
                throw accessdenied();
        }
        catch (accessdenied& ad) {
            cout << "exception occured" << endl;
            cout << ad.what() << endl;
        }
    }
    void clear()
    {
        ptr = nullptr;
        size = 0;
    }
    ~NYvector() {  delete[] ptr ; }
    void show() { for (int i = 0; i < capacity; i++) { cout << ptr[i]; } }
};

int main()
{
    int arr[5] = { 1,2,3,4,5 };
    NYvector<int> v(arr,5);
    NYvector<int> d(5);
    d = v;
    d.clear();
    d = v;
    d.insert(d.give_me(0), 0);
    for (auto i = d.begin(); i < d.end();i++) { cout << *i << "\t"; }
    
}
