#include "Graphics.h"

Graphics::Graphics()
{
	pD3D = NULL;
	pd3dDevice = NULL;
	g_pVB = NULL;
	m_font = NULL;
	stage = 1;
	area = 1;
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

	//set camPos to start of level. lvl dimensions are always 3000x1000
	camPos.x = -1180.0f; camPos.y = 0.0f; camPos.z = 0.0f;

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
	// Finished rendering
	pd3dDevice->EndScene();
	pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

void Graphics::RenderLvl()
{	
	// Begin the scene
	RECT src;

	//draw lvl
	src.left = LONG((camPos.x - SCREEN_WIDTH/2.0f) + 1500.0f);
	src.right = src.left + SCREEN_WIDTH;
	src.top = LONG(500.0f - (camPos.y + SCREEN_HEIGHT/2.0f));
	src.bottom = src.top + SCREEN_HEIGHT;

	spriteSheet *temp = spriteContainer::getInstance()->getSheetPTR("lvl1-1.jpg");

	// Draw with alpha blending - needed for our transparent sprites
	temp->gSprite->Begin(D3DXSPRITE_ALPHABLEND);

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
	temp->gSprite->SetTransform(&mat);
	temp->gSprite->Draw(temp->gTexture,&src,NULL,NULL,0xFFFFFFFF);

	// Finished drawing. By reusing the same sprite object D3D can maximise batching of the draws
	temp->gSprite->End();	
}

void Graphics::drawSprites(eSprInfo* spriteInfo)
{
	// Draw with alpha blending - needed for our transparent sprites
	spriteInfo->spriteSheet->gSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// Draw the backdrop scaled so it covers more of the screen
	// There are two ways to use sprites, like this where we can build a matrix with
	// rotation, scaling and transform and then set it or where we just use the Draw parameters
	//D3DXVECTOR3 pos=D3DXVECTOR3(0,0,0);

	// Scale the texture 4 times in each dimension
	D3DXVECTOR3 scaling(1.0f,1.0f,1.0f);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMATRIX mat;
	D3DXMatrixTransformation(&mat,NULL,NULL,&scaling,NULL,0,NULL);

	// Tell the sprite about the matrix
	D3DXMatrixIdentity(&mat);
	spriteInfo->spriteSheet->gSprite->SetTransform(&mat);
	spriteInfo->spriteSheet->gSprite->Draw(spriteInfo->spriteSheet->gTexture,&spriteInfo->drawRect,NULL,&spriteInfo->POS,0xFFFFFFFF);

	// Finished drawing. By reusing the same sprite object D3D can maximise batching of the draws
	spriteInfo->spriteSheet->gSprite->End();
}												  

void Graphics::nextArea()
{
	area += 1;
	if(area > 3)
	{
		++stage;
		area = 1;
	}
	if(stage > 4)
	{
		stage = 1;
		area = 1;
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