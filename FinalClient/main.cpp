#include "OSGHandler.h"
#include <osg/Node>


#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

#include <osgShadow/ShadowedScene>
#include <osgShadow/StandardShadowMap>
#include <osgShadow/ShadowMap>

static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

int main( int argc, char **argv )
{
	//osgDB::setDataFilePathList("./Data");
	//osg::ref_ptr<osg::PositionAttitudeTransform> transformnode =new osg::PositionAttitudeTransform();
	//transformnode->setScale(osg::Vec3d(1, 0.5, 1));


	//osg::Vec4 lightpos;
	//lightpos.set(0.5f,0.25f,0.8f,0.0f);


	//osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	//shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
	//shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);

	//osg::ref_ptr<osgShadow::StandardShadowMap> st = new osgShadow::StandardShadowMap;
	//shadowedScene->setShadowTechnique(st.get());


	//osg::ref_ptr<osg::LightSource> ls = new osg::LightSource;
	//ls->getLight()->setPosition(lightpos);
	//shadowedScene->addChild(ls.get());

	//shadowedScene->addChild(osgDB::readNodeFile("Lba1/Maps/map0.osgt"));


	//ls->getLight()->setAmbient(osg::Vec4(0.2,0.2,0.2,1.0));
	//ls->getLight()->setDiffuse(osg::Vec4(0.8,0.8,0.8,1.0));



	////set the LightSource, only for checking, there is only 1 light in the scene
	//osgShadow::ShadowMap* shadowMap = dynamic_cast<osgShadow::ShadowMap*>(shadowedScene->getShadowTechnique());
	//if( shadowMap ) 
	//	shadowMap->setLight(ls.get());



 //   // create the window to draw to.
 //   osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
 //   traits->x = 100;
 //   traits->y = 100;
 //   traits->width = 1024;
 //   traits->height = 768;
 //   traits->windowDecoration = true;
 //   traits->doubleBuffer = true;
 //   traits->sharedContext = 0;
	//traits->windowName = "OSGTest";

 //   osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
 //   osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
 //   if (!gw)
 //   {
 //       osg::notify(osg::NOTICE)<<"Error: unable to create graphics window."<<std::endl;
 //       return 1;
 //   }


 //   // create the view of the scene.
 //   osgViewer::Viewer viewer;
 //   viewer.getCamera()->setGraphicsContext(gc.get());
 //   viewer.getCamera()->setViewport(0,0,1024,768);
	//viewer.getCamera()->setClearColor(osg::Vec4(0, 0, 0, 1));



	//transformnode->addChild(shadowedScene);
 //   viewer.setSceneData(transformnode);
 //  
 //   // create a tracball manipulator to move the camera around in response to keyboard/mouse events
 //   viewer.setCameraManipulator( new osgGA::TrackballManipulator );


	//// add the pick handler
	//viewer.addEventHandler( new osgViewer::StatsHandler() );

 //   viewer.realize();
	//double lasttime = viewer.getFrameStamp()->getReferenceTime();

	//while (!viewer.done())
	//{
	//	viewer.frame();
	//}





	// init OSG
	OsgHandler::getInstance()->Initialize("LbaNet", "./Data");

	//load map
	OsgHandler::getInstance()->SetMap(OsgHandler::getInstance()->LoadOSGFile("Lba1/Maps/map0.osgt"));

	while(!OsgHandler::getInstance()->Update());

    return 0;
}

