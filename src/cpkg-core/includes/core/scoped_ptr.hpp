/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once

namespace cpkg::core
{
  template<typename T>
  class scoped_ptr {
    public:
      explicit scoped_ptr(T* ptr = nullptr) : ptr_(ptr) {}

      ~scoped_ptr() {
        delete ptr_;
      }

      scoped_ptr(scoped_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
      }

      scoped_ptr& operator=(scoped_ptr&& other) noexcept {
        if (this != &other) {
          delete ptr_;
          ptr_ = other.ptr_;
          other.ptr_ = nullptr;
        }
        return *this;
      }

      scoped_ptr(const scoped_ptr&) = delete;
      scoped_ptr& operator=(const scoped_ptr&) = delete;

      T& operator*() const {
        return *ptr_;
      }

      T* operator->() const {
        return ptr_;
      }

      operator bool() {
        return ptr_ != nullptr;
      }

      operator T*()
      {
        return ptr_;
      }

      operator T()
      {
        return *ptr_;
      }

      T* get() const {
        return ptr_;
      }

      T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
      }

      void reset(T* ptr = nullptr) {
        delete ptr_;
        ptr_ = ptr;
      }

    private:
      T* ptr_;
  };

  template<typename Type>
  scoped_ptr<Type> make_scoped(Type value)
  {
    return scoped_ptr<Type>(new Type(value));
  }

  template<typename Type>
  scoped_ptr<Type> make_scoped(Type* value)
  {
    return scoped_ptr<Type>(value);
  }
}