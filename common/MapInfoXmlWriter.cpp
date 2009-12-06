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


#include "MapInfoXmlWriter.h"
#include "tinyxml.h"
#include "TextActor.h"
#include "LadderActor.h"
#include "ExitActor.h"
#include "DoorActor.h"
#include "ContainerActor.h"
#include "UpExitActor.h"
#include "SwitchActor.h"
#include "FloorSwitch.h"
#include "AreaSwitch.h"
#include "GameEvents.h"
#include "ScriptableActor.h"
#include "HurtArea.h"
#include "NPCActor.h"
#include "ScriptedZoneActor.h"

#include <fstream>


// save world information into file
void MapInfoXmlWriter::SaveWorld(const std::string Filename, const WorldInfo & wi)
{
	TiXmlDocument doc;
	TiXmlComment * comment;
	std::string s;

 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement("World");
	doc.LinkEndChild( root );
	root->SetAttribute("name", wi.Name);
	root->SetAttribute("firstmap", wi.FirstMap);
	root->SetAttribute("firstsparea", wi.FirstSpawning);

	TiXmlElement * desc = new TiXmlElement( "description" );
	root->LinkEndChild(desc);
	desc->LinkEndChild(new TiXmlText(wi.Description));

	// block: teleport
	{
		comment = new TiXmlComment();
		s="Define the place it is possible to teleport to, the first place is the place where we arrive in the world";
		comment->SetValue(s.c_str());
		root->LinkEndChild( comment );

		TiXmlElement * tps = new TiXmlElement( "teleports" );
		root->LinkEndChild(tps);


		std::map<std::string, TPInfo>::const_iterator it = wi.Teleports.begin();
		std::map<std::string, TPInfo>::const_iterator end = wi.Teleports.end();
		for(; it != end; ++it)
		{
			TiXmlElement * tp = new TiXmlElement( "teleport" );
			tps->LinkEndChild(tp);

			tp->SetAttribute("name", it->second.Name);
			tp->SetAttribute("map", it->second.NewMap);
			tp->SetAttribute("sparea", it->second.Spawning);
		}
	}

		// file info block
		{
			TiXmlElement * files = new TiXmlElement( "files" );
			root->LinkEndChild(files);

			std::map<std::string, std::string>::const_iterator it2 = wi.Files.begin();
			std::map<std::string, std::string>::const_iterator end2 = wi.Files.end();
			for(; it2 != end2; ++it2)
			{
				TiXmlElement * file = new TiXmlElement( "file" );
				files->LinkEndChild(file);

				file->SetAttribute("name", it2->first);
				file->SetAttribute("path", it2->second);
			}
		}



	TiXmlElement * maps = new TiXmlElement( "maps" );
	root->LinkEndChild(maps);

	std::map<std::string, MapInfo>::const_iterator it = wi.Maps.begin();
	std::map<std::string, MapInfo>::const_iterator end = wi.Maps.end();
	for(; it != end; ++it)
	{
		comment = new TiXmlComment();
		s="Map of "+it->second.Description;
		comment->SetValue(s.c_str());
		maps->LinkEndChild( comment );


		TiXmlElement * map = new TiXmlElement( "Map" );
		maps->LinkEndChild(map);

		map->SetAttribute("name", it->first);
		map->SetAttribute("type", it->second.Type);
		map->SetAttribute("music", it->second.Music);
		map->SetAttribute("repeatmusic", it->second.MusicLoop);


		TiXmlElement * descm = new TiXmlElement( "description" );
		map->LinkEndChild(descm);
		descm->LinkEndChild(new TiXmlText(it->second.Description));


		// file info block
		{
			comment = new TiXmlComment();
			s="Give the path of the files containing the island information to be loaded";
			comment->SetValue(s.c_str());
			map->LinkEndChild( comment );

			TiXmlElement * files = new TiXmlElement( "files" );
			map->LinkEndChild(files);

			std::map<std::string, std::string>::const_iterator it2 = it->second.Files.begin();
			std::map<std::string, std::string>::const_iterator end2 = it->second.Files.end();
			for(; it2 != end2; ++it2)
			{
				TiXmlElement * file = new TiXmlElement( "file" );
				files->LinkEndChild(file);

				file->SetAttribute("name", it2->first);
				file->SetAttribute("path", it2->second);
			}
		}


		// light info block
		{
			comment = new TiXmlComment();
			s="Describe the lights present in the scene";
			comment->SetValue(s.c_str());
			map->LinkEndChild( comment );

			TiXmlElement * lights = new TiXmlElement( "lights" );
			map->LinkEndChild(lights);

			std::map<std::string, LighInfo>::const_iterator it2 = it->second.Lights.begin();
			std::map<std::string, LighInfo>::const_iterator end2 = it->second.Lights.end();
			for(; it2 != end2; ++it2)
			{
				TiXmlElement * light = new TiXmlElement( "light" );
				lights->LinkEndChild(light);

				light->SetAttribute("name", it2->first);
				light->SetAttribute("type", it2->second.Type);

				light->SetAttribute("posX", it2->second.PosX);
				light->SetAttribute("posY", it2->second.PosY);
				light->SetAttribute("posZ", it2->second.PosZ);
				light->SetAttribute("dirX", it2->second.DirX);
				light->SetAttribute("dirY", it2->second.DirY);
				light->SetAttribute("dirZ", it2->second.DirZ);
			}
		}


		// spawning areas info block
		{
			comment = new TiXmlComment();
			s="Describe the possible spawning areas of the main character";
			comment->SetValue(s.c_str());
			map->LinkEndChild( comment );

			TiXmlElement * spareas = new TiXmlElement( "spareas" );
			map->LinkEndChild(spareas);

			std::map<std::string, SpawningInfo>::const_iterator it2 = it->second.Spawnings.begin();
			std::map<std::string, SpawningInfo>::const_iterator end2 = it->second.Spawnings.end();
			for(; it2 != end2; ++it2)
			{
				TiXmlElement * sparea = new TiXmlElement( "sparea" );
				spareas->LinkEndChild(sparea);

				sparea->SetAttribute("name", it2->first);

				sparea->SetDoubleAttribute("posX", it2->second.PosX);
				sparea->SetDoubleAttribute("posY", it2->second.PosY);
				sparea->SetDoubleAttribute("posZ", it2->second.PosZ);
				sparea->SetAttribute("RotationAtArrival", it2->second.Rotation);
			}
		}


		// map exit info block
		{
			comment = new TiXmlComment();
			s="Describe the possible map exit places using square areas from TopLeft point to BottomRight point";
			comment->SetValue(s.c_str());
			map->LinkEndChild( comment );

			TiXmlElement * exits = new TiXmlElement( "exits" );
			map->LinkEndChild(exits);

			std::map<std::string, ExitInfo>::const_iterator it2 = it->second.Exits.begin();
			std::map<std::string, ExitInfo>::const_iterator end2 = it->second.Exits.end();
			for(; it2 != end2; ++it2)
			{
				TiXmlElement * exit = new TiXmlElement( "exit" );
				exits->LinkEndChild(exit);

				exit->SetAttribute("name", it2->first);

				exit->SetDoubleAttribute("TopRightX", it2->second.TopRightX);
				exit->SetDoubleAttribute("TopRightY", it2->second.TopRightY);
				exit->SetDoubleAttribute("TopRightZ", it2->second.TopRightZ);

				exit->SetDoubleAttribute("BottomLeftX", it2->second.BottomLeftX);
				exit->SetDoubleAttribute("BottomLeftY", it2->second.BottomLeftY);
				exit->SetDoubleAttribute("BottomLeftZ", it2->second.BottomLeftZ);

				exit->SetAttribute("newMap", it2->second.NewMap);
				exit->SetAttribute("spawning", it2->second.Spawning);
			}
		}

	}

	doc.SaveFile(Filename);
}



// save world information into file
void MapInfoXmlWriter::SaveTexts(const std::string Filename, const std::map<long, std::string> &txts)
{
	TiXmlDocument doc;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement("texts");
	doc.LinkEndChild( root );

	std::map<long, std::string>::const_iterator it = txts.begin();
	std::map<long, std::string>::const_iterator end = txts.end();
	for(; it != end; ++it)
	{
		TiXmlElement * txt = new TiXmlElement( "text" );
		txt->SetAttribute("id", it->first);
		root->LinkEndChild(txt);
		txt->LinkEndChild(new TiXmlText(it->second));
	}


	doc.SaveFile(Filename);
}



// save map actors into memory
void MapInfoXmlWriter::SaveActors(const std::string &Filename, std::map<long, Actor *> * vec)
{
	TiXmlDocument doc;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement("actors");
	doc.LinkEndChild( root );

	std::map<long, Actor *>::const_iterator it = vec->begin();
	std::map<long, Actor *>::const_iterator end = vec->end();
	for(; it != end; ++it)
	{
		TiXmlElement * act = new TiXmlElement( "actor" );
		root->LinkEndChild(act);
		act->SetAttribute("id", it->second->GetId());
		act->SetAttribute("type", it->second->GetType());
		act->SetDoubleAttribute("posX", it->second->GetPosX());
		act->SetDoubleAttribute("posY", it->second->GetPosY());
		act->SetDoubleAttribute("posZ", it->second->GetPosZ());
		act->SetDoubleAttribute("sizeX", it->second->GetSizeX());
		act->SetDoubleAttribute("sizeY", it->second->GetSizeY());
		act->SetDoubleAttribute("sizeZ", it->second->GetSizeZ());
		act->SetDoubleAttribute("rotation", it->second->GetRotation());
		act->SetAttribute("passable", it->second->IsPassable());
		act->SetAttribute("depthmask", it->second->IsDepthMask());
		act->SetAttribute("movable", it->second->IsMovable());
		act->SetAttribute("outputsignal", it->second->GetSignal());
		act->SetAttribute("attachedsound", it->second->GetAttachedSound());

		{
			std::vector<long> vect = it->second->GetTargets();
			std::stringstream str;
			if(vect.size() > 0)
				str<<vect[0];

			for(size_t i=1; i<vect.size(); ++i)
				str<<","<<vect[i];

			act->SetAttribute("signaltargets", str.str());
		}

		if(it->second->GetRenderer() != NULL)
		{
			act->SetAttribute("renderertype", it->second->GetRendererType());

			std::vector<long> vect = it->second->GetRendererTarget();
			std::stringstream str;
			if(vect.size() > 0)
				str<<vect[0];

			for(size_t i=1; i<vect.size(); ++i)
				str<<","<<vect[i];
			act->SetAttribute("renderertarget", str.str());
		}


		switch(it->second->GetType())
		{
			case 1:	//text actor class
			{
				{
					TextActor * tmpa = static_cast<TextActor *>(it->second);
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetAttribute("textid", tmpa->GetTextId());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;

			case 2:	//ladder actor class
			{
				{
					LadderActor * tmpa = static_cast<LadderActor *>(it->second);
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetDoubleAttribute("deltaX", tmpa->GetDX());
					act->SetDoubleAttribute("deltaY", tmpa->GetDY());
					act->SetDoubleAttribute("deltaZ", tmpa->GetDZ());
					act->SetAttribute("direction", tmpa->GetDir());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;

			case 3:	//exit actor class
			{
				{
					ExitActor * tmpa = static_cast<ExitActor *>(it->second);
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetDoubleAttribute("deltaX", tmpa->GetDX());
					act->SetDoubleAttribute("deltaY", tmpa->GetDY());
					act->SetDoubleAttribute("deltaZ", tmpa->GetDZ());
					act->SetAttribute("direction", tmpa->GetDir());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;

			case 4:	//door actor class
			{
				{
					DoorActor * tmpa = static_cast<DoorActor *>(it->second);
					act->SetDoubleAttribute("zoneSizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zoneSizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zoneSizeZ", tmpa->GetZoneZ());
					act->SetAttribute("locked", tmpa->GetLocked());
					act->SetAttribute("keyid", tmpa->GetKeyId());
					act->SetAttribute("hide", tmpa->GetHide());
					act->SetAttribute("destroykey", tmpa->GetDesKey());

					act->SetDoubleAttribute("OpenTransX", tmpa->GetOpenTransX());
					act->SetDoubleAttribute("OpenTransY", tmpa->GetOpenTransY());
					act->SetDoubleAttribute("OpenTransZ", tmpa->GetOpenTransZ());
					act->SetDoubleAttribute("OpenTransSpeedX", tmpa->GetOpenTransSpeedX());
					act->SetDoubleAttribute("OpenTransSpeedY", tmpa->GetOpenTransSpeedY());
					act->SetDoubleAttribute("OpenTransSpeedZ", tmpa->GetOpenTransSpeedZ());
				}
			}
			break;

			case 5:	//container actor class
			{
				{
					ContainerActor * tmpa = static_cast<ContainerActor *>(it->second);
					act->SetDoubleAttribute("zonesizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zonesizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zonesizeZ", tmpa->GetZoneZ());
					act->SetAttribute("activationtype", tmpa->GetActivationType());

					const std::vector<ItemGroup> & lootlist = tmpa->GetLootList();
					for(size_t cci=0; cci<lootlist.size(); ++cci)
					{
						TiXmlElement * grit = new TiXmlElement( "itemgroup" );
						grit->SetDoubleAttribute("respawnTimeInMs", lootlist[cci].RespawningTime);
						act->LinkEndChild(grit);

						for(size_t ccj=0; ccj<lootlist[cci].groupelements.size(); ++ccj)
						{
							TiXmlElement * grelem = new TiXmlElement( "item" );
							grelem->SetDoubleAttribute("id", lootlist[cci].groupelements[ccj].id);
							grelem->SetDoubleAttribute("number", lootlist[cci].groupelements[ccj].number);
							grelem->SetDoubleAttribute("probability", lootlist[cci].groupelements[ccj].probability);
							grit->LinkEndChild(grelem);
						}
					}
				}
			}
			break;

			case 6:	//up exit actor class
			{
				{
					UpExitActor * tmpa = static_cast<UpExitActor *>(it->second);
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetAttribute("direction", tmpa->GetDir());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;

			case 7:	//switch actor class
			{
				{
					SwitchActor * tmpa = static_cast<SwitchActor *>(it->second);
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;

			case 8:	//area switch actor class
			{
				{
					AreaSwitch * tmpa = static_cast<AreaSwitch *>(it->second);
					act->SetDoubleAttribute("zonesizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zonesizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zonesizeZ", tmpa->GetZoneZ());
				}
			}
			break;

			case 9:	//floor switch actor class
			{
				{
					FloorSwitch * tmpa = static_cast<FloorSwitch *>(it->second);
					act->SetDoubleAttribute("zonesizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zonesizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zonesizeZ", tmpa->GetZoneZ());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
				}
			}
			break;


			case 10:	//lift actor class
			{
				{
					TiXmlElement * scripts = new TiXmlElement( "scripts" );
					act->LinkEndChild(scripts);

					ScriptableActor * tmpa = static_cast<ScriptableActor *>(it->second);
					const std::vector<PlayerScriptPart> & scriptsV = tmpa->GetScripts();
					for(size_t cci=0; cci<scriptsV.size(); ++cci)
					{
						TiXmlElement * script = new TiXmlElement( "script" );
						scripts->LinkEndChild(script);

						script->SetAttribute("type", scriptsV[cci].Type);
						script->SetDoubleAttribute("ValueA", scriptsV[cci].ValueA);
						script->SetDoubleAttribute("ValueB", scriptsV[cci].ValueB);
						script->SetDoubleAttribute("ValueC", scriptsV[cci].ValueC);
						script->SetDoubleAttribute("Speed", scriptsV[cci].Speed);
						script->SetAttribute("Sound", scriptsV[cci].Sound);
						script->SetAttribute("SoundNum", scriptsV[cci].SoundNum);
						script->SetAttribute("Animation", scriptsV[cci].Animation);
						script->SetAttribute("Flag", scriptsV[cci].Flag);
					}
				}
			}
			break;

			case 11:	//hurt area class
			{
				{
					HurtArea * tmpa = static_cast<HurtArea *>(it->second);
					act->SetDoubleAttribute("zonesizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zonesizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zonesizeZ", tmpa->GetZoneZ());
					act->SetDoubleAttribute("lifetaken", tmpa->GetLifeTaken());
				}
			}
			break;


			case 12:	//NPC actor class
			{
				{
					NPCActor * tmpa = static_cast<NPCActor *>(it->second);
					act->SetAttribute("NPCType", tmpa->GetNPCType());
					act->SetDoubleAttribute("activationdistance", tmpa->GetActivationDistance());
					act->SetAttribute("Name", tmpa->GetName());

					TiXmlElement * scripts = new TiXmlElement( "scripts" );
					act->LinkEndChild(scripts);

					const std::vector<PlayerScriptPart> & scriptsV = tmpa->GetScripts();
					for(size_t cci=0; cci<scriptsV.size(); ++cci)
					{
						TiXmlElement * script = new TiXmlElement( "script" );
						scripts->LinkEndChild(script);

						script->SetAttribute("type", scriptsV[cci].Type);
						script->SetDoubleAttribute("ValueA", scriptsV[cci].ValueA);
						script->SetDoubleAttribute("ValueB", scriptsV[cci].ValueB);
						script->SetDoubleAttribute("ValueC", scriptsV[cci].ValueC);
						script->SetDoubleAttribute("Speed", scriptsV[cci].Speed);
						script->SetAttribute("Sound", scriptsV[cci].Sound);
						script->SetAttribute("SoundNum", scriptsV[cci].SoundNum);
						script->SetAttribute("Animation", scriptsV[cci].Animation);
						script->SetAttribute("Flag", scriptsV[cci].Flag);
					}
				}
			}
			break;


			case 13:	//scripted zone actor
			{
				{
					ScriptedZoneActor * tmpa = static_cast<ScriptedZoneActor *>(it->second);
					act->SetDoubleAttribute("zonesizeX", tmpa->GetZoneX());
					act->SetDoubleAttribute("zonesizeY", tmpa->GetZoneY());
					act->SetDoubleAttribute("zonesizeZ", tmpa->GetZoneZ());
					act->SetAttribute("activationtype", tmpa->GetActivationType());
					act->SetAttribute("neededitem", tmpa->GetNeededItemId());
					act->SetAttribute("destroyitem", tmpa->GetDesItem());
					act->SetAttribute("abortedmessage", tmpa->GetAbortedMessage());

					TiXmlElement * scripts = new TiXmlElement( "scripts" );
					act->LinkEndChild(scripts);
					const std::vector<PlayerScriptPart> & scriptsV = tmpa->GetScripts();
					for(size_t cci=0; cci<scriptsV.size(); ++cci)
					{
						TiXmlElement * script = new TiXmlElement( "script" );
						scripts->LinkEndChild(script);

						script->SetAttribute("type", scriptsV[cci].Type);
						script->SetDoubleAttribute("ValueA", scriptsV[cci].ValueA);
						script->SetDoubleAttribute("ValueB", scriptsV[cci].ValueB);
						script->SetDoubleAttribute("ValueC", scriptsV[cci].ValueC);
						script->SetDoubleAttribute("Speed", scriptsV[cci].Speed);
						script->SetAttribute("Sound", scriptsV[cci].Sound);
						script->SetAttribute("SoundNum", scriptsV[cci].SoundNum);
						script->SetAttribute("Animation", scriptsV[cci].Animation);
						script->SetAttribute("Flag", scriptsV[cci].Flag);

						script->SetAttribute("newMap", scriptsV[cci].NewMap);
						script->SetAttribute("spawning", scriptsV[cci].Spawning);
					}
				}
			}
			break;
		}
	}


	doc.SaveFile(Filename);
}


// save all sprites info
void MapInfoXmlWriter::SaveSprites(const std::string &Filename, std::map<long, SpriteInfo> *vec)
{
	TiXmlDocument doc;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "UTF8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement("sprites");
	doc.LinkEndChild( root );

	std::map<long, SpriteInfo>::const_iterator it = vec->begin();
	std::map<long, SpriteInfo>::const_iterator end = vec->end();
	for(; it != end; ++it)
	{
		TiXmlElement * act = new TiXmlElement( "sprite" );
		root->LinkEndChild(act);
		act->SetAttribute("id", it->first);
		act->SetAttribute("filename", it->second.filename);

		std::vector<QuadImageInfo>::const_iterator itq = it->second.quadsInfo.begin();
		std::vector<QuadImageInfo>::const_iterator endq = it->second.quadsInfo.end();
		for(; itq != endq; ++itq)
		{
			TiXmlElement * quad = new TiXmlElement( "textpart" );
			act->LinkEndChild(quad);
			quad->SetAttribute("UseFullImage", itq->UseFullImage);

			quad->SetDoubleAttribute("BottomLeftCornerX", itq->BottomLeftCornerX);
			quad->SetDoubleAttribute("BottomLeftCornerY", itq->BottomLeftCornerY);
			quad->SetDoubleAttribute("BottomLeftCornerZ", itq->BottomLeftCornerZ);
			quad->SetDoubleAttribute("TopRightCornerX", itq->TopRightCornerX);
			quad->SetDoubleAttribute("TopRightCornerY", itq->TopRightCornerY);
			quad->SetDoubleAttribute("TopRightCornerZ", itq->TopRightCornerZ);

			quad->SetDoubleAttribute("BottomRightCornerX", itq->BottomRightCornerX);
			quad->SetDoubleAttribute("BottomRightCornerY", itq->BottomRightCornerY);
			quad->SetDoubleAttribute("BottomRightCornerZ", itq->BottomRightCornerZ);
			quad->SetDoubleAttribute("TopLeftCornerX", itq->TopLeftCornerX);
			quad->SetDoubleAttribute("TopLeftCornerY", itq->TopLeftCornerY);
			quad->SetDoubleAttribute("TopLeftCornerZ", itq->TopLeftCornerZ);

			//quad->SetDoubleAttribute("BottomRightCornerX",itq->TopRightCornerX);//itq->BottomRightCornerX);
			//quad->SetDoubleAttribute("BottomRightCornerY",itq->BottomLeftCornerY);//itq->BottomRightCornerY);
			//quad->SetDoubleAttribute("BottomRightCornerZ",itq->TopRightCornerZ);//itq->BottomRightCornerZ);
			//quad->SetDoubleAttribute("TopLeftCornerX",itq->BottomLeftCornerX);//itq->TopLeftCornerX);
			//quad->SetDoubleAttribute("TopLeftCornerY",itq->TopRightCornerY);//itq->TopLeftCornerY);
			//quad->SetDoubleAttribute("TopLeftCornerZ",itq->BottomLeftCornerZ);//itq->TopLeftCornerZ);

			quad->SetAttribute("BottomLeftTextcoordX", itq->BottomLeftTextcoordX);
			quad->SetAttribute("BottomLeftTextcoordY", itq->BottomLeftTextcoordY);

			quad->SetAttribute("BottomRightTextcoordX", itq->BottomRightTextcoordX);
			quad->SetAttribute("BottomRightTextcoordY", itq->BottomRightTextcoordY);

			quad->SetAttribute("TopLeftTextcoordX", itq->TopLeftTextcoordX);
			quad->SetAttribute("TopLeftTextcoordY", itq->TopLeftTextcoordY);

			quad->SetAttribute("TopRightTextcoordX", itq->TopRightTextcoordX);
			quad->SetAttribute("TopRightTextcoordY", itq->TopRightTextcoordY);
		}
	}


	doc.SaveFile(Filename);
}