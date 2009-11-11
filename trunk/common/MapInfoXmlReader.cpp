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

#include "Actor.h"
#include "TextActor.h"
#include "LadderActor.h"
#include "ExitActor.h"
#include "DoorActor.h"
#include "ContainerActor.h"
#include "UpExitActor.h"
#include "SwitchActor.h"
#include "FloorSwitch.h"
#include "AreaSwitch.h"
#include "LiftActor.h"
#include "GameEvents.h"
#include "SignalerBase.h"
#include "HurtArea.h"

#ifndef _LBANET_SERVER_SIDE_
#include "3DObjectRenderer.h"
#include "SpriteRenderer.h"
#include "AviVideoRenderer.h"
#include "ms3d.h"
#endif


// string helper function
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

// string helper function
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

// get world description
void MapInfoXmlReader::GetWorldDescription(const std::string &Filename,
									std::string &WorldName, std::string &WorldDesc)
{
	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	// block: world attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return;

		WorldName = pElem->Attribute("name");

		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

	// block: description
	{
		pElem=hRoot.FirstChild( "description" ).Element();
		if(pElem)
			WorldDesc=pElem->GetText();
	}
}

// load a world information into memory
bool MapInfoXmlReader::LoadWorld(const std::string &Filename, WorldInfo & res)
{
	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;



	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	// block: world attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return false;

		res.Name = pElem->Attribute("name");
		res.FirstMap = pElem->Attribute("firstmap");
		res.FirstSpawning = pElem->Attribute("firstsparea");

		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

	// block: description
	{
		pElem=hRoot.FirstChild( "description" ).Element();
		if(pElem)
			res.Description=pElem->GetText();
	}

	// block: teleport
	{
		pElem=hRoot.FirstChild( "teleports" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			TPInfo tp;
			tp.Name=pElem->Attribute("name");
			tp.NewMap=pElem->Attribute("map");
			tp.Spawning=pElem->Attribute("sparea");

			res.Teleports[tp.Name] = tp;
		}
	}

	// block: files
	{
		pElem=hRoot.FirstChild( "files" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			res.Files[pElem->Attribute("name")] = pElem->Attribute("path");
		}
	}

	// block: map
	{
		pElem=hRoot.FirstChild( "maps" ).FirstChild("Map").Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			MapInfo mi = LoadMap(pElem);
			res.Maps[mi.Name] = mi;
		}
	}

	return true;
}

/*
--------------------------------------------------------------------------------------------------
- load a map into memory
--------------------------------------------------------------------------------------------------
*/
MapInfo MapInfoXmlReader::LoadMap(TiXmlElement* pElem)
{
	MapInfo res;
	res.MusicLoop = 0;
	TiXmlHandle hRoot(0);

	// block: map attributes
	{
		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return res;

		res.Name = pElem->Attribute("name");
		res.Type = pElem->Attribute("type");

		res.Music = pElem->Attribute("music");
		int testr = pElem->QueryIntAttribute("repeatmusic", &res.MusicLoop);
		if(testr != TIXML_SUCCESS)
			res.MusicLoop = 0;


		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

	// block: description
	{
		pElem=hRoot.FirstChild( "description" ).Element();
		if(pElem)
			res.Description=pElem->GetText();
	}


	// block: files
	{
		pElem=hRoot.FirstChild( "files" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			res.Files[pElem->Attribute("name")] = pElem->Attribute("path");
		}
	}

	// block: lights
	{
		pElem=hRoot.FirstChild( "lights" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			LighInfo l;
			l.Name=pElem->Attribute("name");
			l.Type=pElem->Attribute("type");

			pElem->QueryIntAttribute("posX", &l.PosX);
			pElem->QueryIntAttribute("posY", &l.PosY);
			pElem->QueryIntAttribute("posZ", &l.PosZ);
			pElem->QueryIntAttribute("dirX", &l.DirX);
			pElem->QueryIntAttribute("dirY", &l.DirY);
			pElem->QueryIntAttribute("dirZ", &l.DirZ);

			res.Lights[l.Name] = l;
		}
	}

	// block: spawning areas
	{
		pElem=hRoot.FirstChild( "spareas" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			SpawningInfo sp;
			sp.Rotation = 0;
			sp.Name=pElem->Attribute("name");

			pElem->QueryFloatAttribute("posX", &sp.PosX);
			pElem->QueryFloatAttribute("posY", &sp.PosY);
			pElem->QueryFloatAttribute("posZ", &sp.PosZ);
			pElem->QueryIntAttribute("RotationAtArrival", &sp.Rotation);
			if(sp.Rotation < 0)
				sp.Rotation = 0;
			if(sp.Rotation > 360)
				sp.Rotation = 0;

			res.Spawnings[sp.Name] = sp;
		}
	}

	// block: exit areas
	{
		pElem=hRoot.FirstChild( "exits" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			ExitInfo ex;
			ex.Name=pElem->Attribute("name");
			ex.NewMap=pElem->Attribute("newMap");
			ex.Spawning=pElem->Attribute("spawning");

			pElem->QueryFloatAttribute("TopRightX", &ex.TopRightX);
			pElem->QueryFloatAttribute("TopRightY", &ex.TopRightY);
			pElem->QueryFloatAttribute("TopRightZ", &ex.TopRightZ);

			pElem->QueryFloatAttribute("BottomLeftX", &ex.BottomLeftX);
			pElem->QueryFloatAttribute("BottomLeftY", &ex.BottomLeftY);
			pElem->QueryFloatAttribute("BottomLeftZ", &ex.BottomLeftZ);

			res.Exits[ex.Name] = ex;
		}
	}

	return res;
}


/*
--------------------------------------------------------------------------------------------------
- load map actors into memory
--------------------------------------------------------------------------------------------------
*/
bool MapInfoXmlReader::LoadActors(const std::string &Filename, std::map<long, SpriteInfo> &spinfos,
									std::map<long, SpriteInfo> &vidinfos,
									std::map<long, ModelInfo> &modelinfos,
									std::map<long, Actor *> & vec,
									SignalerBase * signaler)
{
	vec.clear();

	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;



	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return false;


		// for each actors
		pElem=pElem->FirstChildElement();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			long id=0, type=0;
			float posX=0, posY=0, posZ=0;
			float sizeX=0, sizeY=0, sizeZ=0;
			int rotation=0;
			bool passable = true;
			bool depthmask = true;
			bool movable = false;
			long renderertype=-1;
			std::vector<long> renderertarget;
			D3ObjectRenderer * renderer = NULL;
			long outputsignal=-1;
			long attachedsound=-1;
			std::vector<long> stargets;
			const char * targetsstring = pElem->Attribute("signaltargets");
			if(targetsstring)
			{
				std::vector<std::string> tokens;
				StringTokenize(targetsstring, tokens, ",");
				for(size_t i=0; i<tokens.size(); ++i)
					stargets.push_back(atol(tokens[i].c_str()));
			}

			const char * rendertargetsstring = pElem->Attribute("renderertarget");
			if(rendertargetsstring)
			{
				std::vector<std::string> tokens;
				StringTokenize(rendertargetsstring, tokens, ",");
				for(size_t i=0; i<tokens.size(); ++i)
					renderertarget.push_back(atol(tokens[i].c_str()));
			}



			pElem->QueryValueAttribute("id", &id);
			pElem->QueryValueAttribute("type", &type);
			pElem->QueryFloatAttribute("posX", &posX);
			pElem->QueryFloatAttribute("posY", &posY);
			pElem->QueryFloatAttribute("posZ", &posZ);
			pElem->QueryFloatAttribute("sizeX", &sizeX);
			pElem->QueryFloatAttribute("sizeY", &sizeY);
			pElem->QueryFloatAttribute("sizeZ", &sizeZ);
			pElem->QueryIntAttribute("rotation", &rotation);
			pElem->QueryValueAttribute("passable", &passable);
			pElem->QueryValueAttribute("depthmask", &depthmask);
			pElem->QueryValueAttribute("movable", &movable);
			pElem->QueryValueAttribute("outputsignal", &outputsignal);
			pElem->QueryValueAttribute("attachedsound", &attachedsound);

#ifndef _LBANET_SERVER_SIDE_
			if(pElem->QueryValueAttribute("renderertype", &renderertype) == TIXML_SUCCESS)
				if(renderertarget.size() > 0)
				{
					switch(renderertype)
					{
						case 0: // sprite renderer
							{
								std::vector<SpriteInfo *> vectmp;
								for(size_t itar = 0; itar<renderertarget.size(); ++itar)
								{
									std::map<long, SpriteInfo>::iterator itsp = spinfos.find(renderertarget[itar]);
									if(itsp != spinfos.end())
										vectmp.push_back(&itsp->second);
								}

								if(vectmp.size() > 0)
								{
									SpriteRenderer * tmp = new SpriteRenderer();
									tmp->SetSprites(vectmp);
									renderer = tmp;
								}
							}
						break;
						case 1: // video renderer
							{
								std::map<long, SpriteInfo>::iterator itsp = vidinfos.find(renderertarget[0]);
								if(itsp != vidinfos.end())
								{
									std::vector<SpriteInfo *> vectmp;
									vectmp.push_back(&itsp->second);
									AviVideoRenderer * tmp = new AviVideoRenderer();
									tmp->SetSprites(vectmp);
									renderer = tmp;
								}
							}
						break;
						case 2: // model renderer
							{
								std::map<long, ModelInfo>::iterator itsp = modelinfos.find(renderertarget[0]);
								if(itsp != modelinfos.end())
								{
									MS3DModel * tmp = new MS3DModel();
									tmp->loadModelData( itsp->second.filename );
									tmp->SetScale(itsp->second.ScaleX, itsp->second.ScaleY, itsp->second.ScaleZ);
									renderer = tmp;
								}
							}
						break;
					}
				}
#endif

			Actor * act;
			switch(type)
			{
				case 0:	//basic actor class
					act = new Actor();
				break;
				case 1:	//text actor class
				{
					long textid=0;
					float activationdistance;
					pElem->QueryValueAttribute("activationdistance", &activationdistance);
					pElem->QueryValueAttribute("textid", &textid);
					act = new TextActor(activationdistance, textid);
				}
				break;
				case 2:	//ladder actor class
				{
					float deltaX=0, deltaY=0, deltaZ=0;
					int direction=0;
					float activationdistance;
					pElem->QueryValueAttribute("activationdistance", &activationdistance);
					pElem->QueryValueAttribute("deltaX", &deltaX);
					pElem->QueryValueAttribute("deltaY", &deltaY);
					pElem->QueryValueAttribute("deltaZ", &deltaZ);
					pElem->QueryValueAttribute("direction", &direction);
					act = new LadderActor(activationdistance, deltaX, deltaY, deltaZ, direction);
				}
				break;
				case 3:	//exit actor class
				{
					float deltaX=0, deltaY=0, deltaZ=0;
					int direction=0;
					float activationdistance;
					pElem->QueryValueAttribute("activationdistance", &activationdistance);
					pElem->QueryValueAttribute("deltaX", &deltaX);
					pElem->QueryValueAttribute("deltaY", &deltaY);
					pElem->QueryValueAttribute("deltaZ", &deltaZ);
					pElem->QueryValueAttribute("direction", &direction);
					act = new ExitActor(activationdistance, deltaX, deltaY, deltaZ, direction);
				}
				break;

				case 4:	//door actor class
				{
					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
					bool locked = false;
					bool hide = false;
					long keyid=0;
					float OpenTransX=0;
					float OpenTransY=0;
					float OpenTransZ=0;
					float OpenTransSpeedX=0;
					float OpenTransSpeedY=0;
					float OpenTransSpeedZ=0;
					bool destroykey = false;

					pElem->QueryValueAttribute("zoneSizeX", &zoneSizeX);
					pElem->QueryValueAttribute("zoneSizeY", &zoneSizeY);
					pElem->QueryValueAttribute("zoneSizeZ", &zoneSizeZ);
					pElem->QueryValueAttribute("locked", &locked);
					pElem->QueryValueAttribute("keyid", &keyid);
					pElem->QueryValueAttribute("hide", &hide);
					pElem->QueryValueAttribute("destroykey", &destroykey);

					pElem->QueryValueAttribute("OpenTransX", &OpenTransX);
					pElem->QueryValueAttribute("OpenTransY", &OpenTransY);
					pElem->QueryValueAttribute("OpenTransZ", &OpenTransZ);
					pElem->QueryValueAttribute("OpenTransSpeedX", &OpenTransSpeedX);
					pElem->QueryValueAttribute("OpenTransSpeedY", &OpenTransSpeedY);
					pElem->QueryValueAttribute("OpenTransSpeedZ", &OpenTransSpeedZ);



					act = new DoorActor(zoneSizeX, zoneSizeY, zoneSizeZ, locked, keyid, hide,
											OpenTransX, OpenTransY, OpenTransZ,
											OpenTransSpeedX, OpenTransSpeedY, OpenTransSpeedZ, destroykey);
					passable = false;
				}
				break;

				case 5:	//container actor class
				{
					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
					ContainerActor *tmpC = new ContainerActor(zoneSizeX, zoneSizeY, zoneSizeZ);

					// get the contained items
					std::vector<ItemGroup> items;
					TiXmlElement* pElemC=pElem->FirstChildElement();
					for( pElemC; pElemC; pElemC=pElemC->NextSiblingElement())
					{
						// for each group of items
						ItemGroup newGroup;
						newGroup.lastSpawningTime = 0;
						newGroup.RespawningTime = 300000; // default = 5min
						newGroup.currpicked = -1;
						pElemC->QueryValueAttribute("respawnTimeInMs", &newGroup.RespawningTime);

						TiXmlElement* itemGroup=pElemC->FirstChildElement();
						for( itemGroup; itemGroup; itemGroup=itemGroup->NextSiblingElement())
						{
							ItemGroupElement elem;
							itemGroup->QueryValueAttribute("id", &elem.id);
							itemGroup->QueryValueAttribute("number", &elem.number);
							itemGroup->QueryValueAttribute("probability", &elem.probability);
							newGroup.groupelements.push_back(elem);
						}
						if(newGroup.groupelements.size() > 0)
							items.push_back(newGroup);
					}
					tmpC->SetLootList(items);

					act = tmpC;
				}
				break;

				case 6:	//up exit actor class
				{
					int direction=0;
					float activationdistance;
					pElem->QueryValueAttribute("activationdistance", &activationdistance);
					pElem->QueryValueAttribute("direction", &direction);
					act = new UpExitActor(activationdistance, direction);
				}
				break;

				case 7:	//switch actor class
				{
					float activationdistance;
					pElem->QueryValueAttribute("activationdistance", &activationdistance);
					act = new SwitchActor(activationdistance);
				}
				break;

				case 8:	//area switch actor class
				{
					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
					act = new AreaSwitch(zoneSizeX, zoneSizeY, zoneSizeZ);
				}
				break;

				case 9:	//floor switch actor class
				{
					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
					act = new FloorSwitch(zoneSizeX, zoneSizeY, zoneSizeZ);
				}
				break;

				case 10:	//lift actor class
				{
					std::vector<PlayerScriptPart> scripts;
					TiXmlElement* pElem2=pElem->FirstChild("scripts")->FirstChildElement();
					for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement())
					{
						PlayerScriptPart ps;
						ps.ValueA = -1;
						ps.ValueB = -1;
						ps.ValueC = -1;
						ps.Sound = -1;
						ps.Speed = -1;
						pElem2->QueryValueAttribute("type", &ps.Type);
						pElem2->QueryValueAttribute("ValueA", &ps.ValueA);
						pElem2->QueryValueAttribute("ValueB", &ps.ValueB);
						pElem2->QueryValueAttribute("ValueC", &ps.ValueC);
						pElem2->QueryValueAttribute("Speed", &ps.Speed);
						pElem2->QueryValueAttribute("Sound", &ps.Sound);
						scripts.push_back(ps);
					}
					act = new LiftActor(scripts);
				}
				break;

				case 11:	//hurt area class
				{
					float zoneSizeX=0, zoneSizeY=0, zoneSizeZ=0;
					int lifetaken=5;
					pElem->QueryValueAttribute("zonesizeX", &zoneSizeX);
					pElem->QueryValueAttribute("zonesizeY", &zoneSizeY);
					pElem->QueryValueAttribute("zonesizeZ", &zoneSizeZ);
					pElem->QueryValueAttribute("lifetaken", &lifetaken);
					act = new HurtArea(zoneSizeX, zoneSizeY, zoneSizeZ, lifetaken);
				}
				break;
			}

			// add common attributes
			act->SetId(id);
			act->SetPosition(posX, posY, posZ);
			act->SetRotation((float)rotation);
			act->SetSize(sizeX, sizeY, sizeZ);
			act->SetPassable(passable);
			act->SetDepthMask(depthmask);
			act->SetMovable(movable);
			act->SetRenderer(renderer);
			act->SetType(type);
			act->SetOutputSignal(outputsignal, stargets);
			act->SetRendererType(renderertype, renderertarget);
			act->SetAttachedSound(attachedsound);
			act->SetSignaler(signaler);

			// add it to the vector
			vec[id] = act;
		}
	}
	return true;
}




/*
--------------------------------------------------------------------------------------------------
- load all sprites info
--------------------------------------------------------------------------------------------------
*/
bool MapInfoXmlReader::LoadSprites(const std::string &Filename, std::map<long, SpriteInfo> &vec)
{
	vec.clear();


	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return false;


		// for each actors
		pElem=pElem->FirstChildElement();
		for( ; pElem; pElem=pElem->NextSiblingElement())
		{
			SpriteInfo spi;
			spi.filename = pElem->Attribute("filename");
			pElem->QueryValueAttribute("id", &spi.id);
			TiXmlElement* pElemC = pElem->FirstChildElement();
			for( ; pElemC; pElemC=pElemC->NextSiblingElement())
			{
				QuadImageInfo qi;
				qi.UseFullImage = true;

				pElemC->QueryFloatAttribute("BottomLeftCornerX", &qi.BottomLeftCornerX);
				pElemC->QueryFloatAttribute("BottomLeftCornerY", &qi.BottomLeftCornerY);
				pElemC->QueryFloatAttribute("BottomLeftCornerZ", &qi.BottomLeftCornerZ);
				pElemC->QueryFloatAttribute("TopRightCornerX", &qi.TopRightCornerX);
				pElemC->QueryFloatAttribute("TopRightCornerY", &qi.TopRightCornerY);
				pElemC->QueryFloatAttribute("TopRightCornerZ", &qi.TopRightCornerZ);

				pElemC->QueryFloatAttribute("BottomRightCornerX", &qi.BottomRightCornerX);
				pElemC->QueryFloatAttribute("BottomRightCornerY", &qi.BottomRightCornerY);
				pElemC->QueryFloatAttribute("BottomRightCornerZ", &qi.BottomRightCornerZ);
				pElemC->QueryFloatAttribute("TopLeftCornerX", &qi.TopLeftCornerX);
				pElemC->QueryFloatAttribute("TopLeftCornerY", &qi.TopLeftCornerY);
				pElemC->QueryFloatAttribute("TopLeftCornerZ", &qi.TopLeftCornerZ);


				if(pElemC->QueryIntAttribute("TopLeftTextcoordX", &qi.TopLeftTextcoordX) == TIXML_SUCCESS)
					qi.UseFullImage = false;

				pElemC->QueryIntAttribute("TopLeftTextcoordY", &qi.TopLeftTextcoordY);
				pElemC->QueryIntAttribute("TopRightTextcoordX", &qi.TopRightTextcoordX);
				pElemC->QueryIntAttribute("TopRightTextcoordY", &qi.TopRightTextcoordY);
				pElemC->QueryIntAttribute("BottomLeftTextcoordX", &qi.BottomLeftTextcoordX);
				pElemC->QueryIntAttribute("BottomLeftTextcoordY", &qi.BottomLeftTextcoordY);
				pElemC->QueryIntAttribute("BottomRightTextcoordX", &qi.BottomRightTextcoordX);
				pElemC->QueryIntAttribute("BottomRightTextcoordY", &qi.BottomRightTextcoordY);

				spi.quadsInfo.push_back(qi);
			}

			vec[spi.id] = spi;
		}
	}

	return true;
}

/*
--------------------------------------------------------------------------------------------------
- get a text from file
--------------------------------------------------------------------------------------------------
*/
std::string MapInfoXmlReader::GetText(const std::string &Filename, long textid)
{
	std::string res;

	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return res;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return res;


		// for each actors
		pElem=pElem->FirstChildElement();
		for(;pElem; pElem=pElem->NextSiblingElement())
		{
			long ctid = -1;
			pElem->QueryValueAttribute("id", &ctid);
			if(ctid == textid)
			{
				if(pElem->FirstChild())
					return pElem->FirstChild()->Value();
				else
					return "";
			}
		}
	}

	return res;
}



/*
--------------------------------------------------------------------------------------------------
- get a text from file
--------------------------------------------------------------------------------------------------
*/
void MapInfoXmlReader::GetAllTexts(const std::string &Filename, std::map<long, std::string> &txts)
{
	std::string res;

	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return;


		// for each actors
		pElem=pElem->FirstChildElement();
		for(;pElem; pElem=pElem->NextSiblingElement())
		{
			long ctid = -1;
			pElem->QueryValueAttribute("id", &ctid);
			if(pElem->FirstChild())
				txts[ctid] = pElem->FirstChild()->Value();
			else
				txts[ctid] = "";
		}
	}
}



/*
--------------------------------------------------------------------------------------------------
- get a sound path from file
--------------------------------------------------------------------------------------------------
*/
std::string MapInfoXmlReader::GetSoundPath(const std::string &Filename, long id)
{
	std::string res;

	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return res;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return res;


		// for each actors
		pElem=pElem->FirstChildElement();
		for(;pElem; pElem=pElem->NextSiblingElement())
		{
			long ctid = -1;
			pElem->QueryValueAttribute("id", &ctid);
			if(ctid == id)
				return pElem->Attribute("path");
		}
	}

	return res;
}


/*
--------------------------------------------------------------------------------------------------
- load all models info
--------------------------------------------------------------------------------------------------
*/
bool MapInfoXmlReader::LoadModels(const std::string &Filename, std::map<long, ModelInfo> &vec)
{
	vec.clear();


	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return false;


		// for each actors
		pElem=pElem->FirstChildElement();
		for( ; pElem; pElem=pElem->NextSiblingElement())
		{
			ModelInfo spi;
			spi.filename = pElem->Attribute("filename");
			pElem->QueryValueAttribute("id", &spi.id);
			pElem->QueryFloatAttribute("scaleX", &spi.ScaleX);
			pElem->QueryFloatAttribute("scaleY", &spi.ScaleY);
			pElem->QueryFloatAttribute("scaleZ", &spi.ScaleZ);

			vec[spi.id] = spi;
		}
	}

	return true;
}



/*
--------------------------------------------------------------------------------------------------
- load inventory info
--------------------------------------------------------------------------------------------------
*/
bool MapInfoXmlReader::LoadInventory(const std::string &Filename, std::map<long, ItemInfo> &mapinv)
{
	mapinv.clear();


	TiXmlDocument doc(Filename);
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;

	// block: actors attributes
	{
		pElem=hDoc.FirstChildElement().Element();

		// should always have a valid root but handle gracefully if it does
		if (!pElem)
			return false;


		// for each actors
		pElem=pElem->FirstChildElement();
		for( ; pElem; pElem=pElem->NextSiblingElement())
		{
			ItemInfo spi;
			spi.Ephemere = false;
			pElem->QueryValueAttribute("id", &spi.id);
			spi.filename = pElem->Attribute("filename");
			pElem->QueryValueAttribute("type", &spi.type);
			pElem->QueryValueAttribute("valueA", &spi.valueA);
			pElem->QueryValueAttribute("Max", &spi.Max);
			spi.Description = pElem->Attribute("Description");
			pElem->QueryValueAttribute("Effect", &spi.Effect);
			pElem->QueryValueAttribute("Ephemere", &spi.Ephemere);
			mapinv[spi.id] = spi;
		}
	}

	return true;
}