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


#if !defined(__LbaNetModel_1_ConditionBase_h)
#define __LbaNetModel_1_ConditionBase_h

#include <vector>
#include <boost/shared_ptr.hpp>
class ConditionBase;
typedef boost::shared_ptr<ConditionBase> ConditionBasePtr;


/***********************************************************************
 * Module:  ConditionBase.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class ConditionBase
 *
 *
 * List of possible conditions:
 *
 * 0 - always true
 * 1 - negate condition 
 * 2 - list of condition
 * 3 - quest started
 * 4 - quest finished
 * 5 - quest condition passed
 * 6 - object in inventory
 * 10 - custom condition using Lua scripts
 *
 *
  *********************************************************************/
class ConditionBase
{
public:
	//! constructor
	ConditionBase(){}

	//! destructor
	virtual ~ConditionBase(){}

	//! check if the condition is true or not
	virtual bool Passed() = 0;
	
};


//! condition is always true
class AlwaysTrueCondition : public ConditionBase
{
public:
	//! constructor
	AlwaysTrueCondition(){}

	//! check if the condition is true or not
	virtual bool Passed()
	{ return true;}
	
};


//! take the negation of a condition
class NegateCondition : public ConditionBase
{
public:
	//! constructor
	NegateCondition(ConditionBasePtr tonegate)
		: _tonegate(tonegate)
	{}

	//! destructor
	virtual ~NegateCondition(){}

	//! check if the condition is true or not
	virtual bool Passed()
	{ return !(_tonegate->Passed());}
	

private:
	ConditionBasePtr _tonegate;
};


//! check for a list of condition to be true
class ConditionList : public ConditionBase
{
public:
	//! constructor
	ConditionList(std::vector<ConditionBasePtr> list)
		: _clist(list)
	{}

	//! destructor
	virtual ~ConditionList(){}

	//! check if the condition is true or not
	virtual bool Passed()
	{ 
		for(size_t i=0; i<_clist.size(); ++i)
		{
			if(!_clist[i]->Passed())
				return false;
		}

		return true;
	}
	

private:
	std::vector<ConditionBasePtr> _clist;
};



#endif