#include "stdafx.h"
#include "DP.h"


#define GLH_EXT_SINGLE_FILE
#include <glh/glh_extensions.h>
#include <glh/glh_obs.h>
#include <glh/glh_convenience.h>

#if !defined(GL_TEXTURE_RECTANGLE_NV) && defined(GL_EXT_texture_rectangle)
#  define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT
#endif

using namespace glh;

// peeling objects

static GLuint * zbuf = NULL;
static GLenum depth_format;
static tex_object_rectangle  ztex, rgba_layer[MAX_LAYERS];

static arb_fragment_program fp_peel, fp_peel_debug, fp_nopeel;



//-----------------------Depth Peeling 初始化設定-----------------------//
bool DP_COM::Scene_Init()
{
	GLint depth_bits;
	glGetIntegerv(GL_DEPTH_BITS, & depth_bits);

	if(depth_bits == 16)  depth_format = GL_DEPTH_COMPONENT16_ARB;
	else                  depth_format = GL_DEPTH_COMPONENT24_ARB;

	if(! glh_init_extensions("GL_ARB_vertex_program "
		"GL_ARB_fragment_program "
		"GL_ARB_fragment_program_shadow "
		"GL_ARB_depth_texture "
		"GL_ARB_shadow "))
	{
		return FALSE;
	}

	if (!glh_extension_supported("GL_EXT_texture_rectangle") && !glh_extension_supported("GL_NV_texture_rectangle"))
	{
		return FALSE;
	}

	ztex.bind();
	ztex.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	ztex.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	ztex.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ztex.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ztex.parameter(GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
	ztex.parameter(GL_TEXTURE_COMPARE_FUNC_ARB, GL_GREATER);



	LPCSTR pStr = (LPCSTR)glGetString(GL_VENDOR);

	const char peel_fp_nvdia[] = 
		"!!ARBfp1.0\n"
		"OPTION ARB_fragment_program_shadow;\n"
		"TEMP R0;\n"
		"MOV R0, fragment.position;\n"
		"TEX R0.x, R0, texture[0], SHADOWRECT;\n" 
		"ADD R0.x, R0.x, -0.5;\n"           
		"KIL R0.x;\n"                       
		"MOV result.color, fragment.color;\n"
		"END\n";

	const char peel_fp_ati[] = 
		"!!ARBfp1.0\n"
		"PARAM c[1] = { { 0.0001 } };\n"
		"TEMP R0;\n"
		"TEX R0.x, fragment.position, texture[0], RECT;\n"
		"ADD R0.x, R0, c[0];\n"
		"SLT R0.x, fragment.position.z, R0;\n"
		"MOV result.color, fragment.color.primary;\n"
		"KIL -R0.x;\n"
		"END\n";


	fp_peel.bind();

	if ( strnicmp( pStr, "ATI", 3 ) == 0 ) {			// ATI render card
		cout<<"ATI Card"<<endl;
		fp_peel.load( peel_fp_ati );
	} 
	else 
	{
		cout<<"Nvidia Card"<<endl;
		fp_peel.load( peel_fp_nvdia ); 
	}

	const char peel_fp_debug[] = 
		"!!ARBfp1.0\n"
		"PARAM alpha = program.env[0];\n"
		"TEX result.color, fragment.texcoord, texture[0], RECT;\n"
		"MOV result.color.w, alpha.w;\n"
		"END\n";

	fp_peel_debug.bind();
	fp_peel_debug.load(peel_fp_debug);

	return TRUE;
}

//---------------Texture Buffer 初始化---------------//
void DP_COM::Set_BufferObj( int scene_width,  int scene_height )
{
	ztex.bind();
	zbuf = new GLuint [ scene_width * scene_height ];

	glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_DEPTH_COMPONENT, scene_width, scene_height, 0, 
		GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, zbuf);


	for(int i=0; i < MAX_LAYERS; i++)
	{
		rgba_layer[i].bind();
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, scene_width, scene_height, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, zbuf);
		rgba_layer[i].parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		rgba_layer[i].parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	}
	delete [] zbuf;

	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, 0.6, 0.6, 0.6, 0.6);

	ClearBuffer();
	m_pZBuffer = new float[scene_width * scene_height * MAX_LAYERS];
	m_ValidBuffer = new bool [scene_width * scene_height];
}

//-------------將ZBuffer清空，及VailBuffer清空-------------//
void DP_COM::ClearBuffer()
{
	if (m_pZBuffer != NULL) {
		delete [] m_pZBuffer;
		m_pZBuffer = NULL;
	}

	if (m_ValidBuffer != NULL) {
		delete [] m_ValidBuffer;
		m_ValidBuffer = NULL;
	}
}


//-------------穿刺一層-------------//
void DP_COM::Peeling_layer( int scene_width, int scene_height, int layer, Tri_Mesh* mesh,  GLCamera camera, xform xf)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	ztex.bind();  
	if (layer > 0) {
		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		fp_peel.bind();
	}

	point center;
	center[0] = 0.0 ;
	center[1] = 0.0 ;
	center[2] = 0.0 ;
	camera.setupGL( xf * center, 1.0 ) ;


	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd((double *)xf);

	glDisable(GL_CULL_FACE);
	
	mesh->Render_Solid();
	glEnable(GL_CULL_FACE);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	glPopMatrix();


	ztex.bind();
	glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, scene_width, scene_height);
	glFinish();
	// copy the RGBA of the layer
	rgba_layer[layer].bind();
	glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 0, 0, scene_width, scene_height);

	glReadPixels(
		0, 0,
		scene_width, scene_height, 
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		& m_pZBuffer[scene_width * scene_height * layer]
	);
	glDisable(GL_DEPTH_TEST);
}