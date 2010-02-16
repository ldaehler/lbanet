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

#include "QuestCondition.h"
#include "QuestHandler.h"

/***********************************************************
check if the condition is true or not
***********************************************************/
bool QuestStartedCondition::Passed()
{
	if(_qH)
		return _qH->QuestStarted(_QuestId);

	return false;
}


/***********************************************************
check if the condition is true or not
***********************************************************/
bool QuestFinishedCondition::Passed()
{
	if(_qH)
		return _qH->QuestFinished(_QuestId);

	return false;
}


/***********************************************************
check if the condition is true or not
***********************************************************/
bool QuestNotDoneCondition::Passed()
{
	if(_qH)
		return ((!_qH->QuestStarted(_QuestId)) && (!_qH->QuestFinished(_QuestId)));

	return false;
}


/***********************************************************
check if the condition is true or not
***********************************************************/
bool QuestConditionsPassedCondition::Passed()
{
	if(_qH)
		return _qH->QuestConditionFulfilled(_QuestId);

	return false;
}