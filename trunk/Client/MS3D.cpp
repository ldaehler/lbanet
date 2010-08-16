//Ronny André Reierstad
//www.morrowland.com
//apron@morrowland.com
// Thanks to Brett Porter and Mete Ciragan for help with this MS3D model loading code


#include "ms3d.h"

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library

#include <IL/ilut.h>

#include <fstream>

/////////////////////////////////////////////////////////////////////////////////////////////////
//										The MS3DModel Class
/////////////////////////////////////////////////////////////////////////////////////////////////
MS3DModel::MS3DModel()
{
	m_numMeshes = 0;
	m_pMeshes = NULL;
	m_numMaterials = 0;
	m_pMaterials = NULL;
	m_numTriangles = 0;
	m_pTriangles = NULL;
	m_numVertices = 0;
	m_pVertices = NULL;
	m_ScaleX = 1;
	m_ScaleY = 1;
	m_ScaleZ = 1;

	m_TransX = 0;
	m_TransY = 0;
	m_TransZ = 0;

	m_RotX = 0;
	m_RotY = 0;
	m_RotZ = 0;

	_type = 4;
}

MS3DModel::~MS3DModel()
{
	int i;
	for ( i = 0; i < m_numMeshes; i++ )
		delete[] m_pMeshes[i].m_pTriangleIndices;


	m_numMeshes = 0;
	if ( m_pMeshes != NULL )
	{
		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}

	m_numMaterials = 0;
	if ( m_pMaterials != NULL )
	{
		delete[] m_pMaterials;
		m_pMaterials = NULL;
	}

	m_numTriangles = 0;
	if ( m_pTriangles != NULL )
	{
		delete[] m_pTriangles;
		m_pTriangles = NULL;
	}

	m_numVertices = 0;
	if ( m_pVertices != NULL )
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}



bool MS3DModel::loadModelData( const std::string &filepath )
{
	const char * filename = filepath.c_str();

	std::ifstream inputFile(filename , std::ios::in | std::ios::binary );
	if ( inputFile.fail())
		return false;	// "Couldn't open the model file."

	std::string pathTemp;
	pathTemp = filepath.substr(0, filepath.find_last_of("/")+1);

	inputFile.seekg( 0, std::ios::end );
	long fileSize = inputFile.tellg();
	inputFile.seekg( 0, std::ios::beg );

	byte *pBuffer = new byte[fileSize];
	inputFile.read( (char *)pBuffer, fileSize );
	inputFile.close();

	const byte *pPtr = pBuffer;
	MS3DHeader *pHeader = ( MS3DHeader* )pPtr;
	pPtr += sizeof( MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
		return false; // "Not a valid Milkshape3D model file."

	if ( pHeader->m_version < 3 )
		return false; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *( word* )pPtr;
	m_numVertices = nVertices;
	m_pVertices = new Vertex[nVertices];
	pPtr += sizeof( word );

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
		m_pVertices[i].m_boneID = pVertex->m_boneID;
		memcpy( m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
		pPtr += sizeof( MS3DVertex );
	}

	int nTriangles = *( word* )pPtr;
	m_numTriangles = nTriangles;
	m_pTriangles = new Triangle[nTriangles];
	pPtr += sizeof( word );

	for ( i = 0; i < nTriangles; i++ )
	{
		MS3DTriangle *pTriangle = ( MS3DTriangle* )pPtr;
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
		memcpy( m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
		memcpy( m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
		memcpy( m_pTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		pPtr += sizeof( MS3DTriangle );
	}



	int nGroups = *( word* )pPtr;
	m_numMeshes = nGroups;
	m_pMeshes = new Mesh[nGroups];
	pPtr += sizeof( word );
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		word nTriangles = *( word* )pPtr;
		pPtr += sizeof( word );
		int *pTriangleIndices = new int[nTriangles];
		for ( int j = 0; j < nTriangles; j++ )
		{
			pTriangleIndices[j] = *( word* )pPtr;
			pPtr += sizeof( word );
		}

		char materialIndex = *( char* )pPtr;
		pPtr += sizeof( char );

		m_pMeshes[i].m_materialIndex = materialIndex;
		m_pMeshes[i].m_numTriangles = nTriangles;
		m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
	}



	int nMaterials = *( word* )pPtr;
	m_numMaterials = nMaterials;
	m_pMaterials = new Material[nMaterials];
	pPtr += sizeof( word );
	for ( i = 0; i < nMaterials; i++ )
	{
		MS3DMaterial *pMaterial = ( MS3DMaterial* )pPtr;
		memcpy( m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
		m_pMaterials[i].m_shininess = pMaterial->m_shininess;

		std::string tmptext = std::string(pMaterial->m_texture, 128);
		if(tmptext[0] != '\0')
		{
			if(strncmp( pMaterial->m_texture, ".\\", 2 ) == 0 )
			{
				// MS3D 1.5.x relative path
				m_pMaterials[i].m_pTextureFilename = pathTemp + tmptext;
			}
			else
			{
				// MS3D 1.4.x or earlier - absolute path
				m_pMaterials[i].m_pTextureFilename = tmptext;
				m_pMaterials[i].m_pTextureFilename = m_pMaterials[i].m_pTextureFilename.substr(m_pMaterials[i].m_pTextureFilename.find_last_of("/")+1);
				m_pMaterials[i].m_pTextureFilename = pathTemp + m_pMaterials[i].m_pTextureFilename;
			}
		}
		pPtr += sizeof( MS3DMaterial );
	}

	Reload();

	delete[] pBuffer;

	return true;
}



void MS3DModel::Render(float alpha)
{
	glTranslatef(m_TransX, m_TransY, m_TransZ);

	glRotatef(m_RotX, 1, 0, 0);
	glRotatef(m_RotY, 0, 1, 0);
	glRotatef(m_RotZ, 0, 0, 1);

	glScalef(m_ScaleX, m_ScaleY, m_ScaleZ);

	glColor4f(1,1,1,alpha);

	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );

	for ( int i = 0; i < m_numMeshes; i++ )			// draw in groups
	{
		int materialIndex = m_pMeshes[i].m_materialIndex;
		if ( materialIndex >= 0 )
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT,   m_pMaterials[materialIndex].m_ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE,   m_pMaterials[materialIndex].m_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR,  m_pMaterials[materialIndex].m_specular );
			glMaterialfv( GL_FRONT, GL_EMISSION,  m_pMaterials[materialIndex].m_emissive );
			glMaterialf(  GL_FRONT, GL_SHININESS, m_pMaterials[materialIndex].m_shininess );



			if ( m_pMaterials[materialIndex].m_texture > 0 )
			{
				glBindTexture( GL_TEXTURE_2D, m_pMaterials[materialIndex].m_texture );
				glEnable( GL_TEXTURE_2D );
			}
			else
			{
				glDisable( GL_TEXTURE_2D );
				glColor4f(m_pMaterials[materialIndex].m_ambient[0],m_pMaterials[materialIndex].m_ambient[1],m_pMaterials[materialIndex].m_ambient[2],m_pMaterials[materialIndex].m_ambient[3]);
			}
		}
		else
		{
			glDisable( GL_TEXTURE_2D );
		}

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = m_pMeshes[i].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					glNormal3fv( pTri->m_vertexNormals[k] );
					glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
					glVertex3fv( m_pVertices[index].m_location );
				}
			}
		}
		glEnd();
	}

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}


// do all check to be done when idle
int MS3DModel::Process(double tnow, float tdiff)
{
	return 1;
}

//! called to reload element when resizing screen
void MS3DModel::Reload()
{
	for ( int i = 0; i < m_numMaterials; i++ )
	{
		if ( m_pMaterials[i].m_pTextureFilename.size() > 0 )
		{
			ILuint ImgId = 0;
			ilGenImages(1, &ImgId);
			ilBindImage(ImgId);
			ilLoadImage(m_pMaterials[i].m_pTextureFilename.c_str());
			m_pMaterials[i].m_texture = ilutGLBindTexImage();
			ilDeleteImages(1, &ImgId);
		}
		else
		{
			m_pMaterials[i].m_texture = 0;
		}
	}
}


//! set model scale
void MS3DModel::SetScale(float X, float Y, float Z)
{
	m_ScaleX = X;
	m_ScaleY = Y;
	m_ScaleZ = Z;
}



//! set model translation
void MS3DModel::SetTranslation(float X, float Y, float Z)
{
	m_TransX = X;
	m_TransY = Y;
	m_TransZ = Z;
}

//! set model rotation
void MS3DModel::SetRotation(float X, float Y, float Z)
{
	m_RotX = X;
	m_RotY = Y;
	m_RotZ = Z;
}



