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


#if !defined(__LbaNetModel_1_QuestCondition_h)
#define __LbaNetModel_1_QuestCondition_h

#include "ConditionBase.h"
class QuestHandler;

/***********************************************************************
 * Module:  QuestCondition.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class QuestStartedCondition
 *********************************************************************/
class QuestStartedCondition : public ConditionBase
{
public:
	//! constructor
	QuestStartedCondition(long QuestId, QuestHandler*  qH)
		: _QuestId(QuestId), _qH(qH)
	{}

	//! destructor
	virtual ~QuestStartedCondition(){}

	//! check if the condition is true or not
	virtual bool Passed();

private:
	long _QuestId;
	QuestHandler *  _qH;
};


/***********************************************************************
 * Module:  QuestCondition.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class QuestFinishedCondition
 *********************************************************************/
class QuestFinishedCondition : public ConditionBase
{
public:
	//! constructor
	QuestFinishedCondition(long QuestId, QuestHandler*  qH)
		: _QuestId(QuestId), _qH(qH)
	{}

	//! destructor
	virtual ~QuestFinishedCondition(){}

	//! check if the condition is true or not
	virtual bool Passed();

private:
	long _QuestId;
	QuestHandler *  _qH;
};



/***********************************************************************
 * Module:  QuestCondition.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class QuestNotDoneCondition
 *********************************************************************/
class QuestNotDoneCondition : public ConditionBase
{
public:
	//! constructor
	QuestNotDoneCondition(long QuestId, QuestHandler*  qH)
		: _QuestId(QuestId), _qH(qH)
	{}

	//! destructor
	virtual ~QuestNotDoneCondition(){}

	//! check if the condition is true or not
	virtual bool Passed();

private:
	long _QuestId;
	QuestHandler *  _qH;
};


/***********************************************************************
 * Module:  QuestCondition.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class QuestConditionsPassedCondition
 *********************************************************************/
class QuestConditionsPassedCondition : public ConditionBase
{
public:
	//! constructor
	QuestConditionsPassedCondition(long QuestId, QuestHandler*  qH)
		: _QuestId(QuestId), _qH(qH)
	{}

	//! destructor
	virtual ~QuestConditionsPassedCondition(){}

	//! check if the condition is true or not
	virtual bool Passed();

private:
	long _QuestId;
	QuestHandler *  _qH;
};


#endif