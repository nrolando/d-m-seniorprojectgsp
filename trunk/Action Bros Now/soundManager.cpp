#include "soundManager.h"

soundManager* soundManager::instance = 0;

soundManager::soundManager()
{
	curVolume = 50;
	volChange = 10;
	BGMplaying = false;
	currBGM = NULL;
}

soundManager* soundManager::getInstance()
{
	if(instance == 0)
		instance = new soundManager();

	return instance;
}

bool soundManager::initSound(HWND hwnd)
{
	HRESULT hr;

	hr = DirectSoundCreate8(NULL, &g_pDS, NULL);
	if FAILED (hr)
		return false;

	hr = g_pDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if FAILED(hr)
		return false;

	return true;
}

void soundManager::shutDown()
{
	if(g_pDS)
	{
		g_pDS->Release();
		g_pDS = NULL;
	}
}

//called once to get everything out of files and into the container
bool soundManager::loadAllSounds()
{
	char fName[MAXCHARSIZE];
	std::ifstream inFile;

	//add stuff to check for emty vector and clear vector

	sprintf_s(fName, (size_t)MAXCHARSIZE, "./Sounds/soundlist.txt"); 

	inFile.open(fName);
	
	if(!inFile.is_open())
		return false;

	inFile.getline(fName, MAXCHARSIZE, '\n');
	while(!inFile.eof())
	{
		//load the sound
		if(!loadWAV(fName))
			return false;
		//get the next sound name
		inFile.getline(fName, MAXCHARSIZE, '\n');
	}
	inFile.close();

	return true;

}

//called within loadAllSounds
//loads sound from file 
//pushes sound into container
bool soundManager::loadWAV(char *sName)
{
	soundFile tempSF;
	char fName[MAXCHARSIZE];
	//LPDIRECTSOUNDBUFFER apDSBuffer = NULL;
	CWaveFile *wavFile;
	HRESULT hr;

	//tempSF.soundName = sName;
	strcpy_s(tempSF.soundName, (size_t)MAXCHARSIZE, sName);

	//need
	sprintf_s(fName, (size_t)MAXCHARSIZE, "./Sounds/%s.wav", sName);

	wavFile = new CWaveFile();
	wavFile->Open(fName, NULL, WAVEFILE_READ );
	if( wavFile->GetSize() == 0 )
		return false;

	DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = 0;
    dsbd.dwBufferBytes   = wavFile->GetSize();
    dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat     = wavFile->m_pwfx;

	hr = g_pDS->CreateSoundBuffer( &dsbd, &tempSF.sound, NULL );
	if FAILED (hr)
	{
		MessageBox(NULL, "unable to create sound buffer", "ERROR", MB_OK);
		return false;
	}

	VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 
	
	hr = tempSF.sound->Lock( 0, wavFile->GetSize(), 
						   &pDSLockedBuffer, &dwDSLockedBufferSize, 
                           NULL, NULL, 0L );
	if FAILED (hr)
        return false;

    // Reset the wave file to the beginning 
    wavFile->ResetFile();

	// Read the wave file
    hr = wavFile->Read( (BYTE*) pDSLockedBuffer,
                       dwDSLockedBufferSize, 
                       &dwWavDataRead );
	// Check to make sure that this was successful
	if FAILED (hr)
        return false;

	// Check to make sure the wav file is not empty
    if( dwWavDataRead == 0 )
    {
        // Wav is blank, so just fill with silence
        FillMemory( (BYTE*) pDSLockedBuffer, 
                    dwDSLockedBufferSize, 
                    (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if( dwWavDataRead < dwDSLockedBufferSize )
    {
	    // Don't repeat the wav file, just fill in silence 
        FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
                   dwDSLockedBufferSize - dwWavDataRead, 
                   (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }

    // Unlock the buffer, we don't need it anymore.
    tempSF.sound->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

	// Clean up
	delete wavFile;
	sCont.pushSound(tempSF);
	return true;
}


//the good stuff
//for playing bgm
void soundManager::playSoundLoop(char* sound)
{
	sCont.getSound(sound)->sound->Play(0,0,DSBPLAY_LOOPING);
	BGMplaying = true;
	currBGM = sound;
}
//for playing sfx
void soundManager::playSound(char *sound)
{
	sCont.getSound(sound)->sound->SetVolume((long)10);
	sCont.getSound(sound)->sound->Play(0,0,0);
}
//for stopping bgm
void soundManager::stopSound()
{
	sCont.getSound(currBGM)->sound->Stop();
	BGMplaying = false;
	currBGM = NULL;
}

//for stoping any sound
void soundManager::stopSound(char *sound)
{
	sCont.getSound(sound)->sound->Stop();
}