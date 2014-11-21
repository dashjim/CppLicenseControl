#include "WRTCCommon.h"
#include <boost/utility.hpp>

namespace VDIMedia
{
	class CWRTCSound;
	typedef boost::shared_ptr<CWRTCSound> sptrCWRTCSound;

	class CWRTCSound : boost::noncopyable
	{
	protected:
		CWRTCSound(const sptrVoiceEngine &spWRTCVoiceEngine);
		~CWRTCSound();
		static unsigned nNextToneId;
		void Release();
	public:
		static sptrCWRTCSound Create(const sptrVoiceEngine &spWRTCVoiceEngine);
		unsigned Play(const char *szFileName, bool bRepeat, float fVolumeScaling = 1.0, int startPointMs = 0, int stopPointMs = 0);
		unsigned PlayFromPath(const char *szFilePath,  bool bLoop, float fVolumeScaling = 1.0);
		bool IsPlaying();
		void StartRecordingMic(const char *szFileName);
		void StopRecordingMic();
		void StartPlayingGreetingAsMicrophone(const char *szFileName);
		bool IsPlayingGreetingAsMicrophone();
		void StopPlayingGreetingAsMicrophone();
		void GetFileDuration(const char *szFileName, int& durationMs);
		void ScalePlayoutVolume(float fVolumeScaling);
		void Stop();
		unsigned GetToneId();
		const sptrVoiceEngine & GetEngine();

	protected:
		sptrVoiceEngine m_spVoiceEngine;
		sptrVoEBase m_spVoEBase;
		sptrVoEFile m_spVoEFile;

		int	m_nChannelId;
		unsigned m_nToneId;
	};

}

