#include ".\dxmanager.h"

dxManager::dxManager(void)
{
	pD3D = NULL;
 	pd3dDevice = NULL;
	gD3dObject = NULL;
}

dxManager::~dxManager(void)
{
}

bool dxManager::init(HWND hwnd)
{
	// Create the D3D object
	gD3dObject=Direct3DCreate9(D3D_SDK_VERSION);
	if (gD3dObject==NULL)
		return FALSE;

	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		lastResult = E_FAIL;
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount  = 1;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferWidth  = 640;
	d3dpp.hDeviceWindow    = hwnd;

    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &pd3dDevice ) ) )
    {
		// Some cards may not do hardware vertex processing so fall back to software:
		HRESULT hr = gD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
		if (FAILED(hr))
			return FALSE;
    }

	return true;
}

void dxManager::shutdown(void)
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
}

void dxManager::beginRender(LPD3DXSPRITE gSprite)
{
	if( NULL == pd3dDevice )
        return;

    // Clear the backbuffer to a black color
    pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// Begin the scene
	HRESULT hr=pd3dDevice->BeginScene();
	if (SUCCEEDED(hr))
	{
		// Draw with alpha blending - needed for our transparent sprites
		gSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
}

void dxManager::endRender(LPD3DXSPRITE gSprite)
{
	// Finished drawing. By reusing the same sprite object D3D can maximise batching of the draws
	gSprite->End();

	// Finished rendering
	pd3dDevice->EndScene();
	pd3dDevice->Present(NULL,NULL,NULL,NULL);
}

IDirect3DSurface9* dxManager::getSurfaceFromBitmap(std::string filename, int width, int height)
{
	HRESULT hResult;
	IDirect3DSurface9* surface = NULL;

	hResult = pd3dDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hResult))
		return NULL;

	hResult = D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(hResult))
		return NULL;

	return surface;
}

IDirect3DSurface9* dxManager::getBackBuffer(void)
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

void dxManager::blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect)
{
	pd3dDevice->StretchRect(srcSurface, srcRect, getBackBuffer(), destRect, D3DTEXF_NONE);
}