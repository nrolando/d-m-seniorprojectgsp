#include "Graphics.h"

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

//load sprite vector with the bitmap and set dimensions
IDirect3DSurface9* Graphics::getSurfaceFromBitmap(std::string filename, int &w, int &h)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;
	D3DXIMAGE_INFO imageInfo;

	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);
	if FAILED (hResult)
		return NULL;
//should always be a 50x50, but not using magic #'s
	w = imageInfo.Width;
	h = imageInfo.Height;

	hResult = pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hResult))
		return NULL;

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return NULL;

	return surface;
}

IDirect3DSurface9* Graphics::getBackBuffer(void)
{
	IDirect3DSurface9* backbuffer = NULL;

	if (!pd3dDevice)
		return NULL;

	HRESULT hResult = pd3dDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	if (FAILED(hResult))
		return NULL;
	else
		return backbuffer;
}

void Graphics::blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect)
{
	pd3dDevice->StretchRect(srcSurface, srcRect, getBackBuffer(), destRect, D3DTEXF_NONE);
}

bool Graphics::loadLvlFromFile(int prog)
{
	register unsigned int i = 0; int j = 0; //index variables
	int w = 0; int h = 0;
	ifstream fin, finput;
	char fname[MAXCHARSIZE];
	char check;		//checks for input deliminator and sprite match
	int lvl = prog/3;
	int sublvl = prog%3;
	SpriteRend tempSR;
	Sprite tempSprite;

	//set camPos to start of level
	camPos.x = -1180.0f; camPos.y = 0.0f; camPos.z = 0.0f;
	
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
		fin.ignore();

		//load image info from file (pull w&h even tho it should always be 50x50
		tempSprite.spriteSurf = getSurfaceFromBitmap(tempSprite.filename, w, h);
		tempSprite.width = w;
		tempSprite.height = h;
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

/*	READ IN .TXT ASCII TILES AND POSITION		*/
	while(!finput.eof())
	{
		finput >> check;

		if(check == '#')
		{
			++j;
			i = 0;
		}
		else
		{
			if(check == '.')
			{
				tempSR.ptr = NULL;
			}
			else
			{
				for(unsigned int k = 0; k < spriteCont.size(); ++k)
				{
					if(spriteCont[k].s == check)
						tempSR.ptr = &spriteCont[k];
				}
			}

			//do x,y calculation here: i think: if i < 30 -25 else +25; if j < 10 -25 else +25
			tempSR.x = -1475.0f + i * 50.0f;
			tempSR.y = 475.0f - j * 50.0f;

			lvlSprites.push_back(tempSR);
			++i;
		}
	}
	i = j = 0;
	
	finput.close();
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

//HRESULT Graphics::SetupLvlVB(int prog)
//{
//	HRESULT hr;	
//	//CUSTOMVERTEX gVert[MAXSPRITESPERSUBLVL*4];	//old way
//	CUSTOMVERTEX gVert[] =
//	{	//3000x1000
//		{-1500.0f, 500.0f, 1.0f,	0.0f, 0.0f},
//		{ 1500.0f, 500.0f, 1.0f,	1.0f, 0.0f},
//		{-1500.0f, -500.0f, 1.0f,	0.0f, 1.0f},
//		{ 1500.0f, -500.0f, 1.0f,	1.0f, 1.0f},
//		//50x50
//		{-25.0f, 25.0f, 0.0f,	0.0f, 0.0f},
//		{25.0f, 25.0f, 0.0f,	1.0f, 0.0f},
//		{-25.0f, -25.0f, 0.0f,	0.0f, 1.0f},
//		{25.0f, -25.0f, 0.0f,	1.0f, 1.0f}
//	};
//
//	//create the vertex buffer
//	g_pVB = createVertexBuffer(sizeof(gVert)*sizeof(CUSTOMVERTEX), D3DFVF_CUSTOMVERTEX);
//	//fill the vertex buffer
//	VOID* pVertices;
//	hr = g_pVB->Lock(0, sizeof(gVert), (void**)&pVertices, 0);
//	if(FAILED(hr))
//		return E_FAIL;
//	//copy the vertices into the buffer
//	memcpy(pVertices, gVert, sizeof(gVert));
//	//unlock vb
//	g_pVB->Unlock();
//
//	return S_OK;
//}

void Graphics::drawLvl()
{
	RECT dest, src;

	//draw lvl
	src.left = LONG((camPos.x - SCREEN_WIDTH/2.0f) + 1500.0f);
	src.right = src.left + SCREEN_WIDTH;
	src.top = LONG(500.0f - (camPos.y + SCREEN_HEIGHT/2.0f));
	src.bottom = src.top + SCREEN_HEIGHT;
	blitToSurface(spriteCont[0].spriteSurf, &src, NULL);

	//draw the tiles
	for(unsigned int i = 0; i < lvlSprites.size(); i++)
	{
		if(lvlSprites[i].ptr != NULL)
		{
			if((lvlSprites[i].x - lvlSprites[i].ptr->width/2.0f) < (camPos.x + SCREEN_WIDTH/2.0f) &&
				(lvlSprites[i].x + lvlSprites[i].ptr->width/2.0f) > (camPos.x - SCREEN_WIDTH/2.0f))
			{
				if((lvlSprites[i].y + lvlSprites[i].ptr->height/2.0f) > (camPos.y - SCREEN_HEIGHT/2.0f) &&
					(lvlSprites[i].y - lvlSprites[i].ptr->height/2.0f) < (camPos.y + SCREEN_HEIGHT/2.0f))
				{
					dest.left = LONG((lvlSprites[i].x - lvlSprites[i].ptr->width/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
					if(dest.left < 0)
						dest.left = 0;
					dest.right = dest.left + lvlSprites[i].ptr->width;
					if(dest.right > SCREEN_WIDTH)
						dest.right = SCREEN_WIDTH;
					dest.top = LONG((camPos.y + SCREEN_HEIGHT/2.0f) - (lvlSprites[i].y + lvlSprites[i].ptr->height/2.0f));
					if(dest.top < 0)
						dest.top = 0;
					dest.bottom = dest.top + lvlSprites[i].ptr->height;
					if(dest.bottom > SCREEN_HEIGHT)
						dest.bottom = SCREEN_HEIGHT;
					blitToSurface(lvlSprites[i].ptr->spriteSurf, NULL, &dest);
				}
			}
		}
	}
}

//void Graphics::drawLvlVB()
//{
//	int count = 0;
//	D3DXMATRIX matTrans, matWorld;
//
//	/*moveCamera(cameraPosition);
//	pointAndSetCamera(cameraLook);*/
//
//	// Set the vertex stream
//	pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
//	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
//
//	//draw the level background
//	D3DXMatrixIdentity(&matWorld);
//	pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
//	pd3dDevice->SetTexture(0, spriteCont[0].g_pTexture);
//	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	
//	//draw the tiles
//	for(unsigned int i = 0; i < lvlSprites.size(); i++)
//	{
//		if(lvlSprites[i].ptr != NULL)
//		{
//			//set the transform matrices
//			D3DXMatrixIdentity(&matWorld);
//			D3DXMatrixIdentity(&matTrans);
//			D3DXMatrixTranslation(&matTrans,lvlSprites[i].x,
//											lvlSprites[i].y,
//											(0.0f));
//			D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
//			pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
//			//apply texture
//			pd3dDevice->SetTexture(0, lvlSprites[i].ptr->g_pTexture);
//			//Draw the trianglestrips that make up the sprite
//			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 4, 2 );
//			count++;
//		}
//	}
//	displayTime(clock_t(count), 40);
//}

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

/*************************************************************************
* move the simulated Camera!
the pixel rect that is grabbed from the level is based on this simulated
camera's position*/
void Graphics::moveCamera(D3DXVECTOR3 vec)
{
	camPos += vec;
	//dont allow camera to go off screen
	if(camPos.x < (-1500.0f + SCREEN_WIDTH/2.0f))
		camPos.x = (-1500.0f + SCREEN_WIDTH/2.0f);
	else if(camPos.x > (1500.0f - SCREEN_WIDTH/2.0f))
		camPos.x = (1500.0f - SCREEN_WIDTH/2.0f);
	if(camPos.y > (500.0f - SCREEN_HEIGHT/2.0f))
		camPos.y = (500.0f - SCREEN_HEIGHT/2.0f);
	else if(camPos.y < (-500.0f + SCREEN_HEIGHT/2.0f))
		camPos.y = (-500.0f + SCREEN_HEIGHT/2.0f);
}