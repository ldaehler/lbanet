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


#if !defined(__LbaNetModel_1_Dialog_h)
#define __LbaNetModel_1_Dialog_h

class ConditionBase;
class DialogTreePlayerChoice;
class DialogEntry;
class DialogTreeRoot;



#include <vector>
#include "WorldInfo.h"



typedef boost::shared_ptr<DialogEntry> DialogEntryPtr;
typedef boost::shared_ptr<DialogTreePlayerChoice> DialogTreePlayerChoicePtr;
typedef boost::shared_ptr<DialogTreeRoot> DialogTreeRootPtr;


/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Entry used by a NPC when the player talk to him
 *
 *
 * Type :
 * 0 - normal DialogEntry
 * 1 - ConditionnalDialogEntry
 * 2 - RandomDialogEntry
 * 3 - DialogEntryList
 *
 *********************************************************************/
class DialogEntry
{
public:
	//! constructor
	DialogEntry(long TextId)
		: _TextId(TextId){}

	//! destructor
	virtual ~DialogEntry(){}


	//! get dialog entry
	virtual long GetText()
	{ 
		if(IsValid())
			return _TextId;
		else
			return -1;
	}


	//! return true if entry is valid and can be said
	virtual bool IsValid()
	{ return true; }


protected:
	long _TextId;
};



/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Entry used by a NPC when the player talk to him
 *********************************************************************/
class ConditionnalDialogEntry : public DialogEntry
{
public:
	//! constructor
	ConditionnalDialogEntry(long TextId, ConditionBasePtr condition)
		: DialogEntry(TextId), _condition(condition){}

	//! destructor
	virtual ~ConditionnalDialogEntry(){}


	//! return true if entry is valid and can be said
	virtual bool IsValid();


protected:
	ConditionBasePtr _condition;
};


/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Entry used by a NPC when the player talk to him
 *********************************************************************/
class RandomDialogEntry : public DialogEntry
{
public:
	//! constructor
	RandomDialogEntry(std::vector<DialogEntryPtr> Entries)
		: _Entries(Entries), DialogEntry(-1){}

	//! destructor
	virtual ~RandomDialogEntry(){}


	//! get a random dialog entry
	virtual long GetText();


protected:
	std::vector<DialogEntryPtr> _Entries;
};



/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Entry used by a NPC when the player talk to him
 *********************************************************************/
class DialogEntryList : public DialogEntry
{
public:
	//! constructor
	DialogEntryList(std::vector<DialogEntryPtr> Entries)
		: _Entries(Entries), DialogEntry(-1){}

	//! destructor
	virtual ~DialogEntryList(){}


	//! get first valid dialog entry
	virtual long GetText();


protected:
	std::vector<DialogEntryPtr> _Entries;
};



/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Declaration of the class DialogTreeRoot
 *********************************************************************/
class DialogTreeRoot
{
public:
	//! constructor
	DialogTreeRoot(long TextId, std::vector<DialogTreePlayerChoicePtr> PChoices,
						std::vector<long> QuestsToStart,
						std::vector<long> QuestsToTrigger,
						bool QuitDialogEnabled, bool GoToRootEnabled,
						long CustomQuitDialog, long CustomGoToRootDialog)
		: _TextId(TextId), _PChoices(PChoices), 
			_QuestsToStart(QuestsToStart), _QuestsToTrigger(QuestsToTrigger),
			_QuitDialogEnabled(QuitDialogEnabled), _GoToRootEnabled(GoToRootEnabled),
			_CustomQuitDialog(CustomQuitDialog), _CustomGoToRootDialog(CustomGoToRootDialog)
	{}

	//! destructor
	virtual ~DialogTreeRoot(){}

	//! get dialog entry
	long GetText() const{ return _TextId;}

	//! get player choice
	DialogTreePlayerChoicePtr GetPlayerChoice(size_t idx);	

	//! get all choices
	const std::vector<DialogTreePlayerChoicePtr> &GetAllChoices()
	{return _PChoices;}

	//! trigger any attached event to the dialog
	void Trigger();

	//! accessors
	bool QuitDialogEnabled(){return _QuitDialogEnabled;}
	bool GoToRootEnabled(){return _GoToRootEnabled;}	
	long CustomQuitDialog(){return _CustomQuitDialog;}
	long CustomGoToRootDialog(){return _CustomGoToRootDialog;}

protected:
	long _TextId;
	std::vector<DialogTreePlayerChoicePtr> _PChoices;
	std::vector<long> _QuestsToStart;
	std::vector<long> _QuestsToTrigger;
	bool _QuitDialogEnabled;
	bool _GoToRootEnabled;
	long _CustomQuitDialog;
	long _CustomGoToRootDialog;
};



/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Declaration of the class DialogTreeElement
 *
 *
 * type:
 * 0 - DialogTreePlayerChoice
 * 1 - DialogTreePlayerChoice with validation condition
 * 2 - ConditionalDialogTreePlayerChoice
 * 3 - ConditionalDialogTreePlayerChoice with validation condition
 *
 *
  *********************************************************************/
class DialogTreePlayerChoice
{
public:
	//! constructor
	DialogTreePlayerChoice(long TextId, ConditionBasePtr validcondition,
								DialogTreeRootPtr Next)
			: _TextId(TextId), _NextElement(Next), _validcondition(validcondition)
	{}

	//! destructor
	virtual ~DialogTreePlayerChoice(){}

	//! get next root element
	virtual DialogTreeRootPtr GetNextRoot()
	{return _NextElement;}

	//! get dialog entry
	long GetText() const{ return _TextId;}

	//! check if entry is valid and can be displayed
	virtual bool IsValid();

protected:
	long _TextId;
	DialogTreeRootPtr _NextElement;
	ConditionBasePtr _validcondition;
};



/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Declaration of the class DialogTreeElement
 *********************************************************************/
class ConditionalDialogTreePlayerChoice : public DialogTreePlayerChoice
{
public:
	//! constructor
	ConditionalDialogTreePlayerChoice(long TextId, ConditionBasePtr validcondition, 
											DialogTreeRootPtr Next, 
											DialogTreeRootPtr NextConditionFalse,
											ConditionBasePtr condition
										)
		: DialogTreePlayerChoice(TextId, validcondition, Next), 
			_NextConditionFalse(NextConditionFalse), _Condition(condition)
	{}

	//! destructor
	virtual ~ConditionalDialogTreePlayerChoice(){}

	//! get next root element
	virtual DialogTreeRootPtr GetNextRoot();

private:

	DialogTreeRootPtr _NextConditionFalse;
	ConditionBasePtr _Condition;
};






/***********************************************************************
 * Module:  Dialog.h
 * Author:  vivien
 * Modified: lundi 10 juillet 2009 14:53:50
 * Purpose: Declaration of the class DialogHandler
 *********************************************************************/
class DialogHandler
{
public:
	//! constructor
	DialogHandler(DialogEntryPtr FirstLiner, DialogTreeRootPtr Tree,
					long StandardQuitDialog, long StandardGoToRootDialog,
					long TradingDialog)
		: _FirstLiner(FirstLiner), _TreeRoot(Tree),
			_StandardQuitDialog(StandardQuitDialog), 
			_StandardGoToRootDialog(StandardGoToRootDialog),
			_TradingDialog(TradingDialog), _currentRoot(Tree)
	{}

	//! destructor
	virtual ~DialogHandler(){}


	//! dialog follow the player choice
	void FollowPlayerChoice(size_t ChoiceIdx);

	//! return current dialog to be displayed
	DialogDisplay GetCurrentDialog();

	//! reset dialog to root
	void ResetDialog();


private:
	DialogEntryPtr _FirstLiner;
	DialogTreeRootPtr _TreeRoot;
	long _StandardQuitDialog;
	long _StandardGoToRootDialog;
	long _TradingDialog;

	DialogTreeRootPtr _currentRoot;
};


#endif