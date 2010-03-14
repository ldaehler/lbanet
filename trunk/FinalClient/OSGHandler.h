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


#ifndef _LBANET_OSG_HANDLER_H_
#define _LBANET_OSG_HANDLER_H_

#include <vector>
#include <string>
#include <osg/ref_ptr>

#include "CommonTypes.h"

namespace osg
{
	class Node;
	class PositionAttitudeTransform;
	class Group;
	class MatrixTransform;
	class LightSource;
	class ClipNode;
}

namespace osgViewer
{
	class Viewer;
}

namespace osgShadow
{
	class ShadowedScene;
}

class EventHandler;


//*************************************************************************************************
//*                               class OsgHandler
//*************************************************************************************************
/**
* @brief Class takign care of the OSG 3D engine
*
*/
class OsgHandler
{
public:

	// singleton pattern
	static OsgHandler * getInstance();

	//! destructor
	~OsgHandler();

	//! initialize
	void Initialize(const std::string &WindowName, const std::string &DataPath, EventHandler * evH);

	//! finalize function
	void Finalize();


	//! change screen resolution
	void Resize(int resX, int resY);

	//! toggle fullscreen or windowed mode
	void ToggleFullScreen();
	
	//! set if the view is perspective or ortho
	void TogglePerspectiveView(bool Perspective);

	//! set camera distance
	void SetCameraDistance(double distance);

	//! delta update camera distance
	void DeltaUpdateCameraDistance(double delta);

	//! set camera zenit
	void SetCameraZenit(double zenit);

	//! delta update camera zenit
	void DeltaUpdateCameraZenit(double delta);

	//! set camera target
	void SetCameraTarget(double TargetX, double TargetY, double TargetZ);

	//! clear all nodes of the display tree
	//! typically called when changing map
	//! set if new map uses lighning or not
	void ResetDisplayTree(const LbaMainLightInfo &NewLightningInfo);

	//! update display - returns true if need to terminate
	bool Update();

	//! load osg files into a osg node
	osg::ref_ptr<osg::Node> LoadOSGFile(const std::string & filename);

	//! add a actor to the display list - return handler to actor position
	osg::ref_ptr<osg::MatrixTransform> AddActorNode(osg::ref_ptr<osg::Node> node);

	//! remove actor from the graph
	void RemoveActorNode(osg::ref_ptr<osg::Node> node);

	//! reset light (can be used to switch light on/off
	void ResetLight(bool On);

	//! set the node the camera needs to follow
	void SetCameraFollowingNode(osg::ref_ptr<osg::MatrixTransform> node);

	//! set clip plane cut layer
	void SetClipPlane(float layer);

protected:
	//! constructor
	OsgHandler();

	//! set or reset screen
	void ResetScreen();

	//! set or reset camera projection matrix
	void ResetCameraProjectiomMatrix();

	//! set or reset camera transform
	void ResetCameraTransform();

	//! update camera position
	void UpdateCameraPos();

private:
	// singleton
	static OsgHandler * _singletonInstance;

	
	// screen info
	bool	_isFullscreen;
	int		_resX;
	int		_resY;
	int		_viewportX;
	int		_viewportY;

	bool	_displayShadow;


	// camera info
	bool	_isPerspective;
	double	_targetx;
	double	_targety;
	double	_targetz;
	double	_fov;
	double	_distance;
	double	_zenit;
	float	_current_clip_layer;

	LbaMainLightInfo _currLightInfo;


	// osg handlers
	osg::ref_ptr<osgViewer::Viewer>					_viewer;
	osg::ref_ptr<osg::PositionAttitudeTransform>	_rootNode;
	osg::ref_ptr<osg::PositionAttitudeTransform>	_translNode;
	osg::ref_ptr<osg::LightSource>					_lightNode;
	osg::ref_ptr<osg::Group>						_sceneRootNode;
	osg::ref_ptr<osg::ClipNode>						_clipNode;

	// camera internal info
	osg::ref_ptr<osg::MatrixTransform>				_cameraNode;
	double											_lastactX;
	double											_lastactY;
	double											_lastactZ;
	bool											_movecamera;
};


#endif
