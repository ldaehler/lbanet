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


#if !defined(__LbaNetModel_1_QuestHanlder_h)
#define __LbaNetModel_1_QuestHanlder_h


#include "WorldInfo.h"
#include "Quest.h"
#include <IceUtil/RecMutex.h>
#include "InventoryHandlerBase.h"

/***********************************************************************
 * Module:  QuestHandler.h
 * Author:  vivien
 * Modified: lundi 27 juillet 2009 14:53:50
 * Purpose: Declaration of the class QuestHandler
 *********************************************************************/
class QuestHandler
{
public:
	// singleton pattern
	static QuestHandler * getInstance();

	//! constructor
	QuestHandler(InventoryHandlerBase * InvH);

	//! destructor
	virtual ~QuestHandler(){}

	//! init QH with quest database
	void Initialize(std::map<long, QuestPtr> questDb);

	//! set started and finished quests
	void SetStartedFinished(std::vector<long> questStarted, std::vector<long> questFinished);

	//! ask QH to start a quest
	void TriggerQuestStart(long QuestId);

	//! ask QH to end a quest
	void TriggerQuestEnd(long QuestId);

	//! return true if quest is started and condition are fullfiled
	bool QuestConditionFulfilled(long QuestId);

	//! return true if quest is started
	bool QuestStarted(long QuestId);

	//! return true if quest is started
	bool QuestFinished(long QuestId);

	//! get quests started
	std::vector<long> GetQuestsStarted();

	//! get quests finished
	std::vector<long> GetQuestsFinished();

	//! get quest info
	QuestInfo GetQuestInfo(long QuestId);

private:
	std::map<long, QuestPtr>	_questDb;
	std::vector<long>			_questStarted;
	std::vector<long>			_questFinished;

	IceUtil::RecMutex			m_mutex;

	InventoryHandlerBase *		_InvH;

	// singleton
	static QuestHandler *		_singletonInstance;
};

#endif