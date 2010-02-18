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

#include "Quest.h"


/***********************************************************
constructor
***********************************************************/
Quest::Quest(long QuestId, long titleTextId, long descriptionTextId,
		std::vector<ConditionBasePtr> conditionsToSucceed,
		std::vector<long> questsStartingAtStart,
		std::vector<long> questsStartingAtEnd,
		std::vector<long> questsTriggeredAtEnd,

		std::vector<std::pair<long, int> > objectsGivenAtEnd,
		std::vector<std::pair<long, int> > objectsTakenAtEnd,
		bool Visible)

	: _QuestId(QuestId), _titleTextId(titleTextId), 
		_descriptionTextId(descriptionTextId), _conditionsToSucceed(conditionsToSucceed),
		_questsStartingAtStart(questsStartingAtStart),
		_questsStartingAtEnd(questsStartingAtEnd),
		_questsTriggeredAtEnd(questsTriggeredAtEnd),
		_objectsGivenAtEnd(objectsGivenAtEnd),
		_objectsTakenAtEnd(objectsTakenAtEnd), _Visible(Visible)
{

}




/***********************************************************
check if condition are passed to finish quest
***********************************************************/
bool Quest::CheckCondition()
{
	for(size_t i=0; i<_conditionsToSucceed.size(); ++i)
	{
		if(!_conditionsToSucceed[i]->Passed())
			return false;
	}

	return true;
}

