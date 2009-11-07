#include "Graphics.h"

Graphics::Graphics()
{
	pD3D = NULL;
	pd3dDevice = NULL;
	g_pVB = NULL;
	px = py = NULL;
	c_spr = NULL;
}

Graphics::~Graphics()
{
	if(px != NULL)
		delete [] px;
	if(py != NULL)
		delete [] py;
	if(c_spr != NULL)
		delete [] c_spr;
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
	int i = 0;
	int j = 0; //index variables
	ifstream fin;
	char fname[MAXCHARSIZE];
	char check;		//checks for input deliminator
	int lvl = prog/3;
	int sublvl = prog%3;
	Sprite tempSprite;
	//initialize to null
	while(i < MAXCHARSIZE)
	{
		tempSprite.filename[i] = '\0';
		fname[i] = '\0';
		i++;
	}
	i = 0; //reset i

//----------------------
//it is safer to use sprintf_s in this case so that i can specify the
//size of the array so it doesn't try to store it in safe memory
//----------------------
	sprintf_s(fname, (size_t)MAXCHARSIZE, "./lvl%isprites/load%i-%i.txt", lvl, lvl, sublvl);
	fin.open(fname);

	if(!lvlSprites.empty())
		lvlSprites.erase(lvlSprites.begin(), lvlSprites.end());

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
			j++;
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
		lvlSprites.push_back(tempSprite);
		i++;
	}
	i = 0;
	fin.close();
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
	ifstream fin;
	char fname[MAXCHARSIZE];
	unsigned int i = 0;
	int lvl = prog/3;
	int sublvl = prog%3;
	CUSTOMVERTEX gVert[MAXSPRITESPERSUBLVL*4];
	

	sprintf_s(fname, MAXCHARSIZE, "./lvl%isprites/render%i-%i.txt", lvl, lvl, sublvl);
	fin.open(fname);

	fin >> spritesRend;
	fin.ignore();

	if(px != NULL)
		delete [] px;
	if(py != NULL)
		delete [] py;
	if(c_spr != NULL)
		delete [] c_spr;
	px = new float[spritesRend];
	py = new float[spritesRend];
	c_spr = new char[spritesRend];

	for(int j = 0; j < spritesRend; j++)
	{
		fin >> c_spr[j];
		//px/py will be used later in drawlvlVB for translation
		fin >> px[j];
		fin >> py[j];

		for(i = 0; i < lvlSprites.size(); i++)
		{
			if(lvlSprites[i].s == c_spr[j])
				break;
		}
		//set up the vertices
		gVert[j*4].x = 0.0f - (float(lvlSprites[i].width)/2.0f)/2.0f;
		gVert[j*4].y = float(lvlSprites[i].height)/2.0f/2.0f;
		gVert[j*4].z = gVert[j*4].tu = gVert[j*4].tv = 0.0f;

		gVert[j*4+1].x = (float(lvlSprites[i].width)/2.0f)/2.0f;
		gVert[j*4+1].y = (float(lvlSprites[i].height)/2.0f)/2.0f;
		gVert[j*4+1].z = 0.0f;
		gVert[j*4+1].tu = 1.0f;
		gVert[j*4+1].tv = 0.0f;

		gVert[j*4+2].x = 0.0f - (float(lvlSprites[i].width)/2.0f)/2.0f;
		gVert[j*4+2].y = 0.0f - (float(lvlSprites[i].height)/2.0f)/2.0f;
		gVert[j*4+2].z = gVert[j*4+2].tu = 0.0f;
		gVert[j*4+2].tv = 1.0f;

		gVert[j*4+3].x = (float(lvlSprites[i].width)/2.0f)/2.0f;
		gVert[j*4+3].y = 0 - (float(lvlSprites[i].height)/2.0f)/2.0f;
		gVert[j*4+3].z = 0.0f;
		gVert[j*4+3].tu = gVert[j*4+3].tv = 1.0f;

		i = 0;
	}
	fin.close();

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

	// Set the vertex stream
	pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	for(int i = 0; i < spritesRend; i++)
	{
		//set the transform matrices
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixTranslation(&matTrans, px[i], py[i], 0.0f);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//apply texture
		for(unsigned int j = 0; j < lvlSprites.size(); j++)
		{
			if(lvlSprites[j].s == c_spr[i])
			{
				pd3dDevice->SetTexture(0, lvlSprites[j].g_pTexture);
				break;
			}
		}
		//Draw the trianglestrips that make up the sprite
		pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP,  i*4, 2 );
	}
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