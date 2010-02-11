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


#include "Dialog.h"
#include "Randomizer.h"
#include "ConditionBase.h"

#ifndef _LBANET_SERVER_SIDE_
#include "DataLoader.h"
#endif

/***********************************************************
return true if entry is valid and can be said
***********************************************************/
bool ConditionnalDialogEntry::IsValid()
{
	if(_condition)
		return _condition->Passed();
	else
		return true;
}


/***********************************************************
get a random dialog entry
***********************************************************/
long RandomDialogEntry::GetText()
{
	if(_Entries.size() == 0)
		return -1;

	int idx = Randomizer::getInstance()->RandInt((int)_Entries.size());
	return _Entries[idx]->GetText();
}



/***********************************************************
get a random dialog entry
***********************************************************/
long DialogEntryList::GetText()
{
	if(_Entries.size() == 0)

	for(size_t i=0; i<_Entries.size(); ++i)
	{
		if(_Entries[i]->IsValid())
			return _Entries[i]->GetText();
	}

	return -1;
}




/***********************************************************
get player choice
***********************************************************/
DialogTreePlayerChoicePtr DialogTreeRoot::GetPlayerChoice(size_t idx)
{
	if(idx >= _PChoices.size())
		return DialogTreePlayerChoicePtr();

	return _PChoices[idx];
}



/***********************************************************
trigger any attached event to the dialog
***********************************************************/
void DialogTreeRoot::Trigger()
{

}



/***********************************************************
 check if entry is valid and can be displayed
***********************************************************/
bool DialogTreePlayerChoice::IsValid()
{
	if(_validcondition)
		return _validcondition->Passed();

	return true;
}



/***********************************************************
get next root element
***********************************************************/
DialogTreeRootPtr ConditionalDialogTreePlayerChoice::GetNextRoot()
{
	if(_Condition && _Condition->Passed())
		return _NextElement;
	else
		return _NextConditionFalse;
}






/***********************************************************
dialog follow the player choice
***********************************************************/
void DialogHandler::FollowPlayerChoice(size_t ChoiceIdx)
{
	if(_currentRoot)
	{
		DialogTreePlayerChoicePtr pc = _currentRoot->GetPlayerChoice(ChoiceIdx);
		if(pc)
		{
			_currentRoot = pc->GetNextRoot();

		}
	}
}

/***********************************************************
return current dialog to be displayed
***********************************************************/
DialogDisplay DialogHandler::GetCurrentDialog()
{
	DialogDisplay res;
	if(_currentRoot)
	{
		// trigger attached event
		_currentRoot->Trigger();


		long tid = -1;
		
		// if we are on the dialog root
		if(_currentRoot == _TreeRoot)
			tid = _FirstLiner->GetText();
		else
			tid = _currentRoot->GetText();

		#ifndef _LBANET_SERVER_SIDE_
		if(tid >= 0)
			res.NPCTalk = DataLoader::getInstance()->GetText(tid);
		#endif

		// build dialog player choices
		const std::vector<DialogTreePlayerChoicePtr> &vecchoices = _currentRoot->GetAllChoices();
		for(size_t i=0; i<vecchoices.size(); ++i)
		{
			if(vecchoices[i]->IsValid())
			{
				long cid = vecchoices[i]->GetText();
				if(cid >= 0)
				{
					PlayerChoiceDisplay pcd;

					#ifndef _LBANET_SERVER_SIDE_
					pcd.Text = DataLoader::getInstance()->GetText(cid);
					#endif

					pcd.QuitDialog = false;
					pcd.ResetDialog = false;
					pcd.StartTrade = false;
					pcd.Index = i;
					res.PlayerChoices.push_back(pcd);
				}
			}
		}
		

		// if we are on the dialog root
		if(_currentRoot == _TreeRoot)
		{
			if(_TradingDialog >= 0)
			{
				PlayerChoiceDisplay pcd;

				#ifndef _LBANET_SERVER_SIDE_
				pcd.Text = DataLoader::getInstance()->GetText(_TradingDialog);
				#endif

				pcd.QuitDialog = false;
				pcd.ResetDialog = false;
				pcd.StartTrade = true;
				res.PlayerChoices.push_back(pcd);
			}
		}
		else
		{
			if(_currentRoot->GoToRootEnabled())
			{
				long ctid = _currentRoot->CustomGoToRootDialog();
				if(ctid < 0)
					ctid = _StandardGoToRootDialog;

				if(ctid >= 0)
				{
					PlayerChoiceDisplay pcd;

					#ifndef _LBANET_SERVER_SIDE_
					pcd.Text = DataLoader::getInstance()->GetText(ctid);
					#endif

					pcd.QuitDialog = false;
					pcd.ResetDialog = true;
					pcd.StartTrade = false;
					res.PlayerChoices.push_back(pcd);
				}
			}
		}


		//! add the quit dialog option if needed
		if(_currentRoot->QuitDialogEnabled())
		{
			long ctid = _currentRoot->CustomQuitDialog();
			if(ctid < 0)
				ctid = _StandardQuitDialog;

			if(ctid >= 0)
			{
				PlayerChoiceDisplay pcd;

				#ifndef _LBANET_SERVER_SIDE_
				pcd.Text = DataLoader::getInstance()->GetText(ctid);
				#endif

				pcd.QuitDialog = true;
				pcd.ResetDialog = false;
				pcd.StartTrade = false;
				res.PlayerChoices.push_back(pcd);
			}
		}
	}
	else
	{
		if(_TradingDialog >= 0)
		{
			PlayerChoiceDisplay pcd;

			#ifndef _LBANET_SERVER_SIDE_
			pcd.Text = DataLoader::getInstance()->GetText(_TradingDialog);
			#endif

			pcd.QuitDialog = false;
			pcd.ResetDialog = false;
			pcd.StartTrade = true;
			res.PlayerChoices.push_back(pcd);
		}

		if(_StandardQuitDialog >= 0)
		{
			PlayerChoiceDisplay pcd;

			#ifndef _LBANET_SERVER_SIDE_
			pcd.Text = DataLoader::getInstance()->GetText(_StandardQuitDialog);
			#endif

			pcd.QuitDialog = true;
			pcd.ResetDialog = false;
			pcd.StartTrade = false;
			res.PlayerChoices.push_back(pcd);
		}
	}

	return res;
}

/***********************************************************
reset dialog to root
***********************************************************/
void DialogHandler::ResetDialog()
{
	_currentRoot = _TreeRoot;
}