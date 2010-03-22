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


#include "MapInfoXmlReader.h"
#include "tinyxml.h"

//#include "Actor.h"
//#include "TextActor.h"
//#include "LadderActor.h"
//#include "ExitActor.h"
//#include "DoorActor.h"
//#include "ContainerActor.h"
//#include "UpExitActor.h"
//#include "SwitchActor.h"
//#include "FloorSwitch.h"
//#include "AreaSwitch.h"
//#include "ScriptableActor.h"
//#include "GameEvents.h"
//#include "SignalerBase.h"
//#include "HurtArea.h"
//#include "LivingActor.h"
//#include "NPCActor.h"
//#include "ScriptedZoneActor.h"
//#include "3DObjectRenderer.h"
//#include "ConditionBase.h"
//#include "InventoryCondition.h"
//#include "Quest.h"
//#include "QuestCondition.h"
//#include "QuestHandler.h"
//
//
//#ifndef _LBANET_SERVER_SIDE_
//#include "SpriteRenderer.h"
//#include "AviVideoRenderer.h"
//#include "ms3d.h"
//#include "CharacterRenderer.h"
//#else
//#include "ServerCharacterRenderer.h"
//#endif



/***********************************************************
string helper function
***********************************************************/
static void Trim(std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');

		if(pos != std::string::npos)
			str.erase(0, pos);
	}
	else
		str.clear();

}

/***********************************************************
string helper function
***********************************************************/
static void StringTokenize(const std::string& str,
						std::vector<std::string>& tokens,
						const std::string& delimiters)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);



	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		std::string tmp = str.substr(lastPos, pos - lastPos);
		Trim(tmp);
		tokens.push_back(tmp);

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}



/***********************************************************
get the list of available worlds
***********************************************************/
 bool MapInfoXmlReader::GetAvailableWorlds(const std::string &Filename, std::vector<WorldDesc> & list)
 {
	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;


	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem=hDoc.FirstChildElement().Element();
	pElem=pElem->FirstChildElement("World");
	for( pElem; pElem; pElem=pElem->NextSiblingElement())
	{
		WorldDesc desc;
		desc.WorldName = pElem->Attribute("Name");
		desc.FileName = pElem->Attribute("Filename");
		desc.Description = pElem->Attribute("Description");
		list.push_back(desc);
	}

	return true;
 }


//// get world description
//void MapInfoXmlReader::GetWorldDescription(const std::string &Filename,
//									std::string &WorldName, std::string &WorldDesc)
//{
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//	TiXmlHandle hRoot(0);
//
//	// block: world attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return;
//
//		WorldName = pElem->Attribute("name");
//
//		// save this for later
//		hRoot=TiXmlHandle(pElem);
//	}
//
//	// block: description
//	{
//		pElem=hRoot.FirstChild( "description" ).Element();
//		if(pElem)
//			WorldDesc=pElem->GetText();
//	}
//}
//
//// load a world information into memory
//bool MapInfoXmlReader::LoadWorld(const std::string &Filename, WorldInfo & res)
//{
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//	TiXmlHandle hRoot(0);
//
//	// block: world attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//		res.Name = pElem->Attribute("name");
//		res.FirstMap = pElem->Attribute("firstmap");
//		res.FirstSpawning = pElem->Attribute("firstsparea");
//
//		// save this for later
//		hRoot=TiXmlHandle(pElem);
//	}
//
//	// block: description
//	{
//		pElem=hRoot.FirstChild( "description" ).Element();
//		if(pElem)
//			res.Description=pElem->GetText();
//	}
//
//	// block: teleport
//	{
//		pElem=hRoot.FirstChild( "teleports" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			TPInfo tp;
//			tp.Name=pElem->Attribute("name");
//			tp.NewMap=pElem->Attribute("map");
//			tp.Spawning=pElem->Attribute("sparea");
//
//			res.Teleports[tp.Name] = tp;
//		}
//	}
//
//	// block: files
//	{
//		pElem=hRoot.FirstChild( "files" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			res.Files[pElem->Attribute("name")] = pElem->Attribute("path");
//		}
//	}
//
//	// block: map
//	{
//		pElem=hRoot.FirstChild( "maps" ).FirstChild("Map").Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			MapInfo mi = LoadMap(pElem);
//			res.Maps[mi.Name] = mi;
//		}
//	}
//
//	return true;
//}
//
///*
//--------------------------------------------------------------------------------------------------
//- load a map into memory
//--------------------------------------------------------------------------------------------------
//*/
//MapInfo MapInfoXmlReader::LoadMap(TiXmlElement* pElem)
//{
//	MapInfo res;
//	res.MusicLoop = 0;
//	TiXmlHandle hRoot(0);
//
//	// block: map attributes
//	{
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return res;
//
//		res.Name = pElem->Attribute("name");
//		res.Type = pElem->Attribute("type");
//
//		res.Music = pElem->Attribute("music");
//		int testr = pElem->QueryIntAttribute("repeatmusic", &res.MusicLoop);
//		if(testr != TIXML_SUCCESS)
//			res.MusicLoop = 0;
//
//
//		// save this for later
//		hRoot=TiXmlHandle(pElem);
//	}
//
//	// block: description
//	{
//		pElem=hRoot.FirstChild( "description" ).Element();
//		if(pElem)
//			res.Description=pElem->GetText();
//	}
//
//
//	// block: files
//	{
//		pElem=hRoot.FirstChild( "files" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			res.Files[pElem->Attribute("name")] = pElem->Attribute("path");
//		}
//	}
//
//	// block: lights
//	{
//		pElem=hRoot.FirstChild( "lights" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			LighInfo l;
//			l.Name=pElem->Attribute("name");
//			l.Type=pElem->Attribute("type");
//
//			pElem->QueryIntAttribute("posX", &l.PosX);
//			pElem->QueryIntAttribute("posY", &l.PosY);
//			pElem->QueryIntAttribute("posZ", &l.PosZ);
//			pElem->QueryIntAttribute("dirX", &l.DirX);
//			pElem->QueryIntAttribute("dirY", &l.DirY);
//			pElem->QueryIntAttribute("dirZ", &l.DirZ);
//
//			res.Lights[l.Name] = l;
//		}
//	}
//
//	// block: spawning areas
//	{
//		pElem=hRoot.FirstChild( "spareas" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			SpawningInfo sp;
//			sp.Rotation = 0;
//			sp.Name=pElem->Attribute("name");
//
//			pElem->QueryFloatAttribute("posX", &sp.PosX);
//			pElem->QueryFloatAttribute("posY", &sp.PosY);
//			pElem->QueryFloatAttribute("posZ", &sp.PosZ);
//			pElem->QueryIntAttribute("RotationAtArrival", &sp.Rotation);
//			if(sp.Rotation < 0)
//				sp.Rotation = 0;
//			if(sp.Rotation > 360)
//				sp.Rotation = 0;
//
//			res.Spawnings[sp.Name] = sp;
//		}
//	}
//
//	// block: exit areas
//	{
//		pElem=hRoot.FirstChild( "exits" ).FirstChild().Element();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			ExitInfo ex;
//			ex.Name=pElem->Attribute("name");
//			ex.NewMap=pElem->Attribute("newMap");
//			ex.Spawning=pElem->Attribute("spawning");
//
//			pElem->QueryFloatAttribute("TopRightX", &ex.TopRightX);
//			pElem->QueryFloatAttribute("TopRightY", &ex.TopRightY);
//			pElem->QueryFloatAttribute("TopRightZ", &ex.TopRightZ);
//
//			pElem->QueryFloatAttribute("BottomLeftX", &ex.BottomLeftX);
//			pElem->QueryFloatAttribute("BottomLeftY", &ex.BottomLeftY);
//			pElem->QueryFloatAttribute("BottomLeftZ", &ex.BottomLeftZ);
//
//			res.Exits[ex.Name] = ex;
//		}
//	}
//
//	return res;
//}
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load map actors into memory
//--------------------------------------------------------------------------------------------------
//*/
//bool MapInfoXmlReader::LoadActors(const std::string &Filename, std::map<long, SpriteInfo> &spinfos,
//									std::map<long, SpriteInfo> &vidinfos,
//									std::map<long, ModelInfo> &modelinfos,
//									std::map<long, Actor *> & vec,
//									SignalerBase * signaler, float AnimationSpeed,
//									InventoryHandlerBase * invH, QuestHandler * qH)
//{
//	vec.clear();
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for( pElem; pElem; pElem=pElem->NextSiblingElement())
//		{
//			long id=0, type=0;
//			float posX=0, posY=0, posZ=0;
//			float sizeX=0, sizeY=0, sizeZ=0;
//			int rotation=0;
//			bool passable = true;
//			bool depthmask = true;
//			bool movable = false;
//			long renderertype=-1;
//			std::vector<long> renderertarget;
//			D3ObjectRenderer * renderer = NULL;
//			long outputsignal=-1;
//			long attachedsound=-1;
//			std::vector<long> stargets;
//			const char * targetsstring = pElem->Attribute("signaltargets");
//			if(targetsstring)
//			{
//				std::vector<std::string> tokens;
//				StringTokenize(targetsstring, tokens, ",");
//				for(size_t i=0; i<tokens.size(); ++i)
//					stargets.push_back(atol(tokens[i].c_str()));
//			}
//
//			const char * rendertargetsstring = pElem->Attribute("renderertarget");
//			if(rendertargetsstring)
//			{
//				std::vector<std::string> tokens;
//				StringTokenize(rendertargetsstring, tokens, ",");
//				for(size_t i=0; i<tokens.size(); ++i)
//					renderertarget.push_back(atol(tokens[i].c_str()));
//			}
//
//
//
//			pElem->QueryValueAttribute("id", &id);
//			pElem->QueryValueAttribute("type", &type);
//			pElem->QueryFloatAttribute("posX", &posX);
//			pElem->QueryFloatAttribute("posY", &posY);
//			pElem->QueryFloatAttribute("posZ", &posZ);
//			pElem->QueryFloatAttribute("sizeX", &sizeX);
//			pElem->QueryFloatAttribute("sizeY", &sizeY);
//			pElem->QueryFloatAttribute("sizeZ", &sizeZ);
//			pElem->QueryIntAttribute("rotation", &rotation);
//			pElem->QueryValueAttribute("passable", &passable);
//			pElem->QueryValueAttribute("depthmask", &depthmask);
//			pElem->QueryValueAttribute("movable", &movable);
//			pElem->QueryValueAttribute("outputsignal", &outputsignal);
//			pElem->QueryValueAttribute("attachedsound", &attachedsound);
//
//
//			if(pElem->QueryValueAttribute("renderertype", &renderertype) == TIXML_SUCCESS)
//				if(renderertarget.size() > 0)
//				{
//#ifndef _LBANET_SERVER_SIDE_
//					//renderer = new SpriteRenderer();
//					switch(renderertype)
//					{
//						case 0: // sprite renderer
//							{
//								std::vector<SpriteInfo *> vectmp;
//								for(size_t itar = 0; itar<renderertarget.size(); ++itar)
//								{
//									std::map<long, SpriteInfo>::iterator itsp = spinfos.find(renderertarget[itar]);
//									if(itsp != spinfos.end())
//										vectmp.push_back(&itsp->second);
//								}
//
//								if(vectmp.size() > 0)
//								{
//									SpriteRenderer * tmp = new SpriteRenderer();
//									tmp->SetSprites(vectmp);
//									renderer = tmp;
//								}
//							}
//						break;
//						case 1: // video renderer
//							{
//								std::map<long, SpriteInfo>::iterator itsp = vidinfos.find(renderertarget[0]);
//								if(itsp != vidinfos.end())
//								{
//									std::vector<SpriteInfo *> vectmp;
//									vectmp.push_back(&itsp->second);
//									AviVideoRenderer * tmp = new AviVideoRenderer();
//									tmp->SetSprites(vectmp);
//									renderer = tmp;
//								}
//							}
//						break;
//						case 2: // model renderer
//							{
//								std::map<long, ModelInfo>::iterator itsp = modelinfos.find(renderertarget[0]);
//								if(itsp != modelinfos.end())
//								{
//									MS3DModel * tmp = new MS3DModel();
//									tmp->loadModelData( itsp->second.filename );
//									tmp->SetScale(itsp->second.ScaleX, itsp->second.ScaleY, itsp->second.ScaleZ);
//									tmp->SetTranslation(itsp->second.TransX, itsp->second.TransY, itsp->second.TransZ);
//									tmp->SetRotation(itsp->second.RotX, itsp->second.RotY, itsp->second.RotZ);
//
//
//									renderer = tmp;
//								}
//							}
//						break;
//						case 3: // character renderer
//							{
//								if(renderertarget.size() > 1)
//								{
//									CharacterRenderer * tmp = new CharacterRenderer(AnimationSpeed);
//									tmp->changeAnimEntity(renderertarget[0], renderertarget[1]);
//									tmp->setActorAnimation(0);
//									renderer = tmp;
//								}
//							}
//						break;
//					}
//#else
//					if(renderertype == 3)
//					{
//						if(renderertarget.size() > 1)
//						{
//							ServerCharacterRenderer * tmp = new ServerCharacterRenderer(AnimationSpeed);
//							tmp->changeAnimEntity(renderertarget[0], renderertarget[1]);
//							tmp->setActorAnimation(0);
//							renderer = tmp;
//						}
//					}
//#endif
//				}
//
//
//			Actor * act;
//			switch(type)
//			{
//				case 0:	//basic actor class
//					act = new Actor();
//				break;
//				case 1:	//text actor class
//				{
//					long textid=0;
//					float activationdistance;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					pElem->QueryValueAttribute("textid", &textid);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new TextActor(activationdistance, textid, activationtype);
//				}
//				break;
//				case 2:	//ladder actor class
//				{
//					float deltaX=0, deltaY=0, deltaZ=0;
//					int direction=0;
//					float activationdistance;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					pElem->QueryValueAttribute("deltaX", &deltaX);
//					pElem->QueryValueAttribute("deltaY", &deltaY);
//					pElem->QueryValueAttribute("deltaZ", &deltaZ);
//					pElem->QueryValueAttribute("direction", &direction);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new LadderActor(activationdistance, deltaX, deltaY, deltaZ, direction, activationtype);
//				}
//				break;
//				case 3:	//exit actor class
//				{
//					float deltaX=0, deltaY=0, deltaZ=0;
//					int direction=0;
//					float activationdistance;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					pElem->QueryValueAttribute("deltaX", &deltaX);
//					pElem->QueryValueAttribute("deltaY", &deltaY);
//					pElem->QueryValueAttribute("deltaZ", &deltaZ);
//					pElem->QueryValueAttribute("direction", &direction);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new ExitActor(activationdistance, deltaX, deltaY, deltaZ, direction, activationtype);
//				}
//				break;
//
//				case 4:	//door actor class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					bool locked = false;
//					bool hide = false;
//					long keyid=0;
//					float OpenTransX=0;
//					float OpenTransY=0;
//					float OpenTransZ=0;
//					float OpenTransSpeedX=0;
//					float OpenTransSpeedY=0;
//					float OpenTransSpeedZ=0;
//					bool destroykey = false;
//
//					pElem->QueryValueAttribute("zoneSizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zoneSizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zoneSizeZ", &zoneSizeZ);
//					pElem->QueryValueAttribute("locked", &locked);
//					pElem->QueryValueAttribute("keyid", &keyid);
//					pElem->QueryValueAttribute("hide", &hide);
//					pElem->QueryValueAttribute("destroykey", &destroykey);
//
//					pElem->QueryValueAttribute("OpenTransX", &OpenTransX);
//					pElem->QueryValueAttribute("OpenTransY", &OpenTransY);
//					pElem->QueryValueAttribute("OpenTransZ", &OpenTransZ);
//					pElem->QueryValueAttribute("OpenTransSpeedX", &OpenTransSpeedX);
//					pElem->QueryValueAttribute("OpenTransSpeedY", &OpenTransSpeedY);
//					pElem->QueryValueAttribute("OpenTransSpeedZ", &OpenTransSpeedZ);
//
//
//
//					act = new DoorActor(zoneSizeX, zoneSizeY, zoneSizeZ, locked, keyid, hide,
//											OpenTransX, OpenTransY, OpenTransZ,
//											OpenTransSpeedX, OpenTransSpeedY, OpenTransSpeedZ, destroykey);
//					passable = false;
//				}
//				break;
//
//				case 5:	//container actor class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					ContainerActor *tmpC = new ContainerActor(zoneSizeX, zoneSizeY, zoneSizeZ, activationtype);
//
//					// get the contained items
//					std::vector<ItemGroup> items;
//					TiXmlElement* pElemC=pElem->FirstChildElement();
//					for( pElemC; pElemC; pElemC=pElemC->NextSiblingElement())
//					{
//						// for each group of items
//						ItemGroup newGroup;
//						newGroup.lastSpawningTime = 0;
//						newGroup.RespawningTime = 300000; // default = 5min
//						newGroup.currpicked = -1;
//						pElemC->QueryValueAttribute("respawnTimeInMs", &newGroup.RespawningTime);
//
//						TiXmlElement* itemGroup=pElemC->FirstChildElement();
//						for( itemGroup; itemGroup; itemGroup=itemGroup->NextSiblingElement())
//						{
//							ItemGroupElement elem;
//							itemGroup->QueryValueAttribute("id", &elem.id);
//							itemGroup->QueryValueAttribute("number", &elem.number);
//							itemGroup->QueryValueAttribute("probability", &elem.probability);
//							newGroup.groupelements.push_back(elem);
//						}
//						if(newGroup.groupelements.size() > 0)
//							items.push_back(newGroup);
//					}
//					tmpC->SetLootList(items);
//
//					act = tmpC;
//				}
//				break;
//
//				case 6:	//up exit actor class
//				{
//					int direction=0;
//					float activationdistance;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					pElem->QueryValueAttribute("direction", &direction);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new UpExitActor(activationdistance, direction, activationtype);
//				}
//				break;
//
//				case 7:	//switch actor class
//				{
//					float activationdistance;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new SwitchActor(activationdistance, activationtype);
//				}
//				break;
//
//				case 8:	//area switch actor class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
//					act = new AreaSwitch(zoneSizeX, zoneSizeY, zoneSizeZ);
//				}
//				break;
//
//				case 9:	//floor switch actor class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//					act = new FloorSwitch(zoneSizeX, zoneSizeY, zoneSizeZ, activationtype);
//				}
//				break;
//
//				case 10:	//lift actor class
//				{
//					bool autoattach = true;
//					pElem->QueryValueAttribute("autoattach", &autoattach);					
//
//					std::vector<PlayerScriptPart> scripts;
//					TiXmlNode* pNode2=pElem->FirstChild("scripts");
//					if(pNode2)
//					{
//						TiXmlElement*pElem2=pNode2->FirstChildElement();
//						for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement())
//						{
//							PlayerScriptPart ps;
//							ps.ValueA = -1;
//							ps.ValueB = -1;
//							ps.ValueC = -1;
//							ps.Sound = -1;
//							ps.SoundNum = -1;
//							ps.Speed = -1;
//							ps.Animation = -1;
//							ps.Flag = true;
//
//							pElem2->QueryValueAttribute("type", &ps.Type);
//							pElem2->QueryValueAttribute("ValueA", &ps.ValueA);
//							pElem2->QueryValueAttribute("ValueB", &ps.ValueB);
//							pElem2->QueryValueAttribute("ValueC", &ps.ValueC);
//							pElem2->QueryValueAttribute("Speed", &ps.Speed);
//							pElem2->QueryValueAttribute("Sound", &ps.Sound);
//							pElem2->QueryValueAttribute("SoundNum", &ps.SoundNum);
//							pElem2->QueryValueAttribute("Animation", &ps.Animation);
//							pElem2->QueryValueAttribute("Flag", &ps.Flag);
//							scripts.push_back(ps);
//						}
//					}
//					act = new ScriptableActor(scripts, autoattach);
//				}
//				break;
//
//				case 11:	//hurt area class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					int lifetaken=5;
//					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
//					pElem->QueryValueAttribute("lifetaken", &lifetaken);
//					act = new HurtArea(zoneSizeX, zoneSizeY, zoneSizeZ, lifetaken);
//				}
//				break;
//
//				case 12:	//NPC class
//				{
//					int npctype=1;
//					float activationdistance=10;//6;
//					pElem->QueryValueAttribute("activationdistance", &activationdistance);
//					pElem->QueryValueAttribute("NPCType", &npctype);
//					const char *namea = pElem->Attribute("Name");
//					std::string NameNPC = "NPC";
//					if(namea)
//						NameNPC = namea;
//
//
//					std::vector<PlayerScriptPart> scripts;
//					TiXmlNode* pNode2=pElem->FirstChild("scripts");
//					if(pNode2)
//					{
//						TiXmlElement*pElem2=pNode2->FirstChildElement();
//						for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement())
//						{
//							PlayerScriptPart ps;
//							ps.ValueA = -1;
//							ps.ValueB = -1;
//							ps.ValueC = -1;
//							ps.Sound = -1;
//							ps.Speed = -1;
//							ps.SoundNum = -1;
//							ps.Animation = -1;
//							ps.Flag = true;
//
//							pElem2->QueryValueAttribute("type", &ps.Type);
//							pElem2->QueryValueAttribute("ValueA", &ps.ValueA);
//							pElem2->QueryValueAttribute("ValueB", &ps.ValueB);
//							pElem2->QueryValueAttribute("ValueC", &ps.ValueC);
//							pElem2->QueryValueAttribute("Speed", &ps.Speed);
//							pElem2->QueryValueAttribute("Sound", &ps.Sound);
//							pElem2->QueryValueAttribute("SoundNum", &ps.SoundNum);
//							pElem2->QueryValueAttribute("Animation", &ps.Animation);
//							pElem2->QueryValueAttribute("Flag", &ps.Flag);
//							scripts.push_back(ps);
//						}
//					}
//
//					DialogHandlerPtr dialogptr = DialogHandlerPtr();
//					#ifndef _LBANET_SERVER_SIDE_
//					dialogptr = LoadDialog(pElem->FirstChildElement("Dialog"), invH, qH);
//					#endif
//
//					NPCActor *tmpact = new NPCActor(scripts, false, npctype, activationdistance, NameNPC, 
//													dialogptr);
//
//					if(npctype == 2)
//					{
//						TiXmlNode* itemGroup=pElem->FirstChild("items");
//						if(itemGroup)
//						{
//							std::map<long, TraderItem> items;
//							TiXmlElement*pElem2=itemGroup->FirstChildElement();
//							for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement())
//							{
//								TraderItem itm;
//								itm.id = -1;
//								pElem2->QueryValueAttribute("id", &itm.id);
//								itm.condition = LoadCondition(pElem2->FirstChildElement("Condition"), invH, qH);
//								 
//								items[itm.id] = itm;
//							}
//
//							tmpact->SetItems(items);
//						}
//					}
//
//					act = tmpact;
//				}
//				break;
//
//
//				case 13: //scripted zone actor class
//				{
//					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
//					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
//					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
//					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
//					int activationtype=1;
//					pElem->QueryValueAttribute("activationtype", &activationtype);
//
//					long neededitem=-1;
//					pElem->QueryValueAttribute("neededitem", &neededitem);
//					bool destroyitem = false;
//					pElem->QueryValueAttribute("destroyitem", &destroyitem);
//
//					const char *abortedmessage = pElem->Attribute("abortedmessage");
//					std::string abortedmessages;
//					if(abortedmessage)
//						abortedmessages = abortedmessage;
//
//
//					std::vector<PlayerScriptPart> scripts;
//					TiXmlNode* pNode2=pElem->FirstChild("scripts");
//					if(pNode2)
//					{
//						TiXmlElement*pElem2=pNode2->FirstChildElement();
//						for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement())
//						{
//							PlayerScriptPart ps;
//							ps.ValueA = -1;
//							ps.ValueB = -1;
//							ps.ValueC = -1;
//							ps.Sound = -1;
//							ps.Speed = -1;
//							ps.SoundNum = -1;
//							ps.Animation = -1;
//							ps.Flag = true;
//							pElem2->QueryValueAttribute("type", &ps.Type);
//							pElem2->QueryValueAttribute("ValueA", &ps.ValueA);
//							pElem2->QueryValueAttribute("ValueB", &ps.ValueB);
//							pElem2->QueryValueAttribute("ValueC", &ps.ValueC);
//							pElem2->QueryValueAttribute("Speed", &ps.Speed);
//							pElem2->QueryValueAttribute("Sound", &ps.Sound);
//							pElem2->QueryValueAttribute("SoundNum", &ps.SoundNum);
//							pElem2->QueryValueAttribute("Animation", &ps.Animation);
//							pElem2->QueryValueAttribute("Flag", &ps.Flag);
//
//							const char *newMap = pElem2->Attribute("newMap");
//							if(newMap)
//								ps.NewMap = newMap;
//
//							const char *spawning = pElem2->Attribute("spawning");
//							if(spawning)
//								ps.Spawning = spawning;
//
//							scripts.push_back(ps);
//						}
//					}
//					act = new ScriptedZoneActor(zoneSizeX, zoneSizeY, zoneSizeZ, scripts, 
//												activationtype, neededitem, destroyitem, abortedmessages);
//				}
//				break;
//			}
//
//			// add common attributes
//			act->SetId(id);
//			act->SetPosition(posX, posY, posZ);
//			act->SetRotation((float)rotation);
//			act->SetSize(sizeX, sizeY, sizeZ);
//			act->SetPassable(passable);
//			act->SetDepthMask(depthmask);
//			act->SetMovable(movable);
//
//			act->SetRenderer(renderer);
//
//			act->SetType(type);
//			act->SetOutputSignal(outputsignal, stargets);
//			act->SetRendererType(renderertype, renderertarget);
//			act->SetAttachedSound(attachedsound);
//			act->SetSignaler(signaler);
//
//			// add it to the vector
//			vec[id] = act;
//		}
//	}
//	return true;
//}
//
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load all sprites info
//--------------------------------------------------------------------------------------------------
//*/
//bool MapInfoXmlReader::LoadSprites(const std::string &Filename, std::map<long, SpriteInfo> &vec)
//{
//	vec.clear();
//
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for( ; pElem; pElem=pElem->NextSiblingElement())
//		{
//			SpriteInfo spi;
//			spi.filename = pElem->Attribute("filename");
//			pElem->QueryValueAttribute("id", &spi.id);
//			TiXmlElement* pElemC = pElem->FirstChildElement();
//			for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//			{
//				QuadImageInfo qi;
//				qi.UseFullImage = true;
//
//				pElemC->QueryFloatAttribute("BottomLeftCornerX", &qi.BottomLeftCornerX);
//				pElemC->QueryFloatAttribute("BottomLeftCornerY", &qi.BottomLeftCornerY);
//				pElemC->QueryFloatAttribute("BottomLeftCornerZ", &qi.BottomLeftCornerZ);
//				pElemC->QueryFloatAttribute("TopRightCornerX", &qi.TopRightCornerX);
//				pElemC->QueryFloatAttribute("TopRightCornerY", &qi.TopRightCornerY);
//				pElemC->QueryFloatAttribute("TopRightCornerZ", &qi.TopRightCornerZ);
//
//				pElemC->QueryFloatAttribute("BottomRightCornerX", &qi.BottomRightCornerX);
//				pElemC->QueryFloatAttribute("BottomRightCornerY", &qi.BottomRightCornerY);
//				pElemC->QueryFloatAttribute("BottomRightCornerZ", &qi.BottomRightCornerZ);
//				pElemC->QueryFloatAttribute("TopLeftCornerX", &qi.TopLeftCornerX);
//				pElemC->QueryFloatAttribute("TopLeftCornerY", &qi.TopLeftCornerY);
//				pElemC->QueryFloatAttribute("TopLeftCornerZ", &qi.TopLeftCornerZ);
//
//
//				if(pElemC->QueryIntAttribute("TopLeftTextcoordX", &qi.TopLeftTextcoordX) == TIXML_SUCCESS)
//					qi.UseFullImage = false;
//
//				pElemC->QueryIntAttribute("TopLeftTextcoordY", &qi.TopLeftTextcoordY);
//				pElemC->QueryIntAttribute("TopRightTextcoordX", &qi.TopRightTextcoordX);
//				pElemC->QueryIntAttribute("TopRightTextcoordY", &qi.TopRightTextcoordY);
//				pElemC->QueryIntAttribute("BottomLeftTextcoordX", &qi.BottomLeftTextcoordX);
//				pElemC->QueryIntAttribute("BottomLeftTextcoordY", &qi.BottomLeftTextcoordY);
//				pElemC->QueryIntAttribute("BottomRightTextcoordX", &qi.BottomRightTextcoordX);
//				pElemC->QueryIntAttribute("BottomRightTextcoordY", &qi.BottomRightTextcoordY);
//
//				spi.quadsInfo.push_back(qi);
//			}
//
//			vec[spi.id] = spi;
//		}
//	}
//
//	return true;
//}
//
///*
//--------------------------------------------------------------------------------------------------
//- get a text from file
//--------------------------------------------------------------------------------------------------
//*/
//std::map<long, std::string> MapInfoXmlReader::LoadTextFile(const std::string &Filename)
//{
//	std::map<long, std::string> res;
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return res;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: text attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return res;
//
//
//		// for each text
//		pElem=pElem->FirstChildElement();
//		for(;pElem; pElem=pElem->NextSiblingElement())
//		{
//			long ctid = -1;
//			pElem->QueryValueAttribute("id", &ctid);
//
//			if(pElem->FirstChild())
//				res[ctid] = pElem->FirstChild()->Value();
//		}
//	}
//
//	return res;
//}
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- get a text from file
//--------------------------------------------------------------------------------------------------
//*/
//void MapInfoXmlReader::GetAllTexts(const std::string &Filename, std::map<long, std::string> &txts)
//{
//	std::string res;
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for(;pElem; pElem=pElem->NextSiblingElement())
//		{
//			long ctid = -1;
//			pElem->QueryValueAttribute("id", &ctid);
//			if(pElem->FirstChild())
//				txts[ctid] = pElem->FirstChild()->Value();
//			else
//				txts[ctid] = "";
//		}
//	}
//}
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- get a sound path from file
//--------------------------------------------------------------------------------------------------
//*/
//std::string MapInfoXmlReader::GetSoundPath(const std::string &Filename, long id)
//{
//	std::string res;
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return res;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return res;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for(;pElem; pElem=pElem->NextSiblingElement())
//		{
//			long ctid = -1;
//			pElem->QueryValueAttribute("id", &ctid);
//			if(ctid == id)
//				return pElem->Attribute("path");
//		}
//	}
//
//	return res;
//}
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load all models info
//--------------------------------------------------------------------------------------------------
//*/
//bool MapInfoXmlReader::LoadModels(const std::string &Filename, std::map<long, ModelInfo> &vec)
//{
//	vec.clear();
//
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for( ; pElem; pElem=pElem->NextSiblingElement())
//		{
//			ModelInfo spi;
//			spi.TransX=0;
//			spi.TransY=0;
//			spi.TransZ=0;
//			spi.RotX=0;
//			spi.RotY=0;
//			spi.RotZ=0;
//
//			spi.filename = pElem->Attribute("filename");
//			pElem->QueryValueAttribute("id", &spi.id);
//
//			pElem->QueryFloatAttribute("scaleX", &spi.ScaleX);
//			pElem->QueryFloatAttribute("scaleY", &spi.ScaleY);
//			pElem->QueryFloatAttribute("scaleZ", &spi.ScaleZ);
//
//			pElem->QueryFloatAttribute("transX", &spi.TransX);
//			pElem->QueryFloatAttribute("transY", &spi.TransY);
//			pElem->QueryFloatAttribute("transZ", &spi.TransZ);
//
//			pElem->QueryFloatAttribute("rotX", &spi.RotX);
//			pElem->QueryFloatAttribute("rotY", &spi.RotY);
//			pElem->QueryFloatAttribute("rotZ", &spi.RotZ);
//
//			vec[spi.id] = spi;
//		}
//	}
//
//	return true;
//}
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load inventory info
//--------------------------------------------------------------------------------------------------
//*/
//bool MapInfoXmlReader::LoadInventory(const std::string &Filename, std::map<long, ItemInfo> &mapinv)
//{
//	mapinv.clear();
//
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for( ; pElem; pElem=pElem->NextSiblingElement())
//		{
//			ItemInfo spi;
//			spi.Ephemere = false;
//			pElem->QueryValueAttribute("id", &spi.id);
//			spi.filename = pElem->Attribute("filename");
//			pElem->QueryValueAttribute("type", &spi.type);
//			pElem->QueryValueAttribute("valueA", &spi.valueA);
//			pElem->QueryValueAttribute("Max", &spi.Max);
//			pElem->QueryValueAttribute("Description", &spi.DescriptionId);
//			pElem->QueryValueAttribute("Effect", &spi.Effect);
//			int tmpv = 0;
//			pElem->QueryValueAttribute("Ephemere", &tmpv);
//			spi.Ephemere = (tmpv == 1);
//			spi.Price = 1;
//			pElem->QueryValueAttribute("Price", &spi.Price);
//
//			const char * datec = pElem->Attribute("Date");
//			if(datec)
//				spi.Date = datec;
//
//			pElem->QueryValueAttribute("From", &spi.FromId);
//			pElem->QueryValueAttribute("Subject", &spi.SubjectId);
//
//			mapinv[spi.id] = spi;
//		}
//	}
//
//	return true;
//}
//
//
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load a condition information into memory
//--------------------------------------------------------------------------------------------------
//*/
//ConditionBasePtr MapInfoXmlReader::LoadCondition(TiXmlElement* pElem, 
//												 InventoryHandlerBase * invH,
//												  QuestHandler * qH)
//{
//	// should always have a valid root but handle gracefully if it does
//	if (!pElem)
//		return ConditionBasePtr();
//
//	// save this for later
//	TiXmlHandle hRoot=TiXmlHandle(pElem);
//
//	int conditiontype = 0;
//	pElem->QueryValueAttribute("type", &conditiontype);
//
//	// depending of the type of condition
//	switch(conditiontype)
//	{
//		case 0: //always true condition
//			return ConditionBasePtr(new AlwaysTrueCondition());
//		break;
//
//		case 1: // negate condition
//		{
//			ConditionBasePtr ptr = LoadCondition(hRoot.FirstChild( "Condition" ).Element(), invH, qH);
//			return ConditionBasePtr(new NegateCondition(ptr));
//		}
//		break;
//
//
//		case 2: // list of conditions
//		{
//			TiXmlElement* pElemC = hRoot.FirstChild( "Condition" ).Element();
//			std::vector<ConditionBasePtr> clist;
//			for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//			{
//				clist.push_back(LoadCondition(pElemC, invH, qH));
//			}
//
//			return ConditionBasePtr(new ConditionList(clist));
//		}
//		break;
//
//
//		case 3: // quest started
//		{
//			long QuestId = -1;
//			pElem->QueryValueAttribute("QuestId", &QuestId);
//
//			return ConditionBasePtr(new QuestStartedCondition(QuestId, qH));
//		}
//		break;
//
//
//		case 4: // quest finished
//		{
//			long QuestId = -1;
//			pElem->QueryValueAttribute("QuestId", &QuestId);
//
//			return ConditionBasePtr(new QuestFinishedCondition(QuestId, qH));
//		}
//		break;
//
//
//		case 5: // quest not started neither finished
//		{
//			long QuestId = -1;
//			pElem->QueryValueAttribute("QuestId", &QuestId);
//
//			return ConditionBasePtr(new QuestNotDoneCondition(QuestId, qH));
//		}
//		break;
//
//
//		case 6: // quest condition passed
//		{
//			long QuestId = -1;
//			pElem->QueryValueAttribute("QuestId", &QuestId);
//
//			return ConditionBasePtr(new QuestConditionsPassedCondition(QuestId, qH));
//		}
//		break;
//
//		case 7: // object in inventory
//		{
//			long ObjectId = -1;
//			pElem->QueryValueAttribute("ObjectId", &ObjectId);
//
//			int ObjectNumber = -1;
//			pElem->QueryValueAttribute("ObjectNumber", &ObjectNumber);
//
//			bool StrictlyEqual = false;
//			pElem->QueryValueAttribute("StrictlyEqual", &StrictlyEqual);
//
//
//			return ConditionBasePtr(new InventoryCondition(ObjectId, ObjectNumber, StrictlyEqual, invH));
//		}
//		break;
//		
//	}
//
//	return ConditionBasePtr();
//}
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load a dialog information into memory
//--------------------------------------------------------------------------------------------------
//*/
//DialogHandlerPtr MapInfoXmlReader::LoadDialog(TiXmlElement* pElem, InventoryHandlerBase * invH,
//												  QuestHandler * qH)
//{
//	// should always have a valid root but handle gracefully if it does
//	if (!pElem)
//		return DialogHandlerPtr();
//
//	// save this for later
//	TiXmlHandle hRoot=TiXmlHandle(pElem);
//
//	long StandardQuitDialog=-1, StandardGoToRootDialog=-1, TradingDialog=-1;
//	pElem->QueryValueAttribute("StandardQuitDialog", &StandardQuitDialog);
//	pElem->QueryValueAttribute("StandardGoToRootDialog", &StandardGoToRootDialog);
//	pElem->QueryValueAttribute("TradingDialog", &TradingDialog);
//
//	DialogEntryPtr entryptr = MapInfoXmlReader::LoadDialogEntry(hRoot.FirstChild( "DialogEntry" ).Element(), invH, qH);
//	DialogTreeRootPtr treeptr = MapInfoXmlReader::LoadTreeRoot(hRoot.FirstChild( "DialogTree" ).Element(), invH, qH);
//
//
//	return DialogHandlerPtr(new	DialogHandler(entryptr, treeptr,
//												StandardQuitDialog, StandardGoToRootDialog,
//												TradingDialog));
//}
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load a dialog entry information into memory
//--------------------------------------------------------------------------------------------------
//*/
//DialogEntryPtr MapInfoXmlReader::LoadDialogEntry(TiXmlElement* pElem, InventoryHandlerBase * invH,
//												  QuestHandler * qH)
//{
//	// should always have a valid root but handle gracefully if it does
//	if (!pElem)
//		return DialogEntryPtr();
//
//	// save this for later
//	TiXmlHandle hRoot=TiXmlHandle(pElem);
//
//	int conditiontype = 0;
//	pElem->QueryValueAttribute("type", &conditiontype);
//
//	// depending of the type of condition
//	switch(conditiontype)
//	{
//		case 0: //normal DialogEntry
//		{
//			long TextId = -1;
//			pElem->QueryValueAttribute("TextId", &TextId);
//			return DialogEntryPtr(new DialogEntry(TextId));
//		}
//		break;
//
//		case 1: // ConditionnalDialogEntry
//		{
//			long TextId = -1;
//			pElem->QueryValueAttribute("TextId", &TextId);
//			ConditionBasePtr ptr = LoadCondition(hRoot.FirstChild( "Condition" ).Element(), invH, qH);
//			return DialogEntryPtr(new ConditionnalDialogEntry(TextId, ptr));
//		}
//		break;
//
//
//		case 2: // RandomDialogEntry
//		{
//			TiXmlElement* pElemC = hRoot.FirstChild( "DialogEntry" ).Element();
//			std::vector<DialogEntryPtr> clist;
//			for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//			{
//				clist.push_back(LoadDialogEntry(pElemC, invH, qH));
//			}
//
//			return DialogEntryPtr(new RandomDialogEntry(clist));
//		}
//		break;
//
//
//		case 3: // DialogEntryList
//		{
//			TiXmlElement* pElemC = hRoot.FirstChild( "DialogEntry" ).Element();
//			std::vector<DialogEntryPtr> clist;
//			for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//			{
//				clist.push_back(LoadDialogEntry(pElemC, invH, qH));
//			}
//
//			return DialogEntryPtr(new DialogEntryList(clist));
//		}
//		break;
//	}
//
//	return DialogEntryPtr();
//}
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load a dialog player choice information into memory
//--------------------------------------------------------------------------------------------------
//*/
//DialogTreePlayerChoicePtr MapInfoXmlReader::LoadPlayerChoice(TiXmlElement* pElem, InventoryHandlerBase * invH,
//																QuestHandler * qH)
//{
//	// should always have a valid root but handle gracefully if it does
//	if (!pElem)
//		return DialogTreePlayerChoicePtr();
//
//	// save this for later
//	TiXmlHandle hRoot=TiXmlHandle(pElem);
//
//	int conditiontype = 0;
//	pElem->QueryValueAttribute("type", &conditiontype);
//
//	long TextId = -1;
//	pElem->QueryValueAttribute("TextId", &TextId);
//
//
//	// depending of the type of condition
//	switch(conditiontype)
//	{
//		case 0: //DialogTreePlayerChoice
//		{
//			DialogTreeRootPtr ptr = LoadTreeRoot(hRoot.FirstChild( "DialogTree" ).Element(), invH, qH);
//			return DialogTreePlayerChoicePtr(new DialogTreePlayerChoice(TextId, ConditionBasePtr(), ptr));
//		}
//		break;
//
//		case 1: // DialogTreePlayerChoice with validation condition
//		{
//			ConditionBasePtr condptr = LoadCondition(hRoot.FirstChild( "Condition" ).Element(), invH, qH);
//			DialogTreeRootPtr ptr = LoadTreeRoot(hRoot.FirstChild( "DialogTree" ).Element(), invH, qH);
//			return DialogTreePlayerChoicePtr(new DialogTreePlayerChoice(TextId, condptr, ptr));
//		}
//		break;
//
//
//		case 2: // ConditionalDialogTreePlayerChoice
//		{
//			TiXmlElement* treeelem = hRoot.FirstChild( "DialogTree" ).Element();
//			DialogTreeRootPtr ptr = LoadTreeRoot(treeelem, invH, qH);
//			DialogTreeRootPtr ptr2 = LoadTreeRoot(treeelem->NextSiblingElement("DialogTree"), invH, qH);
//			ConditionBasePtr condptr = LoadCondition(hRoot.FirstChild( "Condition" ).Element(), invH, qH);
//			return DialogTreePlayerChoicePtr(new 
//				ConditionalDialogTreePlayerChoice(TextId, ConditionBasePtr(), ptr, ptr2, condptr));
//		}
//		break;
//
//
//		case 3: // ConditionalDialogTreePlayerChoice with validation condition
//		{
//			TiXmlElement* treeelem = hRoot.FirstChild( "DialogTree" ).Element();
//			DialogTreeRootPtr ptr = LoadTreeRoot(treeelem, invH, qH);
//			DialogTreeRootPtr ptr2 = LoadTreeRoot(treeelem->NextSiblingElement("DialogTree"), invH, qH);
//
//			TiXmlElement* condelem = hRoot.FirstChild( "Condition" ).Element();
//			ConditionBasePtr condptr = LoadCondition(condelem, invH, qH);
//			ConditionBasePtr condptr2 = LoadCondition(condelem->NextSiblingElement("Condition"), invH, qH);
//
//			return DialogTreePlayerChoicePtr(new 
//				ConditionalDialogTreePlayerChoice(TextId, condptr, ptr, ptr2, condptr2));
//		}
//		break;
//	}
//
//	return DialogTreePlayerChoicePtr();
//}
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load a dialog tree root information into memory
//--------------------------------------------------------------------------------------------------
//*/
//DialogTreeRootPtr MapInfoXmlReader::LoadTreeRoot(TiXmlElement* pElem, InventoryHandlerBase * invH,
//												  QuestHandler * qH)
//{
//	// should always have a valid root but handle gracefully if it does
//	if (!pElem)
//		return DialogTreeRootPtr();
//
//	// save this for later
//	TiXmlHandle hRoot=TiXmlHandle(pElem);
//
//	long TextId = -1;
//	pElem->QueryValueAttribute("TextId", &TextId);
//
//	bool QuitDialogEnabled = true;
//	pElem->QueryValueAttribute("QuitDialogEnabled", &QuitDialogEnabled);
//
//	bool GoToRootEnabled = true;
//	pElem->QueryValueAttribute("GoToRootEnabled", &GoToRootEnabled);
//
//	long CustomQuitDialog = -1;
//	pElem->QueryValueAttribute("CustomQuitDialog", &CustomQuitDialog);
//
//	long CustomGoToRootDialog = -1;
//	pElem->QueryValueAttribute("CustomGoToRootDialog", &CustomGoToRootDialog);
//
//
//	TiXmlElement* pElemC = hRoot.FirstChild( "PlayerChoice" ).Element();
//	std::vector<DialogTreePlayerChoicePtr> clist;
//	for( ; pElemC; pElemC=pElemC->NextSiblingElement("PlayerChoice"))
//	{
//		clist.push_back(LoadPlayerChoice(pElemC, invH, qH));
//	}
//
//	pElemC = hRoot.FirstChild( "QuestToStart" ).Element();
//	std::vector<long> QuestsToStart;
//	for( ; pElemC; pElemC=pElemC->NextSiblingElement("QuestToStart"))
//	{
//		long qid = -1;
//		pElemC->QueryValueAttribute("Id", &qid);
//		if(qid >= 0)
//			QuestsToStart.push_back(qid);
//	}
//
//	pElemC = hRoot.FirstChild( "QuestToTrigger" ).Element();
//	std::vector<long> QuestsToTrigger;
//	for( ; pElemC; pElemC=pElemC->NextSiblingElement("QuestToTrigger"))
//	{
//		long qid = -1;
//		pElemC->QueryValueAttribute("Id", &qid);
//		if(qid >= 0)
//			QuestsToTrigger.push_back(qid);
//	}
//
//	return DialogTreeRootPtr(new DialogTreeRoot(TextId, clist,
//												QuestsToStart,
//												QuestsToTrigger,
//												QuitDialogEnabled, GoToRootEnabled,
//												CustomQuitDialog, CustomGoToRootDialog));
//
//}
//
//
//
//
///*
//--------------------------------------------------------------------------------------------------
//- load quest info
//--------------------------------------------------------------------------------------------------
//*/
//bool MapInfoXmlReader::LoadQuests(const std::string &Filename, std::map<long, QuestPtr> &quests,
//									InventoryHandlerBase * invH, QuestHandler * qH)
//{
//	quests.clear();
//
//
//	TiXmlDocument doc(Filename);
//	if (!doc.LoadFile())
//		return false;
//
//	TiXmlHandle hDoc(&doc);
//	TiXmlElement* pElem;
//
//	// block: actors attributes
//	{
//		pElem=hDoc.FirstChildElement().Element();
//
//		// should always have a valid root but handle gracefully if it does
//		if (!pElem)
//			return false;
//
//
//		// for each actors
//		pElem=pElem->FirstChildElement();
//		for( ; pElem; pElem=pElem->NextSiblingElement())
//		{
//			long QuestId, TitleTextId=-1, DescriptionTextId=-1, QuestAreaTextId=-1;
//			bool Visible = true;
//
//			pElem->QueryValueAttribute("Id", &QuestId);
//			pElem->QueryValueAttribute("TitleTextId", &TitleTextId);
//			pElem->QueryValueAttribute("DescriptionTextId", &DescriptionTextId);
//			pElem->QueryValueAttribute("Visible", &Visible);
//			pElem->QueryValueAttribute("QuestAreaTextId", &QuestAreaTextId);
//
//			std::vector<long> QuestsStartingAtStart;
//			std::vector<long> QuestsStartingAtEnd;
//			std::vector<long> QuestsTriggeredAtEnd;
//
//			TiXmlElement* pElemC = pElem->FirstChildElement( "QuestsStartingAtStart" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					long qid = -1;
//					pElemC->QueryValueAttribute("Id", &qid);
//					if(qid >= 0)
//						QuestsStartingAtStart.push_back(qid);
//				}
//			}
//
//			pElemC = pElem->FirstChildElement( "QuestsStartingAtEnd" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					long qid = -1;
//					pElemC->QueryValueAttribute("Id", &qid);
//					if(qid >= 0)
//						QuestsStartingAtEnd.push_back(qid);
//				}
//			}
//
//			pElemC = pElem->FirstChildElement( "QuestsTriggeredAtEnd" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					long qid = -1;
//					pElemC->QueryValueAttribute("Id", &qid);
//					if(qid >= 0)
//						QuestsTriggeredAtEnd.push_back(qid);
//				}
//			}
//
//
//			std::vector<std::pair<long, int> > ObjectsGivenAtEnd;
//			std::vector<std::pair<long, int> > ObjectsTakenAtEnd;
//
//			pElemC = pElem->FirstChildElement( "ObjectsGivenAtEnd" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					long qid = -1;
//					int number = 0;
//					pElemC->QueryValueAttribute("Id", &qid);
//					pElemC->QueryValueAttribute("Number", &number);
//
//					if(qid >= 0)
//						ObjectsGivenAtEnd.push_back(std::make_pair<long, int>(qid, number));
//				}
//			}
//
//			pElemC = pElem->FirstChildElement( "ObjectsTakenAtEnd" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					long qid = -1;
//					int number = 0;
//					pElemC->QueryValueAttribute("Id", &qid);
//					pElemC->QueryValueAttribute("Number", &number);
//
//					if(qid >= 0)
//						ObjectsTakenAtEnd.push_back(std::make_pair<long, int>(qid, number));
//				}
//			}
//
//
//			std::vector<ConditionBasePtr> ConditionsToSucceed;
//
//			pElemC = pElem->FirstChildElement( "Conditions" );
//			if(pElemC)
//			{
//				pElemC = pElemC->FirstChildElement();
//				for( ; pElemC; pElemC=pElemC->NextSiblingElement())
//				{
//					ConditionBasePtr ptr = LoadCondition(pElemC, invH, qH);
//					if(ptr)
//						ConditionsToSucceed.push_back(ptr);
//				}
//			}
//
//
//			quests[QuestId] = QuestPtr(new Quest(QuestId, TitleTextId, DescriptionTextId, QuestAreaTextId,
//													ConditionsToSucceed,
//													QuestsStartingAtStart,
//													QuestsStartingAtEnd,
//													QuestsTriggeredAtEnd,
//													ObjectsGivenAtEnd,
//													ObjectsTakenAtEnd,
//													Visible));
//
//		}
//	}
//
//	return true;
//}