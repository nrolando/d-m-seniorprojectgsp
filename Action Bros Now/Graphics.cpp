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
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
        return false;
    }

	//create the sprite
	HRESULT hr=D3DXCreateSprite(pd3dDevice,&gSprite);
	if (FAILED(hr))
		return false;

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

//load texture for Player, no width and height 
LPDIRECT3DTEXTURE9 Graphics::getTextureFromBitmap(char *filename)
{
	LPDIRECT3DTEXTURE9 texture;

	D3DXCreateTextureFromFileEx(pd3dDevice, filename, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0xFFFFFFFF,NULL,NULL,&texture);

	return texture;
}

//load sprite vector with the bitmap and set dimensions
LPDIRECT3DTEXTURE9 Graphics::getTextureFromBitmap(char *filename, int &w, int &h)
{
	HRESULT hResult;
	LPDIRECT3DTEXTURE9 texture;
	D3DXIMAGE_INFO imageInfo;

	// Get the width and height info from this bitmap
	hResult = D3DXGetImageInfoFromFile(filename, &imageInfo);
	if FAILED (hResult)
		return NULL;
//should always be a 50x50, but not using magic #'s
	w = imageInfo.Width;
	h = imageInfo.Height;

	D3DXCreateTextureFromFileEx(pd3dDevice, filename, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0xFFFFFFFF,NULL,NULL,&texture);

	return texture;
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

//this loads the entity (player and enemy) spritesheet continer. this is called once
bool Graphics::loadEntityCont()
{
	std::ifstream fin;
	std::ifstream inFile;
	char fname[MAXCHARSIZE];
	spriteSheet SS;

	if(!spriteContainer::getInstance()->EC_isEmpty())
		spriteContainer::getInstance()->EC_clearVec();

	sprintf_s(fname, (size_t)MAXCHARSIZE, "./playerSS/loadSpr.txt");

	fin.open(fname);
	if(!fin.is_open())
		return false;

//load the player sheets
	fin.getline(SS.sheetName, MAXCHARSIZE, '\n');
	while(!fin.eof())
	{

		D3DXCreateTextureFromFileEx(pd3dDevice,SS.sheetName,
										D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
										D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
										D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,
										0xFFFFFFFF,NULL,NULL,&SS.gTexture);

		spriteContainer::getInstance()->EC_push(SS);

		fin.getline(SS.sheetName, MAXCHARSIZE, '\n');
	}
	fin.close();

//get ready to load the enemy sheets
	sprintf_s(fname, (size_t)MAXCHARSIZE, "./enemySprites/load.txt");
	inFile.open(fname);
	if(!inFile.is_open())
		return false;

	//load the enemy sheets
	inFile.getline(SS.sheetName, MAXCHARSIZE, '#');
	while(!inFile.eof())
	{
		inFile >> SS.key;
		inFile.ignore();

		D3DXCreateTextureFromFileEx(pd3dDevice,SS.sheetName,
										D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
										D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
										D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,
										0xFFFFFFFF,NULL,NULL,&SS.gTexture);
		spriteContainer::getInstance()->EC_push(SS);

		inFile.getline(SS.sheetName, MAXCHARSIZE, '#');
	}
	inFile.close();
	return true;
}

//this clears and loads the sprite container for the current lvl
//this is called for every new level (not sublevel)
bool Graphics::loadSpriteCont(int prog)
{
	int lvl = prog/3;
	char fname[MAXCHARSIZE];
	std::ifstream fin;
	spriteSheet tempSS;

	if(!spriteContainer::getInstance()->isEmpty())
		spriteContainer::getInstance()->clearVec();

	sprintf_s(fname, (size_t)MAXCHARSIZE, "./lvlSprites/lvl%isprites/load%i.txt", lvl, lvl);

	fin.open(fname);
	if(!fin.is_open())
		return false;

	/*	READ IN .TXT ASCII TILES AND POSITION		*/
	fin.getline(tempSS.sheetName, MAXCHARSIZE, '#');
	while(!fin.eof())
	{
		fin.get(tempSS.key);
		fin.ignore();
		D3DXCreateTextureFromFileEx(pd3dDevice,tempSS.sheetName,
										D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
										D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
										D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,
										0xFFFFFFFF,NULL,NULL,&tempSS.gTexture);
		spriteContainer::getInstance()->push(tempSS);
		fin.getline(tempSS.sheetName, MAXCHARSIZE, '#');
	}
	fin.close();
	return true;
}
/*
bool Graphics::loadLvlFromFile(int prog)
{
	register unsigned int i = 0; int j = 0; int index = 0; //index variables
	int w = 0; int h = 0;
	std::ifstream fin, finput;
	char fname[MAXCHARSIZE];
	char check;		//checks for input deliminator and sprite match
	int lvl = prog/3;
	int sublvl = prog%3;
	//variables for Sprites
	SpriteRend tempSR;
	Sprite tempSprite;

	//set camPos to start of level. lvl dimensions are always 3000x1000
	camPos.x = -1180.0f; camPos.y = 0.0f; camPos.z = 0.0f;
	
//clear the vectors
	if(!spriteContainer::getInstance()->isEmpty())
		spriteContainer::getInstance()->clearVec();
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

/*
	//set sampler states, seems to work fine with or without
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
*/
/*
	//load texture file name into c-string
	fin.get(check);
	while(!fin.eof())
	{
		while(j < MAXCHARSIZE && check != '#')
		{
			tempSprite.filename[j] = check;
			fin.get(check);
			++j;
		}
		j = 0;

		fin >> tempSprite.s;
		fin.ignore();

		//load image info from file (pull w&h even tho it should always be 50x50
		tempSprite.Texture = getTextureFromBitmap(tempSprite.filename, w, h);
		tempSprite.width = w;
		tempSprite.height = h;
		spriteContainer::getInstance()->push(tempSprite);

		while(j < MAXCHARSIZE)
		{
			tempSprite.filename[j] = '\0';
			++j;
		}
		j = 0;

		++i;
		//load texture file name into c-string
		fin.get(check);
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
			//set tile's ptr its corresponding texture in the container
				for(unsigned int k = 0; k < spriteContainer::getInstance()->size(); ++k)
				{
					if(spriteContainer::getInstance()->getElem(k)->s == check)
						tempSR.ptr = spriteContainer::getInstance()->getElem(k);
				}
			}

			//do x,y calculation here: i think: if i < 30 -25 else +25; if j < 10 -25 else +25
			tempSR.pos.x = -1475.0f + i * 50.0f;
			tempSR.pos.y = 475.0f - j * 50.0f;
			tempSR.pos.z = 0.0f;
			lvlSprites.push_back(tempSR);

			++i;
		}
	}
	i = j = 0;
	
	finput.close();
	return true;
}
*/
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

void Graphics::drawLvl(std::vector<BaseGameEntity*> enemyEntSprites, eSprInfo playerSprite,
					   std::vector<Tile> uberTiles, int sublvl)
{
	RECT src;
	D3DXVECTOR3 l_pos;

	gSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//draw lvl
	src.left = LONG((camPos.x - SCREEN_WIDTH/2.0f) + 1500.0f);
	src.right = src.left + SCREEN_WIDTH;
	src.top = LONG(500.0f - (camPos.y + SCREEN_HEIGHT/2.0f));
	src.bottom = src.top + SCREEN_HEIGHT;
	//the spriteCont must always have the first 3 elements the 3 sublvls for that level
	gSprite->Draw(spriteContainer::getInstance()->getElem(sublvl)->gTexture, &src, NULL, NULL, 0xFFFFFFFF);
	

	//draw the tiles
	for(unsigned int i = 1; i < uberTiles.size(); i++)
	{
		if(uberTiles[i].ptr != NULL)
		{
			if((uberTiles[i].pos.x - uberTiles[i].w/2.0f) < (camPos.x + SCREEN_WIDTH/2.0f) &&
				(uberTiles[i].pos.x + uberTiles[i].w/2.0f) > (camPos.x - SCREEN_WIDTH/2.0f))
			{
				if((uberTiles[i].pos.y + uberTiles[i].h/2.0f) > (camPos.y - SCREEN_HEIGHT/2.0f) &&
					(uberTiles[i].pos.y - uberTiles[i].h/2.0f) < (camPos.y + SCREEN_HEIGHT/2.0f))
				{
					l_pos.x = float((uberTiles[i].pos.x - uberTiles[i].w/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
					if(l_pos.x < 0)
					{
						uberTiles[i].src.left = 0 - l_pos.x;
						l_pos.x = 0;
					}

					l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - (uberTiles[i].pos.y + uberTiles[i].h/2.0f));
					if(l_pos.y < 0)
					{
						uberTiles[i].src.top = 0 - l_pos.y;
						l_pos.y = 0;
					}
					
					l_pos.z = 0.0f;

					gSprite->Draw(uberTiles[i].ptr->gTexture, &uberTiles[i].src, NULL, &l_pos, 0xFFFFFFFF);
				}
			}
		}
	}
//translate the player's world coordinates into screen coord, then render it
	l_pos.x = float((playerSprite.POS.x - playerSprite.width/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
	l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - (playerSprite.POS.y + playerSprite.height/2.0f));
	l_pos.z = 0.0f;
	gSprite->Draw(playerSprite.ss_ptr->gTexture, &playerSprite.drawRect, NULL, &l_pos, 0xFFFFFFFF);


	//draw the entities
//TEST IF SPRITE IS WITHIN VIEWPORT, IF NOT, DON'T DRAW!
	for(unsigned int i = 0; i < enemyEntSprites.size(); i++)
	{
		if((enemyEntSprites[i]->getPos().x - enemyEntSprites[i]->getWidth()/2.0f) < (camPos.x + SCREEN_WIDTH/2.0f) &&
			(enemyEntSprites[i]->getPos().x + enemyEntSprites[i]->getWidth()/2.0f) > (camPos.x - SCREEN_WIDTH/2.0f))
		{
			if((enemyEntSprites[i]->getPos().y + enemyEntSprites[i]->getHeight()/2.0f) > (camPos.y - SCREEN_HEIGHT/2.0f) &&
				(enemyEntSprites[i]->getPos().y - enemyEntSprites[i]->getHeight()/2.0f) < (camPos.y + SCREEN_HEIGHT/2.0f))
			{
				l_pos.x = float((enemyEntSprites[i]->getPos().x - enemyEntSprites[i]->getWidth()/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
		
				l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - (enemyEntSprites[i]->getPos().y + enemyEntSprites[i]->getHeight()/2.0f));
				
				l_pos.z = 0.0f;
				gSprite->Draw(enemyEntSprites[i]->getSSPtr()->gTexture, &enemyEntSprites[i]->getSrc(), NULL, &l_pos, 0xFFFFFFFF);
			}
		}
	}
	gSprite->End();
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

	sprintf_s(display, (size_t)MAXCHARSIZE, "elapsed time: %i  CPS: %i", _time, CLOCKS_PER_SEC);

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