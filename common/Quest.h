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


#if !defined(__LbaNetModel_1_Quest_h)
#define __LbaNetModel_1_Quest_h


#include "WorldInfo.h"
/***********************************************************************
 * Module:  Quest.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class Quest
 *********************************************************************/
class Quest
{
public:
	//! constructor
	Quest(long QuestId, long titleTextId, long descriptionTextId,
			std::vector<ConditionBasePtr> conditionsToSucceed,
			std::vector<long> questsStartingAtStart,
			std::vector<long> questsStartingAtEnd,
			std::vector<long> questsTriggeredAtEnd,

			std::vector<std::pair<long, int> > objectsGivenAtEnd,
			std::vector<std::pair<long, int> > objectsTakenAtEnd);

	//! destructor
	virtual ~Quest(){}


	//! check if condition are passed to finish quest
	bool CheckCondition();




	//! accessors
	long GetTitle(){ return _titleTextId; }
	long GetDescription(){ return _descriptionTextId; }

	const std::vector<ConditionBasePtr>&	GetConditions(){ return _conditionsToSucceed;}
	const std::vector<long>&				GetQuestsStartingAtStart(){ return _questsStartingAtStart;}
	const std::vector<long>&				GetQuestsStartingAtEnd(){ return _questsStartingAtEnd;}
	const std::vector<long>&				GetQuestsTriggeredAtEnd(){ return _questsTriggeredAtEnd;}

	const std::vector<std::pair<long, int> >& GetObjectsGivenAtEnd(){ return _objectsGivenAtEnd;}
	const std::vector<std::pair<long, int> >&GetObjectsTakenAtEnd(){ return _objectsTakenAtEnd;}

private:
	long							_QuestId;
	long							_titleTextId;
	long							_descriptionTextId;
	std::vector<ConditionBasePtr>	_conditionsToSucceed;
	std::vector<long>				_questsStartingAtStart;
	std::vector<long>				_questsStartingAtEnd;
	std::vector<long>				_questsTriggeredAtEnd;

	std::vector<std::pair<long, int> >	_objectsGivenAtEnd;
	std::vector<std::pair<long, int> >	_objectsTakenAtEnd;
};

#endif