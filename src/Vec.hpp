// Vec.hpp
#ifndef GUARD_VEC_H
#define GUARD_VEC_H

template <class T>

class Vec {

public:

    typedef T value_type;
    
    Vec();

    Vec(size_t n, const value_type &val);

    typedef value_type *iterator;

    typedef const value_type *const_iterator;

    size_t size() const {return n_;}

    iterator begin() {return data_;}

    iterator end() {return data_+n_;}

    const_iterator begin() const {return data_;}

    const_iterator end() const {return data_+n_;}

    value_type &operator[](size_t i) {
        assert(i < n_);
        return data_[i];
    }

    const value_type &operator[](size_t i) const {
        assert(i < n_);
        return data_[i];
    }

    void reserve(size_t cap);

    Vec(const Vec<T> &rhs);

    Vec<T> &operator=(const Vec<T> &rhs);

    void swap(Vec<T> &rhs) {
        std::swap(data_, rhs.data_);
        std::swap(n_, rhs.n_);
        std::swap(capacity_, rhs.capacity_);
    }

private:

    value_type *data_;

    size_t n_;

    size_t capacity_;

    static value_type *allocate(size_t n);

    static void deallocate(value_type *p);

    static void uninitialized_fill(value_type *uninit_b, value_type *uninit_e, const value_type &val);

    static void uninitialized_copy(const value_type *from_b, const value_type *from_e, value_type *uninit_b);

    // within class Vec<T>
    static value_type allocate(size_t n) {
    
        size_t num_of_bytes = sizeof(value_type)*n;
    
        void *p = ::operator new[](num_of_bytes);
    
        return (value_type *)p;
    }

    static void deallocate(value_type *p) {
    
        if (p == NULL) 
        
            return;
        
        ::operator delete[](p);
    
    }

    static void uninitialized_fill(value_type *uninit_b, value_type *uninit_e, const value_type &val) {
        value_type *init_b = uninit_b;
        try {
            for (; uninit_b != uninit_e; ++uninit_b) {
                new (uninit_b) value_type(val);
            }
        }
        catch (...) { // catch all exceptions
            for (; init_b != uninit_b; ++init_b) {
                init_b->~value_type(); // call dtor to destroy the object
            }
            throw; // re-throw the exception
        }
    }

    static void uninitialized_copy(const value_type *from_b, const value_type *from_e, value_type *uninit_b) {
        value_type *init_b = uninit_b;
        try {
            for (; from_b != from_e; ++from_b, ++uninit_b) {
                new (uninit_b) value_type(*from_b);
            }
        }
        catch (...) { // catch all exceptions
            for (; init_b != uninit_b; ++init_b) {
                init_b->~value_type(); // call dtor to destroy the object
            }
            throw; // re-throw the exception
        }
    }

    Vec() : data_(NULL), n_(0), capacity_(0) {}

    Vec(size_t n, const value_type &val) : data_(allocate(n)), n_(n), capacity_(n) {
        try {
            uninitialized_fill(data_, data_+n_, val);
        }
        catch (...) {
            deallocate(data_);
            throw;
        }
    }

    Vec::~Vec() {
        for (size_t i = 0; i < n_; ++i) {
            data_[i].~value_type();
        }
        deallocate(data_);
    }

    void reserve(size_t cap) {
        if (cap <= capacity_)
            return; // nothing to do if there is enough memory
        // prepare new memory/objects
        value_type *p = allocate(cap);
        try {
            uninitialized_copy(data_, data_+n, p);
        }
        catch (...) {
            deallocate(p);
            throw;
        }
        // get rid of old objects/memory
        for (size_t i = 0; i < n_; ++i) {
            data_[i].~value_type();
        }
        deallocate(data_);
        // update members
        data_ = p;
        capacity_ = cap;
    }

    void push_back(const value_type &val) {
        if (n_ == capacity_) {
            reserve(std::max(n_+1, n_*2));
        }
        new (data_+n_) value_type(val);
        ++n_;
    }

    void pop_back() {
        assert(n_ > 0);
        data_[n_-1].~value_type();
        --n_;
    }

    Vec(const Vec<T> &rhs) : data_(allocate(rhs.n_)), n_(rhs.n_), capacity_(rhs.n_) {
        try {
            uninitialized_copy(rhs.data_, rhs.data_+n, data_);
        }
        catch (...) {
            deallocate(data_);
            throw;
        }
    }

    Vec<int> increment(Vec<int> v) {
        for (size_t i = 0; i < v.size(); ++i) {
            ++v[i];
        }
        return v;
    }

    Vec<T> &operator=(const Vec<T> &rhs) {
        if (this == &rhs)
            return *this; // nothing to do for self-assignment
        // prepare new memory/objects
        value_type *p = allocate(rhs.n_);
        try {
            uninitialized_copy(rhs_.data_, rhs_.data_+rhs_.n_, p);
        }
        catch (...) {
            deallocate(p);
            throw;
        }
        // get rid of old objects/memory
        for (size_t i = 0; i < n_; ++i) {
            data_[i].~value_type();
        }
        deallocate(data_);
        // update members
        data_ = p; n_ = capacity_ = rhs.n_;
        return *this;
    }

    Vec<T> &operator=(const Vec<T> &rhs) {
        if (this == &rhs)
            return *this; // nothing to do for self-assignment
        Vec<T> copy = rhs;
        swap(copy);
        return *this;
    }

}; // class Vec<T>

#endif
