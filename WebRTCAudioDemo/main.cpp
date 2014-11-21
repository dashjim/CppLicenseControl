
#include <iostream>
#include <locale>
#include <string>
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
#pragma execution_character_set("utf-8") 

using namespace std;
int main(int argc, char *argv[])
{
    std::cout << "hello WebRTC" << std::endl;

    webrtc::VoiceEngine* voe = webrtc::VoiceEngine::Create();
    if(!voe)
        cout << "voe is NULL" << endl;

    webrtc::VoEBase *voeBase = webrtc::VoEBase::GetInterface(voe);
    webrtc::AudioDeviceModule *audio;
    
//    audio = webrtc::CreateAudioDeviceModule(0, webrtc::AudioDeviceModule::kPlatformDefaultAudio);
  //  assert(audio);
    //audio->Init();

    int init = voeBase->Init();
    cout << "voe init result " << init << endl;

    //handware  
    int nRec = 0;  
    char devName[128] = {0};  
    char guidName[128] = {0};  
      
    webrtc::VoEHardware* hardware = webrtc::VoEHardware::GetInterface(voe);  
    hardware->GetNumOfPlayoutDevices(nRec);  
    std::cout << "Get num of recordingdevice:" << nRec << std::endl;    
    for(int idx = 0; idx < nRec; idx++)  
    {  
        hardware->GetPlayoutDeviceName(idx , devName , guidName);  
        std::cout << "GetRecordingName(" << idx << ")  " << "name:" << devName << "  guidName:" << guidName << std::endl;  
    }  
    
    int channel = voeBase->CreateChannel();
    cout << "channel " << channel << endl;

    if(voeBase->StartPlayout(channel)!=0)
    {
        std::cout << "start playout last error: " << voeBase->LastError();
    }
    else
    {
        std::cout << "start play done" << std::endl;
    }
    
    std::string fileName = "test.wav";

    webrtc::VoEFile* voeFile = webrtc::VoEFileImpl::GetInterface(voe);
    int nRet = voeFile->StartPlayingFileLocally(channel, fileName.c_str(), false,webrtc::FileFormats::kFileFormatWavFile);

    std::cout << "play local file done with return code: " << nRet << std::endl;
    
    string s;
    std::getline(std::cin, s);

    voeBase->DeleteChannel(channel);
    voeBase->Terminate();
    voeFile->Release();
    hardware->Release();
    webrtc::VoiceEngine::Delete(voe);

    return 0;
}