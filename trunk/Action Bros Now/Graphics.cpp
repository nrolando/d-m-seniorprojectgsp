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
       hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      & d3dpp, &pd3dDevice );
    }
	if( FAILED(hr))
		return false;

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
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	pd3dDevice->BeginScene();
	gSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::BeginSplashRender()
{
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
	pd3dDevice->BeginScene();
	gSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void Graphics::EndRender()
{
	gSprite->End();
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

void Graphics::DisplayPlayerStat(int currhealth,int maxhealth,int currspecial,int maxspecial)
{
	HRESULT hr;

	D3DXVECTOR3 hBarPos  = D3DXVECTOR3(HEALTHBAR_POSX,HEALTHBAR_POSY,HEALTHBAR_POSZ),
				healPos  = D3DXVECTOR3(HEALTH_POSX,HEALTH_POSY,HEALTH_POSZ),
				sBarPos  = D3DXVECTOR3(SPECIALBAR_POSX,SPECIALBAR_POSY,SPECIALBAR_POSZ),
				specPos	 = D3DXVECTOR3(SPECIAL_POSX,SPECIAL_POSY,SPECIAL_POSZ);
	
	RECT healthBar,
		 health,
		 specialBar,
		 special;
	
	D3DXIMAGE_INFO hBar,
				   sBar,
				   sMeter,
				   hMeter;

	//this checks if the sprite image information was 
	//successfully retrieved for health and special
	//upon failure nothing will be drawn
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('h')->sheetName, &hBar);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Health Bar", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('s')->sheetName, &sBar);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Special Bar", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('S')->sheetName, &sMeter);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Special Meter", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('H')->sheetName, &hMeter);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Health Meter", "ERROR", MB_OK);}
	else
	{
		//Health and Special Bars
		healthBar.top = 0;
		healthBar.left = 0;
		healthBar.right = healthBar.left + hBar.Width;
		healthBar.bottom = healthBar.top + hBar.Height;

		specialBar.top = 0;
		specialBar.left = 0;
		specialBar.right = specialBar.left + sBar.Width;
		specialBar.bottom = specialBar.top + sBar.Height;

		//This fills the Bars
		health.top = 0;
		health.left = 0;
		health.right = long(health.left + ((float)currhealth/(float)maxhealth * hMeter.Width));
		health.bottom = health.top + hMeter.Height;

		special.top = 0;
		special.left = 0;
		special.right = long(special.left + ((float)currspecial/(float)maxspecial *  + sMeter.Width));
		special.bottom = special.top + sMeter.Height;

		gSprite->Draw(spriteContainer::getInstance()->getElemKey('h')->gTexture, &healthBar, NULL, &hBarPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('H')->gTexture, &health, NULL, &healPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('s')->gTexture, &specialBar, NULL, &sBarPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('S')->gTexture, &special, NULL, &specPos, 0xFFFFFFFF);
	}
	
}

void Graphics::DisplayEnemyHealth(int currhealth,int maxhealth)
{
	HRESULT hr;

	D3DXVECTOR3 hBarPos  = D3DXVECTOR3(ENEMY_HEALTHBAR_POSX,ENEMY_HEALTHBAR_POSY,ENEMY_HEALTHBAR_POSZ),
				healPos  = D3DXVECTOR3(ENEMY_HEALTH_POSX,ENEMY_HEALTH_POSY,ENEMY_HEALTH_POSZ);
	
	RECT healthBar,
		 health;
	
	D3DXIMAGE_INFO hBar,
				   hMeter;

	//this checks if the sprite image information was 
	//successfully retrieved for health and special
	//upon failure nothing will be drawn
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('e')->sheetName, &hBar);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Enemy Health Bar", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('E')->sheetName, &hMeter);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Enemy Special Bar", "ERROR", MB_OK);}
	else
	{
		//Health and Bar
		healthBar.top = 0;
		healthBar.left = 0;
		healthBar.right = healthBar.left + hBar.Width;
		healthBar.bottom = healthBar.top + hBar.Height;

		//This fills the Bar
		health.top = 0;
		health.left = 0;
		health.right = long(health.left + ((float)currhealth/(float)maxhealth * hMeter.Width));
		health.bottom = health.top + hMeter.Height;

		gSprite->Draw(spriteContainer::getInstance()->getElemKey('e')->gTexture, &healthBar, NULL, &hBarPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('E')->gTexture, &health, NULL, &healPos, 0xFFFFFFFF);
	}
}

void Graphics::DisplayBossStat(int currhealth,int maxhealth,int currspecial,int maxspecial)
{
	HRESULT hr;

	D3DXVECTOR3 hBarPos  = D3DXVECTOR3(BOSS_HEALTHBAR_POSX,BOSS_HEALTHBAR_POSY,BOSS_HEALTHBAR_POSZ),
				healPos  = D3DXVECTOR3(BOSS_HEALTH_POSX,BOSS_HEALTH_POSY,BOSS_HEALTH_POSZ),
				specPos	 = D3DXVECTOR3(BOSS_SPECIAL_POSX,BOSS_SPECIAL_POSY,BOSS_SPECIAL_POSZ);
	
	RECT healthBar,
		 health,
		 special;
	
	D3DXIMAGE_INFO hBar,
				   sMeter,
				   hMeter;

	//this checks if the sprite image information was 
	//successfully retrieved for health and special
	//upon failure nothing will be drawn
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('b')->sheetName, &hBar);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Health Bar", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('B')->sheetName, &hMeter);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Special Bar", "ERROR", MB_OK);}
	hr = D3DXGetImageInfoFromFile(spriteContainer::getInstance()->getElemKey('D')->sheetName, &sMeter);
	if FAILED (hr)
	{MessageBox(NULL, "Unable to display Player Special Meter", "ERROR", MB_OK);}
	else
	{
		//Health and Special Bars
		healthBar.top = 0;
		healthBar.left = 0;
		healthBar.right = healthBar.left + hBar.Width;
		healthBar.bottom = healthBar.top + hBar.Height;

		//This fills the Bars
		health.top = 0;
		health.left = 0;
		health.right = long(health.left + ((float)currhealth/(float)maxhealth * hMeter.Width));
		health.bottom = health.top + hMeter.Height;

		special.top = 0;
		special.left = 0;
		special.right = long(special.left + ((float)currspecial/(float)maxspecial *  + sMeter.Width));
		special.bottom = special.top + sMeter.Height;

		gSprite->Draw(spriteContainer::getInstance()->getElemKey('b')->gTexture, &healthBar, NULL, &hBarPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('B')->gTexture, &health, NULL, &healPos, 0xFFFFFFFF);
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('D')->gTexture, &special, NULL, &specPos, 0xFFFFFFFF);
	}
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
//If in DEBUG MODE then load HIT/THREAT boxes
	if(DEBUGMODE)
	{
		D3DXCreateTextureFromFile(pd3dDevice, "./playerSS/hBox.png", &hBoxTexture);
		D3DXCreateTextureFromFile(pd3dDevice, "./playerSS/tBox.png", &tBoxTexture);
	}

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
	std::ifstream fin, FIN;
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

	//load the screen sprites
	sprintf_s(fname, (size_t)MAXCHARSIZE, "./Screen Sprites/load.txt");

	FIN.open(fname);
	if(!FIN.is_open())
		return false;

	/*	READ IN .TXT ASCII TILES AND POSITION		*/
	FIN.getline(tempSS.sheetName, MAXCHARSIZE, '#');
	while(!FIN.eof())
	{
		FIN.get(tempSS.key);
		FIN.ignore();
		D3DXCreateTextureFromFileEx(pd3dDevice,tempSS.sheetName,
									D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
									D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
									D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,
									0xFFFFFFFF,NULL,NULL,&tempSS.gTexture);
		spriteContainer::getInstance()->push(tempSS);
		FIN.getline(tempSS.sheetName, MAXCHARSIZE, '#');
	}
	FIN.close();
	return true;
}

void Graphics::drawLvl(std::vector<BaseGameEntity*> enemyEntSprites, eSprInfo playerSprite,
					   std::vector<Tile> bTiles, std::vector<Tile> fTiles, int sublvl)
{
	//initialize local variables
	D3DXVECTOR3 b_pos;
	RECT b_src;
	RECT src;
	D3DXVECTOR3 l_pos;
	eSprInfo e_SprInfo;

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
	

	//draw the background tiles
	for(unsigned int i = 0; i < bTiles.size(); i++)
	{
		if(bTiles[i].ptr != NULL)
		{
			if(bTiles[i].pos.x < (camPos.x + SCREEN_WIDTH/2.0f) &&
				(bTiles[i].pos.x + bTiles[i].w) > (camPos.x - SCREEN_WIDTH/2.0f))
			{
				if(bTiles[i].pos.y > (camPos.y - SCREEN_HEIGHT/2.0f) &&
					(bTiles[i].pos.y - bTiles[i].h) < (camPos.y + SCREEN_HEIGHT/2.0f))
				{
					l_pos.x = float(bTiles[i].pos.x - (camPos.x - SCREEN_WIDTH/2.0f));
					l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - bTiles[i].pos.y);
					l_pos.z = 0.8f;

					gSprite->Draw(bTiles[i].ptr->gTexture, &bTiles[i].src, NULL, &l_pos, 0xFFFFFFFF);
				}
			}
		}
	}
	//draw the foreground tiles
	for(unsigned int i = 0; i < fTiles.size(); i++)
	{
		if(fTiles[i].ptr != NULL)
		{
			if(fTiles[i].pos.x < (camPos.x + SCREEN_WIDTH/2.0f) &&
				(fTiles[i].pos.x + bTiles[i].w) > (camPos.x - SCREEN_WIDTH/2.0f))
			{
				if(fTiles[i].pos.y > (camPos.y - SCREEN_HEIGHT/2.0f) &&
					(fTiles[i].pos.y - fTiles[i].h) < (camPos.y + SCREEN_HEIGHT/2.0f))
				{
					l_pos.x = float(fTiles[i].pos.x - (camPos.x - SCREEN_WIDTH/2.0f));
					l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - fTiles[i].pos.y);
					l_pos.z = 0.1f;

					gSprite->Draw(fTiles[i].ptr->gTexture, &fTiles[i].src, NULL, &l_pos, 0xFFFFFFFF);
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
		//first setup and draw hitbox
	//b_src == size of hitbox 45x80
		b_src.left = 0;
		b_src.right = 45;
		b_src.top = 0;
		b_src.bottom = 80;

		b_pos.x = l_pos.x + (playerSprite.hitBox.left - playerSprite.POS.x); 
		b_pos.y = l_pos.y + (playerSprite.POS.y - playerSprite.hitBox.top);
		b_pos.z = l_pos.z + 0.1f;
		gSprite->Draw(hBoxTexture, &b_src, NULL, &b_pos, 0xFFFFFFFF);

		//setup and draw threatbox
		b_src.left = 0;
		b_src.right = playerSprite.threatBox.right - playerSprite.threatBox.left;
		b_src.top = 0;
		b_src.bottom = playerSprite.threatBox.top - playerSprite.threatBox.bottom;

		b_pos.x = l_pos.x + (playerSprite.threatBox.left - playerSprite.POS.x); 
		b_pos.y = l_pos.y + (playerSprite.POS.y - playerSprite.threatBox.top);
		b_pos.z = l_pos.z + 0.1f;
		gSprite->Draw(tBoxTexture, &b_src, NULL, &b_pos, 0xFFFFFFFF);
	}

	//draw the entities
	//TEST IF SPRITE IS WITHIN VIEWPORT, IF NOT, DON'T DRAW!
	for(unsigned int i = 0; i < enemyEntSprites.size(); i++)
	{
		e_SprInfo = enemyEntSprites[i]->getDrawInfo();
		if(e_SprInfo.POS.x < (camPos.x + SCREEN_WIDTH/2.0f) &&
			(e_SprInfo.POS.x + e_SprInfo.width) > (camPos.x - SCREEN_WIDTH/2.0f))
		{
			if(e_SprInfo.POS.y > (camPos.y - SCREEN_HEIGHT/2.0f) &&
				(e_SprInfo.POS.y - e_SprInfo.height) < (camPos.y + SCREEN_HEIGHT/2.0f))
			{
				l_pos.x = float(e_SprInfo.POS.x - (camPos.x - SCREEN_WIDTH/2.0f));
				l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - e_SprInfo.POS.y);
				l_pos.z = ((e_SprInfo.POS.y - e_SprInfo.height) + 500.0f)/1000.0f;
				gSprite->Draw(e_SprInfo.ss_ptr->gTexture, &e_SprInfo.drawRect, NULL, &l_pos, 0xFFFFFFFF);
			}
		}
	}

	//Draw for Debug mode here
	if(DEBUGMODE)
	{
		for(unsigned int i = 0; i < enemyEntSprites.size(); i++)
		{
			e_SprInfo = enemyEntSprites[i]->getDrawInfo();
			l_pos.x = float(e_SprInfo.POS.x - (camPos.x - SCREEN_WIDTH/2.0f));
			l_pos.y = float((camPos.y + SCREEN_HEIGHT/2.0f) - e_SprInfo.POS.y);
			l_pos.z = ((e_SprInfo.POS.y - e_SprInfo.height) + 500.0f)/1000.0f;
		//b_src == size of hitbox
			b_src.left = 0;
			b_src.right = e_SprInfo.hitBox.right - e_SprInfo.hitBox.left;
			b_src.top = 0;
			b_src.bottom = e_SprInfo.hitBox.top - e_SprInfo.hitBox.bottom;
		//get position
			b_pos.x = l_pos.x + (e_SprInfo.hitBox.left - e_SprInfo.POS.x); 
			b_pos.y = l_pos.y + (e_SprInfo.POS.y - e_SprInfo.hitBox.top);
			b_pos.z = l_pos.z + 0.1f;
			gSprite->Draw(hBoxTexture, &b_src, NULL, &b_pos, 0xFFFFFFFF);

			//setup and draw threatbox
			b_src.left = 0;
			b_src.right = e_SprInfo.threatBox.right - e_SprInfo.threatBox.left;
			b_src.top = 0;
			b_src.bottom = e_SprInfo.threatBox.top - e_SprInfo.threatBox.bottom;

			b_pos.x = l_pos.x + (e_SprInfo.threatBox.left - e_SprInfo.POS.x); 
			b_pos.y = l_pos.y + (e_SprInfo.POS.y - e_SprInfo.threatBox.top);
			b_pos.z = l_pos.z + 0.1f;
			gSprite->Draw(tBoxTexture, &b_src, NULL, &b_pos, 0xFFFFFFFF);
		}
	}
}

//display text to the screen
void Graphics::displayTime(clock_t _time, int y)	//elapsed time
{
	RECT rct;
	char display[MAXCHARSIZE];
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 180, 0, 0);

	rct.left = 245;
	rct.right = rct.left + 250;
	rct.top = y;
	rct.bottom = rct.top + 50;

	sprintf_s(display, "Milliseconds: %i  CPS: %i", _time, CLOCKS_PER_SEC);

	m_font->DrawText(NULL, display, -1, &rct,
					DT_NOCLIP | DT_WORDBREAK, fontColor);
}

void Graphics::displayString(int x, int y, char* msg)
{
	RECT rct;
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	rct.left = x;
	rct.right = (rct.left + strlen(msg)*20);
	rct.top = y;
	rct.bottom = rct.top + 50;

	m_font->DrawText(NULL, msg, -1, &rct,
					DT_NOCLIP | DT_WORDBREAK, fontColor);
}

void Graphics::drawTitle(int cursor)
{
	//screen position
	D3DXVECTOR3 l_pos;

	l_pos.x = l_pos.y = 0.0f;
	l_pos.z = 0.9f;
	//draw title screen
	gSprite->Draw(spriteContainer::getInstance()->getElemKey(',')->gTexture, NULL, NULL, &l_pos, 0xFFFFFFFF);

	//draw the title screen sprites
	if(cursor == 0)
	{
		l_pos.x = (SCREEN_WIDTH/10) * 1;
		l_pos.y = (SCREEN_HEIGHT/20) * 15;
		l_pos.z = 0.5f;
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('?')->gTexture, NULL, NULL, &l_pos, 0xFFFFFFFF);
		this->displayString((SCREEN_WIDTH/10) * 7, (SCREEN_HEIGHT/10) * 8, "LOAD");
	}
	else if(cursor == 1)
	{
		l_pos.x = (SCREEN_WIDTH/10) * 6;
		l_pos.y = (SCREEN_HEIGHT/20) * 15;
		l_pos.z = 0.5f;
		gSprite->Draw(spriteContainer::getInstance()->getElemKey('/')->gTexture, NULL, NULL, &l_pos, 0xFFFFFFFF);
		this->displayString((SCREEN_WIDTH/10) * 2, (SCREEN_HEIGHT/10) * 8, "NEW");
	}
}

void Graphics::drawSplash(int r, int c, int w, int h)
{
	//screen position
	D3DXVECTOR3 l_pos;
	RECT src;
//note: 200 is half frame width/height
	l_pos.x = SCREEN_WIDTH/2 - 200;
	l_pos.y = SCREEN_HEIGHT/2 - 200;
	l_pos.z = 0.9f;

	src.left = c*w;
	src.right = src.left+w;
	src.top = r*h;
	src.bottom = src.top+h;

	//draw splash screen
	gSprite->Draw(spriteContainer::getInstance()->getElemKey('>')->gTexture, &src, NULL, &l_pos, 0xFFFFFFFF);
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