#pragma once
#ifndef _DP_H_
#define _DP_H_

#include "GUA_OM.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLCamera.h"
#include "XForm.h"
#define MAX_LAYERS 10

class DP_COM
{

public:
	float*	   m_pZBuffer      ;    //�x�s�C�@���h��ZBuffer��       
	bool*     m_ValidBuffer ;    //��Pixel�O�_�s�bmesh���P�_buffer�A�P�_�[�t��

	//---------------function---------------//
	bool Scene_Init( );
	void Set_BufferObj(  int, int  );
	void ClearBuffer( );
	void Peeling_layer(  int, int, int,  Tri_Mesh*, GLCamera, xform) ;

protected:
private:

};


#endif