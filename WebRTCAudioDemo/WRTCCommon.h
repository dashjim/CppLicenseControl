
#ifndef WRTC_COMMON_H
#define WRTC_COMMON_H

#include <boost/bind.hpp>

#include "voe_base.h"
#include "voe_codec.h"
#include "voe_volume_control.h"
#include "voe_dtmf.h"
#include "voe_rtp_rtcp.h"
#include "voe_audio_processing.h"
#include "voe_file.h"
#include "voe_file_impl.h"
#include "voe_video_sync.h"
#include "voe_encryption.h"
#include "voe_hardware.h"
#include "voe_external_media.h"
#include "voe_network.h"
#include "voe_neteq_stats.h"
#include "engine_configurations.h"
#include "voe_errors.h"
#include "voe_call_report.h"
#include "common_types.h"

#include "boost/smart_ptr/shared_ptr.hpp"
#include "boost/bind/mem_fn.hpp"

namespace VDIMedia
{
	enum etWRTCReturnCode
	{
		eWRTC_ERROR = -1,
		eWRTC_SUCCESS = 0
	};

	typedef boost::shared_ptr<webrtc::VoiceEngine> sptrVoiceEngine;
	typedef boost::shared_ptr<webrtc::VoEBase> sptrVoEBase;
	typedef boost::shared_ptr<webrtc::VoEDtmf> sptrVoEDtmf;
	typedef boost::shared_ptr<webrtc::VoEAudioProcessing> sptrVoEAudioProcessing;
	typedef boost::shared_ptr<webrtc::VoENetwork> sptrVoENetwork;
	typedef boost::shared_ptr<webrtc::VoEVideoSync> sptrVoEVideoSync;
	typedef boost::shared_ptr<webrtc::VoEExternalMedia> sptrVoEExternalMedia;
	typedef boost::shared_ptr<webrtc::VoENetEqStats> sptrVoENetEqStats;
	typedef boost::shared_ptr<webrtc::VoEVolumeControl> sptrVoEVolumeControl;
	typedef boost::shared_ptr<webrtc::VoECodec> sptrVoECodec;
	typedef boost::shared_ptr<webrtc::VoEHardware> sptrVoEHardware;
	typedef boost::shared_ptr<webrtc::VoEEncryption> sptrVoEEncryption;
	typedef boost::shared_ptr<webrtc::VoEFile> sptrVoEFile;
	typedef boost::shared_ptr<webrtc::VoERTP_RTCP> sptrVoERTP_RTCP;
	typedef boost::shared_ptr<webrtc::VoECallReport> sptrVoECallReport;

	class VoEFactory
	{
	public:
		static sptrVoiceEngine Create();
		static sptrVoiceEngine Instance();
	};

	template<typename T>
	boost::shared_ptr<T> GetInterface(sptrVoiceEngine &spEngine)
	{
		boost::shared_ptr<T> spInt(T::GetInterface(spEngine.get()), boost::mem_fn(&T::Release));

		if(!spInt)
		{
			LOGERROR << typeid(T).name() << "::GetInterface failed";
			throw audio_error();
		}

		return spInt;
	}

	template<typename T>
	boost::shared_ptr<T> GetInterface()
	{
		boost::shared_ptr<T> spInt(T::GetInterface(VoEFactory::Instance().get()), boost::mem_fn(&T::Release));

		if(!spInt)
		{
			LOGERROR << typeid(T).name() << "::GetInterface failed";
			throw audio_error();
		}

		return spInt;
	}
}

#endif
