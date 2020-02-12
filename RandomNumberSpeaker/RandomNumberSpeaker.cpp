// RandomNumberSpeaker.cpp : Defines the entry point for the console application.
//
// WinAPI TTS useful links:
//- Requirements:
//  https://msdn.microsoft.com/en-us/library/hh362873(v=office.14).aspx
//- Tutorial:
//  https://msdn.microsoft.com/en-us/library/ms720163(v=vs.85).aspx
//- TTS XML tutorial:
//  https://msdn.microsoft.com/en-us/library/ms717077(v=vs.85).aspx
//
// In my case, I ended up installing the Speech Runtime and using the 
// following path in the dependencies: 
// C:\Program Files\Microsoft SDKs\Speech\v11.0\Lib

#include "stdafx.h"
#include <sapi.h>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

// Constants 
#define STRSIZE (10)
#define DIGIT_COUNT (7)
#define MAX_RAND_NUM (10)

int main(int argc, char* argv[])
{
	ISpVoice * pVoice = NULL;
	int iSecret, iCount;

	char pchTest[STRSIZE];
	wchar_t wtext[STRSIZE];

	if (FAILED(::CoInitialize(NULL)))
		return FALSE;

	/* 
	Initialize random seed: 
	(This is required for rand() to produce different results each 
	time the program is executed)
	*/
	srand(time(NULL));

	/* Request speech service: */
	HRESULT hr = CoCreateInstance(
		CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);

	if (SUCCEEDED(hr))
	{
		//while (TRUE)
		for (iCount = 0; iCount < DIGIT_COUNT; iCount++)
		{
			/* Generate a random number bound between 0 and (MAX_RAND_NUM - 1): */
			iSecret = rand() % MAX_RAND_NUM;

			/* Convert number to multi-byte character string */
			if (snprintf(pchTest, STRSIZE, "%d", iSecret) < 0)
			{
				return FALSE;
			}
			/* Convert to wide-character string */
			if (mbstowcs_s(NULL, wtext, pchTest, strlen(pchTest) + 1)//Plus null
				!= 0)
			{
				return FALSE;
			}
			
			/* Send the result string to Microsoft Sam */
			hr = pVoice->Speak(wtext, 0, NULL);
		}
	}

	/* Release all resources: */
	pVoice->Release();
	pVoice = NULL;

	::CoUninitialize();

	/* Return */
	return TRUE;
}