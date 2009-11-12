#include "Graphics.h"
//#include <vector>

Graphics::Graphics()
{
	pD3D = NULL;
	pd3dDevice = NULL;
	g_pVB = NULL;
	m_font = NULL;
}

Graphics::~Graphics()
{
	
}

bool Graphics::initD3D(HWND &hwnd)
{
	//create direct3D object
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferWidth  = SCREEN_WIDTH;
	d3dpp.hDeviceWindow    = hwnd;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//create direct3D device
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
        return false;
    }

	//*********** Set the default render states**************
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	//set alpha state
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ALPHAREF, DWORD(8));
	pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXCreateFont( pd3dDevice,
					20,				//SIZE
					0, 
					FW_BOLD,		//weight
					0, 
					FALSE,			//Italic
					DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, 
					DEFAULT_QUALITY, 
					DEFAULT_PITCH | FF_DONTCARE, 
					TEXT("Arial"), 
					&m_font );

	//create and set Camera
	createCamera(1.0f, 5000.0f);
	moveCamera(D3DXVECTOR3(0.0f, 0.0f, -1000.0f));
	pointAndSetCamera(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return true;
}

void Graphics::_shutdown()
{
	if( pd3dDevice != NULL) 
	{
        pd3dDevice->Release();
		pd3dDevice = NULL;
	}
    if( pD3D != NULL)
	{
        pD3D->Release();
		pD3D = NULL;
	}
	if( g_pVB != NULL)
	{
		g_pVB->Release();
		g_pVB = NULL;
	}
	if(m_font != NULL)
	{
		m_font->Release();
		m_font = NULL;
	}
}

void Graphics::BeginRender()
{
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,200,0), 1.0f, 0 );
	pd3dDevice->BeginScene();
}

void Graphics::EndRender()
{
	pd3dDevice->EndScene();
	// Present the backbuffer contents to the display
	pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

bool Graphics::loadLvlFromFile(int prog)
{
	register unsigned int i = 0;
	int j = 0; //index variables
	ifstream fin, finput;
	char fname[MAXCHARSIZE];
	char check;		//checks for input deliminator and sprite match
	int lvl = prog/3;
	int sublvl = prog%3;
	SpriteRend tempSR;
	Sprite tempSprite;
	
//clear the vectors
	if(!spriteCont.empty())
		spriteCont.clear();
	if(!lvlSprites.empty())
		lvlSprites.clear();

//initialize
	while(i < MAXCHARSIZE)
	{
		tempSprite.filename[i] = '\0';
		fname[i] = '\0';
		++i;
	}
	i = 0; //reset i
	

//----------------------
//it is safer to use sprintf_s in this case so that i can specify the
//size of the array so it doesn't try to store it in safe memory
//----------------------
	sprintf_s(fname, (size_t)MAXCHARSIZE, "./lvl%isprites/load%i-%i.txt", lvl, lvl, sublvl);
	fin.open(fname);
	if(!fin.is_open())
		return false;

	//set sampler states, seems to work fine with or without
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	while(!fin.eof())
	{
		//load texture file name into c-string
		fin.get(check);
		while(j < MAXCHARSIZE && check != '#')
		{
			tempSprite.filename[j] = check;
			fin.get(check);
			++j;
		}
		tempSprite.filename[j] = '\0';	//null ends the c-string
		j = 0;	//reset j

		fin >> tempSprite.s;
		fin >> tempSprite.width;
		fin >> tempSprite.height;
		fin.ignore();

		//load texture from file
		D3DXCreateTextureFromFile	( pd3dDevice, tempSprite.filename, 
									&(tempSprite.g_pTexture) );
		spriteCont.push_back(tempSprite);
		++i;
	}
	i = 0;
	fin.close();

//*****apply position and textures to our sprites*****
	//reset fname
	while(i < MAXCHARSIZE)
	{
		fname[i] = '\0';
		++i;
	}
	i = 0;
	sprintf_s(fname, MAXCHARSIZE, "./lvl%isprites/render%i-%i.txt", lvl, lvl, sublvl);
	finput.open(fname);
	if(!finput.is_open())
		return false;

	while(!finput.eof())
	{
		finput >> check;
		finput >> tempSR.x;
		finput >> tempSR.y;

		for(i; i < spriteCont.size(); ++i)
		{
			if(spriteCont[i].s == check)
				tempSR.index = i;
		}
		i = 0;
		lvlSprites.push_back(tempSR);
	}

	if(FAILED(SetupLvlVB(prog)))
		return false;
	else
		return true;
}

/****************************************/
//creates the vertex buffer for setupVB()
/****************************************/
LPDIRECT3DVERTEXBUFFER9 Graphics::createVertexBuffer(int size, DWORD usage)
{
	HRESULT hr;
	LPDIRECT3DVERTEXBUFFER9 buffer;

    // Create the vertex buffer.
    hr = pd3dDevice->CreateVertexBuffer( size,
                                         0, 
										 usage,
                                         D3DPOOL_DEFAULT, 
										 &buffer, 
										 NULL );
	if FAILED ( hr)
        return NULL;
    
	return buffer;
}

HRESULT Graphics::SetupLvlVB(int prog)
{
	HRESULT hr;	
	CUSTOMVERTEX gVert[MAXSPRITESPERSUBLVL*4];

	for(unsigned int j = 0; j < spriteCont.size(); j++)
	{
		//set up the vertices
		gVert[j*4].x = 0.0f - (float(spriteCont[j].width)/2.0f)/2.0f;
		gVert[j*4].y = float(spriteCont[j].height)/2.0f/2.0f;
		gVert[j*4].z = gVert[j*4].tu = gVert[j*4].tv = 0.0f;

		gVert[j*4+1].x = (float(spriteCont[j].width)/2.0f)/2.0f;
		gVert[j*4+1].y = (float(spriteCont[j].height)/2.0f)/2.0f;
		gVert[j*4+1].z = 0.0f;
		gVert[j*4+1].tu = 1.0f;
		gVert[j*4+1].tv = 0.0f;

		gVert[j*4+2].x = 0.0f - (float(spriteCont[j].width)/2.0f)/2.0f;
		gVert[j*4+2].y = 0.0f - (float(spriteCont[j].height)/2.0f)/2.0f;
		gVert[j*4+2].z = gVert[j*4+2].tu = 0.0f;
		gVert[j*4+2].tv = 1.0f;

		gVert[j*4+3].x = (float(spriteCont[j].width)/2.0f)/2.0f;
		gVert[j*4+3].y = 0 - (float(spriteCont[j].height)/2.0f)/2.0f;
		gVert[j*4+3].z = 0.0f;
		gVert[j*4+3].tu = gVert[j*4+3].tv = 1.0f;
	}

	//create the vertex buffer
	g_pVB = createVertexBuffer(sizeof(gVert)*sizeof(CUSTOMVERTEX), D3DFVF_CUSTOMVERTEX);
	//fill the vertex buffer
	VOID* pVertices;
	hr = g_pVB->Lock(0, sizeof(gVert), (void**)&pVertices, 0);
	if(FAILED(hr))
		return E_FAIL;
	//copy the vertices into the buffer
	memcpy(pVertices, gVert, sizeof(gVert));
	//unlock vb
	g_pVB->Unlock();

	return S_OK;
}

void Graphics::drawLvlVB()
{
	D3DXMATRIX matTrans, matWorld;

	moveCamera(cameraPosition);
	pointAndSetCamera(cameraLook);

	// Set the vertex stream
	pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	for(unsigned int i = 0; i < lvlSprites.size(); i++)
	{
		//set the transform matrices
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixTranslation(&matTrans, lvlSprites[i].x, lvlSprites[i].y, 0.0f);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//apply texture
		pd3dDevice->SetTexture(0, spriteCont[lvlSprites[i].index].g_pTexture);
		//Draw the trianglestrips that make up the sprite
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  lvlSprites[i].index*4, 2 );
	}
}

//display text to the screen
void Graphics::displayTime(clock_t _time, int y)	//elapsed time
{
	RECT rct;
	char display[MAXCHARSIZE];
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 180, 0, 0);

	rct.left = 20;
	rct.right = rct.left + 250;
	rct.top = y;
	rct.bottom = rct.top + 50;

	sprintf(display, "elapsed time: %i  CPS: %i", _time, CLOCKS_PER_SEC);

	m_font->DrawText(NULL, display, -1, &rct,
					DT_NOCLIP | DT_WORDBREAK, fontColor);
}

//	CAMERA	*****************************************************
void Graphics::createCamera(float nearClip, float farClip)
{
	//Here we specify the field of view, aspect ration and near and far clipping planes.
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, SCREEN_WIDTH/SCREEN_HEIGHT, nearClip, farClip);
    pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

}

/*************************************************************************
* moveCamera	*/
void Graphics::moveCamera(D3DXVECTOR3 vec)
{
	cameraPosition = vec;
}

void Graphics::translateCamera(D3DXVECTOR3 vec)
{
	cameraPosition += vec;
}

/*************************************************************************
* pointCamera
* points the camera a location specified by the passed vector
*************************************************************************/
void Graphics::pointAndSetCamera(D3DXVECTOR3 vec)
{
	cameraLook = vec;

	D3DXMatrixLookAtLH(&matView, &cameraPosition,		//Camera Position
                                 &cameraLook,			//Look At Position
                                 &D3DXVECTOR3(0.0f, 1.0f, 0.0f));	//Up Direction

	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}