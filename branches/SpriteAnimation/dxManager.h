#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3dx9.h>
#include <string>

class dxManager
{
public:
	dxManager(void);
	~dxManager(void);
	bool init(HWND hwnd);
	void shutdown(void);
	void beginRender(LPD3DXSPRITE);
	void endRender(LPD3DXSPRITE);
	IDirect3DSurface9* getSurfaceFromBitmap(std::string filename, int width, int height);

	IDirect3DSurface9* getBackBuffer(void);
	void blitToSurface(IDirect3DSurface9* srcSurface, const RECT *srcRect, const RECT *destRect);

	//Get functions for the directx variables
	LPDIRECT3DDEVICE9  getpd3dDevice(){return pd3dDevice;}

private:
	HRESULT                 lastResult;
	LPDIRECT3D9             pD3D;
	LPDIRECT3D9		        gD3dObject;	// the directX object
	LPDIRECT3DDEVICE9       pd3dDevice;
};
