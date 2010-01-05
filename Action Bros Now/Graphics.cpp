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

bool Graphics::initD3D(HWND hwnd)
{

	HRESULT hr;

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

	hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      & d3dpp, &pd3dDevice );

	//create direct3D device
    if( FAILED(hr))
    {
        pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      & d3dpp, &pd3dDevice );
    }

	//create the sprite
	hr=D3DXCreateSprite(pd3dDevice,&gSprite);
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
	l_pos.x = 0.0f;
	l_pos.y = 0.0f;
	l_pos.z = 0.9f;	//.9 is the farthest away, sprite doesn't draw at > 1.0
	//the spriteCont must always have the first 3 elements the 3 sublvls for that level
	gSprite->Draw(spriteContainer::getInstance()->getElem(sublvl)->gTexture, &src, NULL, &l_pos, 0xFFFFFFFF);
	

	//draw the tiles
	for(unsigned int i = 0; i < uberTiles.size(); i++)
	{
		if(uberTiles[i].ptr != NULL)
		{
			if(uberTiles[i].pos.x < (camPos.x + SCREEN_WIDTH/2.0f) &&
				(uberTiles[i].pos.x + uberTiles[i].w) > (camPos.x - SCREEN_WIDTH/2.0f))
			{
				if(uberTiles[i].pos.y > (camPos.y - SCREEN_HEIGHT/2.0f) &&
					(uberTiles[i].pos.y - uberTiles[i].h) < (camPos.y + SCREEN_HEIGHT/2.0f))
				{
					l_pos.x = float(uberTiles[i].pos.x - (camPos.x - SCREEN_WIDTH/2.0f));
					l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - uberTiles[i].pos.y);
					l_pos.z = ((uberTiles[i].pos.y - uberTiles[i].h) + 500.0f)/1000.0f;

					gSprite->Draw(uberTiles[i].ptr->gTexture, &uberTiles[i].src, NULL, &l_pos, 0xFFFFFFFF);
				}
			}
		}
	}
//translate the player's world coordinates into screen coord, then render it
	l_pos.x = float(playerSprite.POS.x - (camPos.x - SCREEN_WIDTH/2.0f));
	l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - playerSprite.POS.y);
	l_pos.z = ((playerSprite.POS.y - playerSprite.height) + 500.0f)/1000.0f;
	gSprite->Draw(playerSprite.ss_ptr->gTexture, &playerSprite.drawRect, NULL, &l_pos, 0xFFFFFFFF);

    //Draw for Debug mode here
	if(DEBUGMODE)
	{
		D3DXVECTOR3 b_pos;
		b_pos.x = l_pos.x;  
		b_pos.y = l_pos.y + 48; //offset y position so that the hitbox is with sprite
		b_pos.z = l_pos.z;

		D3DXCreateTextureFromFile(pd3dDevice, "./playerSS/hBox.png", &playerSprite.ss_ptr->hBoxTexture);
		D3DXCreateTextureFromFile(pd3dDevice, "./playerSS/tBox.png", &playerSprite.ss_ptr->tBoxTexture);
		gSprite->Draw(playerSprite.ss_ptr->hBoxTexture, &playerSprite.hitBox, NULL, &b_pos, 0xFFFFFFFF);
		gSprite->Draw(playerSprite.ss_ptr->tBoxTexture, &playerSprite.threatBox, NULL, &b_pos, 0xFFFFFFFF);
	}

	//draw the entities
//TEST IF SPRITE IS WITHIN VIEWPORT, IF NOT, DON'T DRAW!
	for(unsigned int i = 0; i < enemyEntSprites.size(); i++)
	{
		if(enemyEntSprites[i]->getPos().x < (camPos.x + SCREEN_WIDTH/2.0f) &&
			(enemyEntSprites[i]->getPos().x + enemyEntSprites[i]->getWidth()) > (camPos.x - SCREEN_WIDTH/2.0f))
		{
			if(enemyEntSprites[i]->getPos().y > (camPos.y - SCREEN_HEIGHT/2.0f) &&
				(enemyEntSprites[i]->getPos().y - enemyEntSprites[i]->getHeight()) < (camPos.y + SCREEN_HEIGHT/2.0f))
			{
				l_pos.x = float(enemyEntSprites[i]->getPos().x - (camPos.x - SCREEN_WIDTH/2.0f));
				l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - enemyEntSprites[i]->getPos().y);
				l_pos.z = ((enemyEntSprites[i]->getPos().y - enemyEntSprites[i]->getHeight()) + 500.0f)/1000.0f;;
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

	sprintf_s(display, "Milliseconds: %i  CPS: %i", _time, CLOCKS_PER_SEC);

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

void Graphics::updateCamera(eSprInfo si)
{
	static float xRange = (SCREEN_WIDTH/6.0f);
	static float yRange = (SCREEN_HEIGHT/6.0f);

	if(si.POS.x + si.width/2.0f < camPos.x - xRange)
		camPos.x = si.POS.x + si.width/2.0f + xRange;
	if(si.POS.x + si.width/2.0f > camPos.x + xRange)
		camPos.x = si.POS.x + si.width/2.0f - xRange;
	if(si.POS.y - si.height/2.0f < camPos.y - yRange)
		camPos.y = si.POS.y - si.height/2.0f + yRange;
	if(si.POS.y - si.height/2.0f > camPos.y + yRange)
		camPos.y = si.POS.y - si.height/2.0f - yRange;

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