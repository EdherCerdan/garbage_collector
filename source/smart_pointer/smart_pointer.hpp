// Copyright 2020 Roger Peralta Aranibar
#ifndef SOURCE_SMART_POINTER_SMART_POINTER_HPP_
#define SOURCE_SMART_POINTER_SMART_POINTER_HPP_
#include <cstddef>
#include <iostream>
#include <ostream>

/*
 * Smart Pointer que implementa un tipo de estrategia por conteo de referencias.
 * Permite que varios SmartPointers puedan acceder al mismo recurso compartido.
 * Cuando el ˙ltimo puntero es eliminado, se elimina y libera el recurso.
 */
class PtrCounter {
public:
    PtrCounter()
        : m_counter(0){};
 
    PtrCounter(const PtrCounter&) = delete;
    PtrCounter& operator=(const PtrCounter&) = delete;
    ~PtrCounter()
    {
    }
 
    void reset() {
        m_counter = 0;
    }
 
    unsigned int get() {
        return m_counter;
    }
    void operator++() {
        m_counter++;
    }
 
    void operator++(int) {
        m_counter++;
    }
    void operator--() {
        m_counter--;
    }
    void operator--(int) {
        m_counter--;
    }
 
private:
    unsigned int m_counter{};
};

template <typename Type>
class SmartPointer {
 private:
  Type *resource_;
  PtrCounter* m_counter;

 public:
  /* Constructor: SmartPointer(Type* resource=NULL)
   * Uso: SmartPointer<string> myPtr(new string);
   *      SmartPointer<string> myPtr;
   * ------------------------------------------------------------
   * Construye un nuevo objeto SmartPointer que administra el recurso
   * asignado. Se asume que el recurso viene de una llamada a new.
   * El recurso también podría ser NULL lo que ocasionaría que el
   * recurso no administre ningún recurso.
   */
  explicit SmartPointer(Type *resource = nullptr) :resource_(resource) {
        resource_ = resource;
        m_counter = new PtrCounter();
        if (resource) {
            (*m_counter)++;
        }
  }

  /* Destructor: ~SmartPointer();
   * Uso: (implícito)
   * ------------------------------------------------------------
   * Decrementa el contador de referencias del recurso, eliminando
   * y liberando la memoria si fuera el último SmartPointer apuntando
   * al recurso.
   */
  ~SmartPointer() {
        (*m_counter)--;
        if (m_counter->get() == 0) {
            delete m_counter;
            delete resource_;
        }
  }

  /* SmartPointer operadores de "des-referencia"(dereference)
   * Uso: cout << *myPtr << endl;
   * Uso: cout << myPtr->length << endl;
   * ------------------------------------------------------------
   * Permite al SmartPointer comportarse como si fuera un puntero.
   */
  Type &operator*() const { return *resource_; }
  Type *operator->() const { return resource_; }

  /* Funciones de copia
   * Uso: SmartPointer<string> ptr=existingPointer;
   *      ptr=existingPtr;
   * ------------------------------------------------------------
   * Cambia el recurso referenciado por este SmartPointer por otro
   * SmartPointer. Si el conteo llega a cero, debe ser eliminado
   * (deallocated).
   */
  SmartPointer &operator=(const SmartPointer &other) {
    if (resource_ == other.resource_ and m_counter == other.m_counter){
      return *this;
    }
    (*(m_counter))--;
    if (m_counter->get() == 0) {
      delete m_counter;
      delete resource_;
      }
    resource_ = other.resource_;
    m_counter = other.m_counter;
    if (resource_) {
      (*m_counter)++;
    }
    return *this;
  }
  SmartPointer &operator=(Type *other) {
    (*(m_counter))--;
    if (m_counter->get() == 0) {
      delete m_counter;
      delete resource_;
      }
    resource_ = other;
    m_counter = new PtrCounter();
    if (resource_) {
      (*m_counter)++;
    }
    return *this;
  }
  SmartPointer(const SmartPointer &other) {
        resource_ = other.resource_;
        m_counter = other.m_counter;
        (*m_counter)++;
  }

  /* Helper Function: Obtener recurso.
   * Uso: Type* p=GetPointer();
   * ------------------------------------------------------------
   * Retorna una variable puntero al recurso administrado.
   */
  Type *GetPointer() const { return resource_; }

  /* Helper Function: Obtiene conteo
   * Uso: if (ptr.GetReferenceCount()==1) // Única referencia
   * ------------------------------------------------------------
   * Retorna el número de referencias apuntando al recurso.
   */
  size_t GetReferenceCount() const { return m_counter->get(); }

  /* Helper Function: se des-asocia del recurso;
   * Uso: ptr.Detach();
   * ------------------------------------------------------------
   * Deja de administrar un recurso. eliminando y liberando la
   * memoria si es necesario.
   */
  void Detach() {
  }
};

#endif  // SOURCE_SMART_POINTER_SMART_POINTER_HPP_