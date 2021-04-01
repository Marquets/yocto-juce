/*
  =============================================================================

    This file contains the basic startup code for a JUCE application

  =============================================================================
*/

#include <JuceHeader.h>
using namespace juce;

//=============================================================================
class MyCallback : public AudioIODeviceCallback
{
public:
	MyCallback() {}
	void audioDeviceIOCallback(const float ** inputChannelData, 
		int numInputChannels, float ** outputChannelData, int numOutputChannels, int numSamples) override
	{
		for (int i = 0; i < numSamples; ++i)
		{
			float s = m_gain * sin(m_phase);
			for (int j = 0; j < numOutputChannels; ++j)
			{
				outputChannelData[j][i] = s;
			}
			m_phase += (m_freq / m_sr)*juce::MathConstants<double>::twoPi;
		}
	}

	void audioDeviceAboutToStart(AudioIODevice * device) override
	{
		m_phase = 0.0;
		m_sr = device->getCurrentSampleRate();
	}

	void audioDeviceStopped() override
	{
	}
	double m_freq = 440.0;
	double m_gain = 0.1;
private:
	double m_phase = 0.0;
	double m_sr = 0.0;
};

int main (int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI gui_init; // not used for "GUI" in this case, just for initing and shutting down the MessageManager
	AudioDeviceManager aman;
	String err = aman.initialiseWithDefaultDevices(0, 2);
	if (err.isEmpty())
	{
		std::cout << "device opened : " << aman.getCurrentAudioDevice()->getName() << "\n";
		MyCallback cb;
		aman.addAudioCallback(&cb);
		while (true)
		{
			double freq = 0.0;
			std::cin >> freq;
			if (freq > 0.0)
				cb.m_freq = freq;
			else
				break;
		}
		std::cout << "closing device...\n";
	}
	else
		std::cout << "could not open device : " << err << "\n";
    return 0;
}
