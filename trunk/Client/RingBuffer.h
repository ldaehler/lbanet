/*
------------------------[ Lbanet Source ]-------------------------
Copyright (C) 2009
Author: Vivien Delage [Rincevent_123]
Email : vdelage@gmail.com

-------------------------------[ GNU License ]-------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------------
*/


#ifndef _RING_BUFFER_H__
#define _RING_BUFFER_H__

#include <cassert>


/*
*************************************************************************************************
* class RingBuffer<P_EType>
*************************************************************************************************
*/
template
<
	typename P_EType
>
class RingBuffer
{
public:

	typedef P_EType	                           T_EType;
	typedef RingBuffer<T_EType>                T_RingBuffer;

	//! constructor
	explicit RingBuffer( size_t size )
		: _elems( new T_EType[size] )
		, _n_max(size)
		, _size(0)
		, _pos(0)
	{ }

	//! constructor
	RingBuffer( const T_RingBuffer& r )
		: _elems( new T_EType[r._n_max] )
		, _n_max( r._n_max )
		, _size( r._size )
		, _pos( r._pos )
	{
		memcpy( _elems,r._elems,_n_max*sizeof(T_EType) );
	}

	//! operator =
	void operator=( const T_RingBuffer& r )
	{
		T_RingBuffer tmp(r);
		Swap(tmp);
	}

	//! destructor
    virtual ~RingBuffer()
	{
		delete [] _elems;
	}

	//! add element
	size_t Add( const T_EType& e )
	{
		assert( _pos < _n_max );
		assert( _size <= _n_max);

		_elems[_pos] = e;

		_pos  = ( _pos+1 == _n_max ) ? 0 : ++_pos;
		_size = ( _size == _n_max ) ? _n_max : ++_size;

		assert( _pos < _n_max );
		assert( _size <= _n_max && _size > 0 );

		return (_size-1);
	}

	//! accessor
	T_EType& operator[] ( size_t i )
	{
		assert( i < _size );
		return _elems[(First()+i)%_n_max];
	}

	//! accessor
	const T_EType& operator[] ( size_t i ) const
	{
		assert( i < _size );
		return _elems[(First()+i)%_n_max];
	}

	//! get current index
	size_t CurrentIdx(void) const
	{
		return _size-1;
	}

	//! is empty
	bool Empty() const
	{
		return ( _size == 0 );
	}

	//! get current element
	T_EType& Current( void )
	{
		return (*this)[_size-1];
	}

	//! get current element
	const T_EType& Current( void ) const
	{
		return (*this)[_size-1];
	}


	//! get first index
	size_t First(void) const
	{
		return (_pos+_n_max-_size);
	}

	//! clear buffer
	void ClearAll()
	{
		_size = 0;
		_pos = 0;
	}

	//! swap
	void Swap( T_RingBuffer& r )
	{
		size_t itmp = _pos; _pos=r._pos; r._pos=itmp;
		itmp = _size; _size=r._size; r._size=itmp;
		itmp = _n_max; _n_max=r._n_max;r._n_max=itmp;
		T_EType* etmp = _elems; _elems=r._elems;r._elems=etmp;
	}

	//! get size
	size_t Size(void) const { return _size; };

	//! get max size
	size_t Capacity(void) const { return _n_max; };


private:

	//! constructor
	explicit RingBuffer()
		: _elems( NULL )
		, _n_max(0)
		, _size(0)
		, _pos(0)
	{ }

	T_EType*  _elems;

	size_t    _pos;
	size_t    _size;
	size_t    _n_max;
};


#endif
