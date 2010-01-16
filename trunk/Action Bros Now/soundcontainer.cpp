//#include "soundContainer.h"
//
//soundContainer* soundContainer::instance = 0;
//
//soundContainer* soundContainer::getInstance()
//{
//	if(instance == 0)
//		instance = new soundContainer();
//
//	return instance;
//}
//
//void soundContainer::loadAllSounds()
//{
//	char fName[MAXCHARSIZE];
//	std::ifstream inFile;
//	soundFile tempSound;
//
//	//add stuff to check for emty vector and clear vector
//
//	sprintf_s(fName, (size_t)MAXCHARSIZE, ""); //fill string with the file path to sounds
//
//	inFile.open(fName);
//	
//	//add check for file open
//
//	inFile.getline(tempSound.soundName, MAXCHARSIZE, '\n');
//	while(!inFile.eof())
//	{
//		//load wav loads sound & returns it to be pushed into vector
//		soundCont.push_back(LoadWav(tempSound.soundName));
//		//get the next sound name
//		inFile.getline(tempSound.soundName, MAXCHARSIZE, 'n');
//	}
//	inFile.close();
//
//}
//
//
////copy pasted from example code (hope it works)
//LPDIRECTSOUNDBUFFER soundContainer::LoadWav(char* sName)
//{
//LPDIRECTSOUNDBUFFER apDSBuffer = NULL;
//	CWaveFile *wavFile;
//	HRESULT hr;
//
//	wavFile = new CWaveFile();
//	wavFile->Open(sName, NULL, WAVEFILE_READ );
//	if( wavFile->GetSize() == 0 )
//	{
////	MessageBox(wndHandle, "invalid file", "ERROR", MB_OK);
//		return false;
//	}
//
//	DSBUFFERDESC dsbd;
//    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
//    dsbd.dwSize          = sizeof(DSBUFFERDESC);
//    dsbd.dwFlags         = 0;
//    dsbd.dwBufferBytes   = wavFile->GetSize();
//    dsbd.guid3DAlgorithm = GUID_NULL;
//	dsbd.lpwfxFormat     = wavFile->m_pwfx;
//
//	hr = g_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer, NULL );
//	if FAILED (hr)
//	{
//		MessageBox(NULL, "unable to create sound buffer", "ERROR", MB_OK);
//		return NULL;
//	}
//
//	VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
//    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
//    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 
//	
//	hr = apDSBuffer->Lock( 0, wavFile->GetSize(), 
//						   &pDSLockedBuffer, &dwDSLockedBufferSize, 
//                           NULL, NULL, 0L );
//	if FAILED (hr)
//        return NULL;
//
//    // Reset the wave file to the beginning 
//    wavFile->ResetFile();
//
//	// Read the wave file
//    hr = wavFile->Read( (BYTE*) pDSLockedBuffer,
//                       dwDSLockedBufferSize, 
//                       &dwWavDataRead );
//	// Check to make sure that this was successful
//	if FAILED (hr)
//        return NULL;
//
//	// Check to make sure the wav file is not empty
//    if( dwWavDataRead == 0 )
//    {
//        // Wav is blank, so just fill with silence
//        FillMemory( (BYTE*) pDSLockedBuffer, 
//                    dwDSLockedBufferSize, 
//                    (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
//    }
//    else if( dwWavDataRead < dwDSLockedBufferSize )
//    {
//	    // Don't repeat the wav file, just fill in silence 
//        FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
//                   dwDSLockedBufferSize - dwWavDataRead, 
//                   (BYTE)(wavFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
//    }
//
//    // Unlock the buffer, we don't need it anymore.
//    apDSBuffer->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );
//
//	// Clean up
//	delete wavFile;
//
//	return apDSBuffer;
//}