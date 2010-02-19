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

#include "QuestHandler.h"

#ifndef _LBANET_SERVER_SIDE_
#include "DataLoader.h"
#include "InventoryHandler.h"
#endif


QuestHandler* QuestHandler::_singletonInstance = NULL;

/***********************************************************
singleton pattern
***********************************************************/
QuestHandler * QuestHandler::getInstance()
{
    if(!_singletonInstance)
	{
		#ifndef _LBANET_SERVER_SIDE_
        _singletonInstance = new QuestHandler(InventoryHandler::getInstance());
		#else
		_singletonInstance = new QuestHandler(NULL);
		#endif
	}

	return _singletonInstance;
}


/***********************************************************
constructor
***********************************************************/
QuestHandler::QuestHandler(InventoryHandlerBase * InvH)
: _InvH(InvH)
{

}

/***********************************************************
init QH with quest database
***********************************************************/
void QuestHandler::Initialize(std::map<long, QuestPtr> questDb)
{
	IceUtil::RecMutex::Lock lock(m_mutex);
	_questDb = questDb;
}


/***********************************************************
set started and finished quests
***********************************************************/
void QuestHandler::SetStartedFinished(std::vector<long> questStarted, std::vector<long> questFinished)
{
	IceUtil::RecMutex::Lock lock(m_mutex);
	_questStarted = questStarted;
	_questFinished = questFinished;
}


/***********************************************************
ask QH to start a quest
***********************************************************/
void QuestHandler::TriggerQuestStart(long QuestId)
{
	IceUtil::RecMutex::Lock lock(m_mutex);

	//! quest already started
	if(std::find(_questStarted.begin(), _questStarted.end(), QuestId) != _questStarted.end())
		return;

	//! quest already finished
	if(std::find(_questFinished.begin(), _questFinished.end(), QuestId) != _questFinished.end())
		return;

	_questStarted.push_back(QuestId);


#ifdef _LBANET_SERVER_SIDE_
	// inform client quest is started
	if(_InvH)
		_InvH->InformQuestStarted(QuestId);
	else
		std::cout<<"Problem do not have invh ptr"<<std::endl;

	std::map<long, QuestPtr>::iterator it = _questDb.find(QuestId);
	if(it != _questDb.end())
	{
		const std::vector<long>& listq =it->second->GetQuestsStartingAtStart();
		for(size_t i=0; i<listq.size(); ++i)
			TriggerQuestStart(listq[i]);
	}
#endif
}


/***********************************************************
ask QH to end a quest
***********************************************************/
void QuestHandler::TriggerQuestEnd(long QuestId)
{
	IceUtil::RecMutex::Lock lock(m_mutex);


#ifdef _LBANET_SERVER_SIDE_

	if(QuestConditionFulfilled(QuestId))
	{
		// remove from quest started
		std::vector<long>::iterator it = std::find(_questStarted.begin(), _questStarted.end(), QuestId);
		if(it == _questStarted.end())
			_questStarted.erase(it);

		// add to quest finished
		_questFinished.push_back(QuestId);


		// inform client quest is finished
		if(_InvH)
			_InvH->InformQuestFinished(QuestId);


		// trigger stuff at end quest
		std::map<long, QuestPtr>::iterator itm = _questDb.find(QuestId);
		if(itm != _questDb.end())
		{
			const std::vector<long>& listq =itm->second->GetQuestsStartingAtEnd();
			for(size_t i=0; i<listq.size(); ++i)
				TriggerQuestStart(listq[i]);

			const std::vector<long>& listqT =itm->second->GetQuestsTriggeredAtEnd();
			for(size_t i=0; i<listqT.size(); ++i)
				TriggerQuestEnd(listqT[i]);


			if(_InvH)
			{
				const std::vector<std::pair<long, int> >&givens = itm->second->GetObjectsGivenAtEnd();
				const std::vector<std::pair<long, int> >&takens = itm->second->GetObjectsTakenAtEnd();
				
				for(size_t i=0; i<givens.size(); ++i)
					_InvH->UpdateItemNumber(givens[i].first, givens[i].second);

				for(size_t i=0; i<takens.size(); ++i)
					_InvH->UpdateItemNumber(takens[i].first, -takens[i].second);

			}
		}
	}

#else
	// no need to check in client side has it has been checked by server
	{
		// remove from quest started
		std::vector<long>::iterator it = std::find(_questStarted.begin(), _questStarted.end(), QuestId);
		if(it == _questStarted.end())
			_questStarted.erase(it);

		// add to quest finished
		_questFinished.push_back(QuestId);
	}

#endif

}


/***********************************************************
return true if quest is started and condition are fullfiled
***********************************************************/
bool QuestHandler::QuestConditionFulfilled(long QuestId)
{
	IceUtil::RecMutex::Lock lock(m_mutex);

	//! quest already finished
	if(std::find(_questFinished.begin(), _questFinished.end(), QuestId) != _questFinished.end())
		return false;


	//! check if quest started
	std::vector<long>::iterator it = std::find(_questStarted.begin(), _questStarted.end(), QuestId);
	if(it == _questStarted.end())
		return false;

	std::map<long, QuestPtr>::iterator itm = _questDb.find(QuestId);
	if(itm != _questDb.end())
		return itm->second->CheckCondition();

	return false;
}


/***********************************************************
return true if quest is started
***********************************************************/
bool QuestHandler::QuestStarted(long QuestId)
{	
	IceUtil::RecMutex::Lock lock(m_mutex);
	return (std::find(_questStarted.begin(), _questStarted.end(), QuestId) != _questStarted.end());
}


/***********************************************************
return true if quest is started
***********************************************************/
bool QuestHandler::QuestFinished(long QuestId)
{
	IceUtil::RecMutex::Lock lock(m_mutex);
	return (std::find(_questFinished.begin(), _questFinished.end(), QuestId) != _questFinished.end());
}


/***********************************************************
get quests started
***********************************************************/
std::vector<long> QuestHandler::GetQuestsStarted()
{
	IceUtil::RecMutex::Lock lock(m_mutex);
	return _questStarted;
}


/***********************************************************
get quests finished
***********************************************************/
std::vector<long> QuestHandler::GetQuestsFinished()
{
	IceUtil::RecMutex::Lock lock(m_mutex);
	return _questFinished;
}


/***********************************************************
get quest info
***********************************************************/
QuestInfo QuestHandler::GetQuestInfo(long QuestId)
{
	QuestInfo res;
	res.Id = -1;

	IceUtil::RecMutex::Lock lock(m_mutex);

	std::map<long, QuestPtr>::iterator itm = _questDb.find(QuestId);
	if(itm != _questDb.end())
	{
		res.Id = itm->first;

		#ifndef _LBANET_SERVER_SIDE_
		res.Visible = itm->second->GetVisible();
		if(res.Visible)
		{
			res.Tittle = DataLoader::getInstance()->GetQuestText(itm->second->GetTitle());
			res.Description = DataLoader::getInstance()->GetQuestText(itm->second->GetDescription());
			res.QuestArea = DataLoader::getInstance()->GetQuestText(itm->second->GetQuestArea());
		}
		#endif
	}

	return res;
}
