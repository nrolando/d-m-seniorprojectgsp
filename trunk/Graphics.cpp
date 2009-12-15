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
	
	spriteContainer::getInstance()->setDevice(pd3dDevice);

	//*********** Set the default render states**************
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

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
	//Sprite tempSprite;

	//test the test vector
	int number = 0;

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
		spriteSurf.filename[i] = '\0';
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

	//load texture file name into c-string
		fin.get(check);
	while(!fin.eof())
	{
		while(j < MAXCHARSIZE && check != '#')
		{
			spriteSurf.filename[j] = check;
			fin.get(check);
			++j;
		}
		j = 0;

		fin >> spriteSurf.s;
		fin.ignore();

		//load image info from file (pull w&h even tho it should always be 50x50
<<<<<<< .mine
		//tempSprite.gSprite = getSurfaceFromBitmap(tempSprite.filename, w, h);
		D3DXCreateTextureFromFileEx(pd3dDevice,spriteSurf.filename,D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,D3DX_DEFAULT, 0,D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT, D3DX_FILTER_NONE,D3DX_FILTER_NONE,0xFFFFFFFF,NULL,NULL,&spriteSurf.gTexture);
		//tempSprite.width = w;
		//tempSprite.height = h;
		spriteContainer::getInstance()->push(spriteSurf);
=======
		tempSprite.spriteSurf = getSurfaceFromBitmap(tempSprite.filename, w, h);
		tempSprite.width = w;
		tempSprite.height = h;
//	spriteContainer::getInstance()->push(tempSprite);
>>>>>>> .r63

		while(j < MAXCHARSIZE)
		{
			spriteSurf.filename[j] = '\0';
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
				for(unsigned int k = 0; k < spriteContainer::getInstance()->size(); ++k)
				{
//				if(spriteContainer::getInstance()->getElem(k)->s == check)
//					tempSR.ptr = spriteContainer::getInstance()->getElem(k);
				}
			}

			if (tempSR.ptr)
			{
				//do x,y calculation here: i think: if i < 30 -25 else +25; if j < 10 -25 else +25
				tempSR.ptr->pos.x = -1475.0f + i * 50.0f;
				tempSR.ptr->pos.y = 475.0f - j * 50.0f;
				tempSR.ptr->pos.z = 0.0f;
				lvlSprites.push_back(tempSR);
			}

			++i;
			++index;
		}
	}
	i = j = 0;
	
	finput.close();
	return true;
}

void Graphics::drawLvl(std::vector<BaseGameEntity*> enemyEntSprites)
{
	RECT dest, src;

	//draw lvl
	src.left = LONG((camPos.x - SCREEN_WIDTH/2.0f) + 1500.0f);
	src.right = src.left + SCREEN_WIDTH;
	src.top = LONG(500.0f - (camPos.y + SCREEN_HEIGHT/2.0f));
	src.bottom = src.top + SCREEN_HEIGHT;
<<<<<<< .mine
=======
//blitToSurface(spriteContainer::getInstance()->getElem(0)->spriteSurf, &src, NULL);
>>>>>>> .r63

	Sprite *tempSpr = spriteContainer::getInstance()->getElem(0);
	tempSpr->pos = D3DXVECTOR3(0,0,0);

	// Draw the backdrop scaled so it covers more of the screen
	// There are two ways to use sprites, like this where we can build a matrix with
	// rotation, scaling and transform and then set it or where we just use the Draw parameters
	D3DXVECTOR3 pos=D3DXVECTOR3(0,0,1);

	// Scale the texture 4 times in each dimension
	D3DXVECTOR3 scaling(1.0f,1.0f,1.0f);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMATRIX mat;
	D3DXMatrixTransformation(&mat,NULL,NULL,&scaling,NULL,0,&pos);

	// Tell the sprite about the matrix
	tempSpr->gSprite->SetTransform(&mat);
	tempSpr->gSprite->Draw(tempSpr->gTexture,&src,NULL,NULL,0xFFFFFFFF);
														  
/*
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
					//blitToSurface(lvlSprites[i].ptr->gSprite, NULL, &dest);
					lvlSprites[i].ptr->gSprite->Draw(lvlSprites[i].ptr->gTexture,
													 &dest,
													 NULL,
													 &lvlSprites[i].ptr->pos,
													 0xFFFFFFFF);
				}
			}
		}
	}*/
/*
//draw the entities
//**TEST IF SPRITE IS WITHIN VIEWPORT, IF NOT, DON'T DRAW!
//**copied from above for loop**will finish later	
	for(unsigned int i = 0; i < enemyEntSprites.size(); i++)
	{
		//dont test, just draw
				dest.left = LONG((enemyEntSprites[i]->getPos().x - enemyEntSprites[i]->getWidth()/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
				if(dest.left < 0)
					dest.left = 0;
				dest.right = dest.left + enemyEntSprites[i]->getWidth();
				if(dest.right > SCREEN_WIDTH)
					dest.right = SCREEN_WIDTH;
				dest.top = LONG((camPos.y + SCREEN_HEIGHT/2.0f) - (enemyEntSprites[i]->getPos().y + enemyEntSprites[i]->getHeight()/2.0f));
				if(dest.top < 0)
					dest.top = 0;
				dest.bottom = dest.top + enemyEntSprites[i]->getHeight();
				if(dest.bottom > SCREEN_HEIGHT)
					dest.bottom = SCREEN_HEIGHT;
				blitToSurface(enemyEntSprites[i]->getSpritePtr()->gSprite,
					  &enemyEntSprites[i]->getSrc(), &dest);
				lvlSprites[i].ptr->gSprite->Draw(enemyEntSprites[i]->getSpritePtr()->gTexture,
												 &dest,
												 NULL,
												 &enemyEntSprites[i]->getSpritePtr()->pos,
												 0xFFFFFFFF);
*/
/*
//test if sprite is within viewport
		if((enemyEntSprites[i]->getPos().x - enemyEntSprites[i]->getWidth()/2.0f) < (camPos.x + SCREEN_WIDTH/2.0f) &&
			(enemyEntSprites[i]->getPos().x + enemyEntSprites[i]->getWidth()/2.0f) > (camPos.x - SCREEN_WIDTH/2.0f))
		{
			if((enemyEntSprites[i]->getPos().y + enemyEntSprites[i]->getHeight()/2.0f) > (camPos.y - SCREEN_HEIGHT/2.0f) &&
				(enemyEntSprites[i]->getPos().y - enemyEntSprites[i]->getHeight()/2.0f) < (camPos.y + SCREEN_HEIGHT/2.0f))
			{
				dest.left = LONG((enemyEntSprites[i]->getPos().x - enemyEntSprites[i]->getWidth()/2.0f) - (camPos.x - SCREEN_WIDTH/2.0f));
				if(dest.left < 0)
					dest.left = 0;
				dest.right = dest.left + enemyEntSprites[i]->getWidth();
				if(dest.right > SCREEN_WIDTH)
					dest.right = SCREEN_WIDTH;
				dest.top = LONG((camPos.y + SCREEN_HEIGHT/2.0f) - (enemyEntSprites[i]->getPos().y + enemyEntSprites[i]->getHeight()/2.0f));
				if(dest.top < 0)
					dest.top = 0;
				dest.bottom = dest.top + lvlSprites[i].ptr->height;
				if(dest.bottom > SCREEN_HEIGHT)
					dest.bottom = SCREEN_HEIGHT;
				blitToSurface(enemyEntSprites[i]->getSpritePtr()->gSprite,
					  &enemyEntSprites[i]->getSrc(), &dest);
			} 
		}	
	}*/
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