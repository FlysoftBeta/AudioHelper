#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

using namespace std;

int main()
{
	HRESULT hr;

	hr = CoInitialize(nullptr);

	IMMDeviceEnumerator* pEnum = nullptr;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&pEnum);

	IMMDevice* pDevice = nullptr;
	hr = pEnum->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

	IAudioMeterInformation* pMeter = nullptr;
	pDevice->Activate(__uuidof(IAudioMeterInformation),
		CLSCTX_ALL,
		nullptr,
		(void**)&pMeter);

	while (true)
	{
		UINT channels = 0;
		pMeter->GetMeteringChannelCount(&channels);
		float* channelPeaks = new float[channels];
		pMeter->GetChannelsPeakValues(channels, channelPeaks);

		cout << *max_element(channelPeaks, channelPeaks + channels) << endl;

		Sleep(100);
	}

	CoUninitialize();
}