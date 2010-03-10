/* OpenSceneGraph example, osgkeyboardmouse.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

// Simple example of use of osgViewer::GraphicsWindow + Viewer
// example that provides the user with control over view position with basic picking.

#include <osg/Timer>
#include <osg/io_utils>
#include <osg/observer_ptr>

#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>

#include <iostream>
#include <sstream>


#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/CullFace>
#include <osg/Depth>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ComputeBoundsVisitor>
#include <osgUtil/Optimizer>
#include <osg/PositionAttitudeTransform>


#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ParallelSplitShadowMap>
#include <osgShadow/LightSpacePerspectiveShadowMap>
#include <osgShadow/StandardShadowMap>

//#include "LBACharacterRenderer.h"

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;



// class to handle events with a pick
class PickHandler : public osgGA::GUIEventHandler
{
public:

    PickHandler(osg::ref_ptr<osg::Group> root, const std::vector<std::string> & mapfiles)
		: _root(root), _currmap_idx(0), _mapfiles(mapfiles)
	{
		LoadCurrentMap();
	}

    ~PickHandler() {}

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer) 
			return false;

        switch(ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::KEYUP):
            {
				if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up)
				{
					++_currmap_idx;
					if(_currmap_idx >= (int)_mapfiles.size())
						_currmap_idx = 0;
					LoadCurrentMap();
				}

				if(ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down)
				{
					--_currmap_idx;
					if(_currmap_idx < 0)
						_currmap_idx = (int)_mapfiles.size()-1;
					LoadCurrentMap();
				}
                return false;
            }
 
            default:
                return false;
        }
    }

	void LoadCurrentMap()
	{
		if(_currmap_idx < (int)_mapfiles.size())
		{	  
			_root->removeChild(_currentmap);

			_currentmap = osgDB::readNodeFile(_mapfiles[_currmap_idx]);
			if (!_currentmap)
				return;

			_currentmap->setNodeMask(CastsShadowTraversalMask | ReceivesShadowTraversalMask);

			_root->addChild(_currentmap);
		}
	}

private:
	osg::ref_ptr<osg::Group>	_root;
	osg::ref_ptr<osg::Node>		_currentmap;
	std::vector<std::string>	_mapfiles;
	int							_currmap_idx;
};




int main( int argc, char **argv )
{
	bool updateLightPosition = true;
	bool postionalLight = true;
	bool spotlight = false;

	osgDB::setDataFilePathList("./Data");

	std::vector<std::string> maps;
	maps.push_back("Lba1/Maps/map0.osgt");


	for(int i=1; i<=30/*133*/; ++i)
	{
		std::stringstream filename;
		filename<<"Lba1/Maps/map"<<i<<".osgb";
		maps.push_back(filename.str());
	}

	for(int i=1; i<=30/*148*/; ++i)
	{
		std::stringstream filename;
		filename<<"Lba2/Maps/map"<<i<<".osgb";
		maps.push_back(filename.str());
	}


	//maps.push_back("map0.osgb");



	osg::ref_ptr<osg::PositionAttitudeTransform> transformnode =new osg::PositionAttitudeTransform();
	transformnode->setScale(osg::Vec3d(1, 0.5, 1));



	// get the bounds of the model.
	//osg::ComputeBoundsVisitor cbbv;
	//loadedModel->accept(cbbv);
	//osg::BoundingBox bb = cbbv.getBoundingBox();

	osg::Vec4 lightpos;

	//if (postionalLight)
	//{
	//	lightpos.set(bb.center().x(), bb.center().y(), bb.zMax() + bb.radius()*2.0f  ,1.0f);
	//}
	//else
	//{
		lightpos.set(0.5f,0.25f,0.8f,0.0f);
	//}

	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	osg::ref_ptr<osgShadow::StandardShadowMap> st = new osgShadow::StandardShadowMap;
	shadowedScene->setShadowTechnique(st.get());


	osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
	ls->getLight()->setPosition(lightpos);
	shadowedScene->addChild(ls.get());

	PickHandler* phandler = new PickHandler(shadowedScene, maps);


	//osg::ref_ptr<osg::PositionAttitudeTransform> chartrans =new osg::PositionAttitudeTransform();
	//chartrans->setPosition(osg::Vec3d(30, 30, 30));

	
	//osg::ref_ptr<osg::Geode> chargeode = new osg::Geode();
	//osg::ref_ptr<LBACharacterRenderer> charrender = new LBACharacterRenderer(2.2f);
	//charrender->changeAnimEntity(0, 0);
	//charrender->setActorAnimation(0);

	//chargeode->addDrawable(charrender);

	//chartrans->addChild(chargeode);
	//transformnode->addChild(chargeode);


	//if(spotlight)
	//{
	//	osg::Vec3 center = bb.center();
	//	osg::Vec3 lightdir = center - osg::Vec3(lightpos.x(), lightpos.y(), lightpos.z());
	//	lightdir.normalize();
	//	ls->getLight()->setDirection(lightdir);
	//	ls->getLight()->setSpotCutoff(25.0f);
	//}


	ls->getLight()->setAmbient(osg::Vec4(0.2,0.2,0.2,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.8,0.8,0.8,1.0));



	//set the LightSource, only for checking, there is only 1 light in the scene
	osgShadow::ShadowMap* shadowMap = dynamic_cast<osgShadow::ShadowMap*>(shadowedScene->getShadowTechnique());
	if( shadowMap ) 
		shadowMap->setLight(ls.get());



    // create the window to draw to.
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 100;
    traits->y = 100;
    traits->width = 1024;
    traits->height = 768;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;
	traits->windowName = "OSGTest";

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
    if (!gw)
    {
        osg::notify(osg::NOTICE)<<"Error: unable to create graphics window."<<std::endl;
        return 1;
    }


    // create the view of the scene.
    osgViewer::Viewer viewer;
    viewer.getCamera()->setGraphicsContext(gc.get());
    viewer.getCamera()->setViewport(0,0,1024,768);
	viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));



	transformnode->addChild(shadowedScene);
    viewer.setSceneData(transformnode);
   
    // create a tracball manipulator to move the camera around in response to keyboard/mouse events
    viewer.setCameraManipulator( new osgGA::TrackballManipulator );

    osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator(viewer.getCamera()->getStateSet());
    viewer.addEventHandler(statesetManipulator.get());

    // add the pick handler
    viewer.addEventHandler(phandler);
	viewer.addEventHandler( new osgViewer::StatsHandler() );

    viewer.realize();
	double lasttime = viewer.getFrameStamp()->getReferenceTime();

	while (!viewer.done())
	{
		if(updateLightPosition)
		{
			float t = viewer.getFrameStamp()->getSimulationTime();

			//if (postionalLight)
			//{
			//	lightpos.set(bb.center().x()+sinf(t)*bb.radius(), bb.center().y() + cosf(t)*bb.radius(), bb.zMax() + bb.radius()*3.0f  ,1.0f);
			//}
			//else
			//{
				lightpos.set(sinf(t),cosf(t),1.0f,0.0f);
			//}
			ls->getLight()->setPosition(lightpos);

			osg::Vec3f lightDir(-lightpos.x(),-lightpos.y(),-lightpos.z());
			//if(spotlight)
			//	lightDir =  osg::Vec3(bb.center().x()+sinf(t)*bb.radius()/2.0, bb.center().y() + cosf(t)*bb.radius()/2.0, bb.center().z())
			//	- osg::Vec3(lightpos.x(), lightpos.y(), lightpos.z()) ;
			lightDir.normalize();
			ls->getLight()->setDirection(lightDir);
		}

		//{
		//	double tnow = viewer.getFrameStamp()->getReferenceTime();
		//	charrender->Process(tnow, (float)(tnow-lasttime));
		//	lasttime = tnow;
		//}

		viewer.frame();
	}


    return 0;
}
















//
//
//
//
//
//
//#include <osg/Notify>
//#include <osg/MatrixTransform>
//#include <osg/PositionAttitudeTransform>
//#include <osg/Geometry>
//#include <osg/Geode>
//
//#include <osgUtil/Optimizer>
//
//#include <osgDB/Registry>
//#include <osgDB/ReadFile>
//#include <osgDB/FileUtils>
//
//#include <osgGA/TrackballManipulator>
//#include <osgGA/FlightManipulator>
//#include <osgGA/DriveManipulator>
//
//#include <osgSim/OverlayNode>
//
//#include <osgViewer/Viewer>
//#include <iostream>
//
//osg::AnimationPath* createAnimationPath(const osg::Vec3& center,float radius,double looptime)
//{
//    // set up the animation path
//    osg::AnimationPath* animationPath = new osg::AnimationPath;
//    animationPath->setLoopMode(osg::AnimationPath::LOOP);
//   
//    int numSamples = 40;
//    float yaw = 0.0f;
//    float yaw_delta = 2.0f*osg::PI/((float)numSamples-1.0f);
//    float roll = osg::inDegrees(30.0f);
//   
//    double time=0.0f;
//    double time_delta = looptime/(double)numSamples;
//    for(int i=0;i<numSamples;++i)
//    {
//        osg::Vec3 position(center+osg::Vec3(sinf(yaw)*radius,cosf(yaw)*radius,0.0f));
//        osg::Quat rotation(osg::Quat(roll,osg::Vec3(0.0,1.0,0.0))*osg::Quat(-(yaw+osg::inDegrees(90.0f)),osg::Vec3(0.0,0.0,1.0)));
//       
//        animationPath->insert(time,osg::AnimationPath::ControlPoint(position,rotation));
//
//        yaw += yaw_delta;
//        time += time_delta;
//
//    }
//    return animationPath;   
//}
//
//osg::Node* createBase(const osg::Vec3& center,float radius)
//{
//
//   
//
//    int numTilesX = 10;
//    int numTilesY = 10;
//   
//    float width = 2*radius;
//    float height = 2*radius;
//   
//    osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
//    osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
//    osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));
//   
//    // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
//    osg::Vec3Array* coords = new osg::Vec3Array;
//    int iy;
//    for(iy=0;iy<=numTilesY;++iy)
//    {
//        for(int ix=0;ix<=numTilesX;++ix)
//        {
//            coords->push_back(v000+dx*(float)ix+dy*(float)iy);
//        }
//    }
//   
//    //Just two colours - black and white.
//    osg::Vec4Array* colors = new osg::Vec4Array;
//    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f)); // white
//    colors->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.0f)); // black
//    int numColors=colors->size();
//   
//   
//    int numIndicesPerRow=numTilesX+1;
//    osg::UByteArray* coordIndices = new osg::UByteArray; // assumes we are using less than 256 points...
//    osg::UByteArray* colorIndices = new osg::UByteArray;
//    for(iy=0;iy<numTilesY;++iy)
//    {
//        for(int ix=0;ix<numTilesX;++ix)
//        {
//            // four vertices per quad.
//            coordIndices->push_back(ix    +(iy+1)*numIndicesPerRow);
//            coordIndices->push_back(ix    +iy*numIndicesPerRow);
//            coordIndices->push_back((ix+1)+iy*numIndicesPerRow);
//            coordIndices->push_back((ix+1)+(iy+1)*numIndicesPerRow);
//           
//            // one color per quad
//            colorIndices->push_back((ix+iy)%numColors);
//        }
//    }
//   
//
//    // set up a single normal
//    osg::Vec3Array* normals = new osg::Vec3Array;
//    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
//   
//
//    osg::Geometry* geom = new osg::Geometry;
//    geom->setVertexArray(coords);
//    geom->setVertexIndices(coordIndices);
//   
//    geom->setColorArray(colors);
//    geom->setColorIndices(colorIndices);
//    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
//   
//    geom->setNormalArray(normals);
//    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
//   
//    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));
//   
//    osg::Geode* geode = new osg::Geode;
//    geode->addDrawable(geom);
//   
//    return geode;
//}
//
//osg::Node* createMovingModel(const osg::Vec3& center, float radius)
//{
//    float animationLength = 10.0f;
//
//    osg::AnimationPath* animationPath = createAnimationPath(center,radius,animationLength);
//
//    osg::Group* model = new osg::Group;
//
//    osg::Node* glider = osgDB::readNodeFile("glider.osg");
//    if (glider)
//    {
//        const osg::BoundingSphere& bs = glider->getBound();
//
//        float size = radius/bs.radius()*0.3f;
//        osg::MatrixTransform* positioned = new osg::MatrixTransform;
//        positioned->setDataVariance(osg::Object::STATIC);
//        positioned->setMatrix(osg::Matrix::translate(-bs.center())*
//                                     osg::Matrix::scale(size,size,size)*
//                                     osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));
//   
//        positioned->addChild(glider);
//   
//        osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;   
//        xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0,1.0));
//        xform->addChild(positioned);
//
//        model->addChild(xform);
//    }
// 
//    osg::Node* cessna = osgDB::readNodeFile("cessna.osg");
//    if (cessna)
//    {
//        const osg::BoundingSphere& bs = cessna->getBound();
//
//        float size = radius/bs.radius()*0.3f;
//        osg::MatrixTransform* positioned = new osg::MatrixTransform;
//        positioned->setDataVariance(osg::Object::STATIC);
//        positioned->setMatrix(osg::Matrix::translate(-bs.center())*
//                                     osg::Matrix::scale(size,size,size)*
//                                     osg::Matrix::rotate(osg::inDegrees(180.0f),0.0f,0.0f,1.0f));
//   
//        positioned->addChild(cessna);
//   
//        osg::MatrixTransform* xform = new osg::MatrixTransform;
//        xform->setUpdateCallback(new osg::AnimationPathCallback(animationPath,0.0f,2.0));
//        xform->addChild(positioned);
//
//        model->addChild(xform);
//    }
//   
//    return model;
//}
//
//osg::Node* createModel(bool overlay, osgSim::OverlayNode::OverlayTechnique technique)
//{
//    osg::Vec3 center(0.0f,0.0f,0.0f);
//    float radius = 100.0f;
//
//    osg::Group* root = new osg::Group;
//
//    float baseHeight = center.z()-radius*0.5;
//    osg::Node* baseModel = createBase(osg::Vec3(center.x(), center.y(), baseHeight),radius);
//    osg::Node* movingModel = createMovingModel(center,radius*0.8f);
//
//    if (overlay)
//    {
//        osgSim::OverlayNode* overlayNode = new osgSim::OverlayNode(technique);
//        overlayNode->setContinuousUpdate(true);
//        overlayNode->setOverlaySubgraph(movingModel);
//        overlayNode->setOverlayBaseHeight(baseHeight-0.01);
//        overlayNode->addChild(baseModel);
//        root->addChild(overlayNode);
//    }
//    else
//    {
//   
//        root->addChild(baseModel);
//    }
//   
//    root->addChild(movingModel);
//
//    return root;
//}
//
//
//int main( int argc, char **argv )
//{
//	osgDB::setDataFilePathList("./Data;./Data/Images");
//   
//    bool overlay = false;
//    osg::ArgumentParser arguments(&argc,argv);
//    while (arguments.read("--overlay")) overlay = true;
//   
//    osgSim::OverlayNode::OverlayTechnique technique = osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY;
//    while (arguments.read("--object")) { technique = osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY; overlay=true; }
//    while (arguments.read("--ortho") || arguments.read("--orthographic")) { technique = osgSim::OverlayNode::VIEW_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY; overlay=true; }
//    while (arguments.read("--persp") || arguments.read("--perspective")) { technique = osgSim::OverlayNode::VIEW_DEPENDENT_WITH_PERSPECTIVE_OVERLAY; overlay=true; }
//   
//
//    // initialize the viewer.
//    osgViewer::Viewer viewer;
//
//    // load the nodes from the commandline arguments.
//    osg::Node* model = createModel(overlay, technique);
//    if (!model)
//    {
//        return 1;
//    }
//   
//    // tilt the scene so the default eye position is looking down on the model.
//    osg::MatrixTransform* rootnode = new osg::MatrixTransform;
//    rootnode->setMatrix(osg::Matrix::rotate(osg::inDegrees(30.0f),1.0f,0.0f,0.0f));
//    rootnode->addChild(model);
//
//    // run optimization over the scene graph
//    osgUtil::Optimizer optimzer;
//    optimzer.optimize(rootnode);
//     
//    // set the scene to render
//    viewer.setSceneData(rootnode);
//
//    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
//
//    // viewer.setUpViewOnSingleScreen(1);
//
//#if 0
//
//    // use of custom simulation time.
//   
//    viewer.realize();
//   
//    double simulationTime = 0.0;
//   
//    while (!viewer.done())
//    {
//        viewer.frame(simulationTime);
//        simulationTime += 0.001;
//    }
//   
//    return 0;
//#else
//
//    // normal viewer usage.
//    return viewer.run();
//
//#endif
//}