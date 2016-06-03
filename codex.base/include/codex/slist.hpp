/**
 * license
 */
#ifndef __codex_slist_h__
#define __codex_slist_h__

namespace codex {

  /**
   */
  template < class T >
  class slist{
  public:
    ///
    slist( void );
    ///
    ~slist( void );

    ///
    template < class U >
    void swap( slist<U>& rhs );

    ///
    void add_tail( T* node );

    ///
    T* head( void );

    ///
    T* remove_head( void );
  private:
    T* _head;
    T* _tail;
  };

  template < class T >
  slist< T >::slist( void )
    : _head( nullptr ) , _tail( nullptr )
  {
  }
  
  template < class T >
  slist< T >::~slist( void )
  {
  }

  template < class T >
  template < class U >
  void slist< T >::swap( slist<U>& rhs ) {
    std::swap( _head , rhs._head );
    std::swap( _tail , rhs._tail );
  }

  template < class T >
  void slist< T >::add_tail( T* node ) {
    if ( _head == nullptr ){
      _head = _tail = node;
    } else {
      _tail->next( node );
    }
    node->next( nullptr ); 
  }
  
  template < class T >
  T* slist< T >::head( void ) {
    return _head;
  }

  template < class T >
  T* slist< T >::remove_head( void ) {
    T* head = _head;
    if ( _head == _tail ) {
      _head = _tail = nullptr;
    } else {
      _head = _head->next();
    }
    return head;
  }


}

#endif
