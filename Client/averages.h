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


#ifndef __LBANET_AVERAGE_H__
#define __LBANET_AVERAGE_H__

#include <cmath>
#include "RingBuffer.h"

/*
************************************************************************************************************************
*                                                  class MeanValue
************************************************************************************************************************
*/
class MeanValue
{

public:

	MeanValue( void )
		: _n(0 )
		, _M(0.)
		, _Q(0.)
	{ }

	explicit MeanValue( double StartingValue)
		: _n(0 )
		, _M(0.)
		, _Q(0.)
	{
		Update(StartingValue);
	}

	explicit MeanValue( int dummy, double StartingValue)
		: _n(0 )
		, _M(0.)
		, _Q(0.)
	{
		Update(StartingValue);
	}

	double Update( double val )
	{
		if ( _n == 0 )
		{
			_M = val; _Q = 0.;
			++_n;
			return _M;
		}

		double k( static_cast<double>(_n+1) );
		double k1( static_cast<double>(_n) );
		double diff( val-_M);

		_M += diff/k;
		_Q += k1*diff*diff/k;

		++_n;
		return _M;
	}

	unsigned long Elements( void ) const { return _n; }
	double Variance( void ) const
		{ return (_n != 0 ) ? _Q/static_cast<double>(_n) : 0.; }
	double Value( void ) const { return _M; }

	void Reset( void )	{ _M = 0; _Q = 0.; _n = 0; }
	void Restore( double M, double Q, unsigned long n ) { _M = M; _Q = Q; _n = n; }

	double & M(){return _M;}
	double & Q(){return _Q;}
	unsigned long & N(){return _n;}

protected:

	double			_M;
	double			_Q;
	unsigned long	_n;
};



/*
************************************************************************************************************************
*                                                  class MA
************************************************************************************************************************
*/
class MA
{

public:

	MA()
		: _data( 0 )
		, _val ( 0. )
		, _var ( 0. )
	{ }

	explicit MA( unsigned long w_length )
		: _data( w_length )
		, _val ( 0. )
		, _var ( 0. )
	{ }

	double Update( double val )
	{
		_data.Add( val );

		MeanValue v;
		for(size_t i=0; i<_data.Size(); ++i) 
			v.Update(_data[i]);

		_val = v.Value();
		_var = v.Variance();

		return _val;
	}


	double Value( void ) const { return _val; }
	double Variance( void ) const { return _var; }

	unsigned long Elements( void ) const { return static_cast<unsigned long>(_data.Size()); }
	unsigned long WindowLength( void ) const { return static_cast<unsigned long>(_data.Capacity()); }

	void Reset( void )
	{ _val = 0.; _var = 0; _data.ClearAll(); }

protected:

	typedef RingBuffer<double> T_array;

	T_array _data;
	double  _val;
	double  _var;
};


/*
************************************************************************************************************************
*                                                  class EMA
************************************************************************************************************************
*/
class EMA
{
public:

	explicit EMA( unsigned long period, double StartingValue )
		: _ema	  ( 0.)
		, _n      ( 0 )
		, _alpha  ( 2./static_cast<double>(period+1) )
	{
		Update(StartingValue);
	}

	explicit EMA( unsigned long period )
		: _ema	  ( 0.)
		, _n      ( 0 )
		, _alpha  ( 2./static_cast<double>(period+1) )
	{
	}

	EMA( double alpha )
		: _ema	  ( 0. )
		, _alpha  ( alpha )
	{
	}

	EMA()
		: _ema	  ( 0.)
		, _n      ( 0 )
		, _alpha  ( 0 )
	{}

	void setParam( unsigned long period )
	{
		_alpha  = ( 2./static_cast<double>(period+1) );
	}


	double Update( double val )
	{
		_ema *= (1.-_alpha);
		_ema += _alpha*val;
		++_n;
		return _ema;
	}

	bool IsInitialized( void ) const
	{
		const double required_accuracy( 0.999 );
		return ( TermLength( required_accuracy ) < _n );
	}

	void Reset( void )
		{ _ema = 0.; _n = 0; }

	double Value( void ) const { return _ema; }

	unsigned long TermLength( double influence ) const
	{
		return static_cast<unsigned long> (log( (1.-influence)*_alpha )/log(1.-_alpha));
	}

	unsigned long Elements( void ) const { return _n; }

protected:

	double			_ema;
	unsigned long	_n;
	double			_alpha;

};



#endif