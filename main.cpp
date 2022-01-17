#include "SRRecorder.h"

int main(){
    char filename[512] = "filmato_di_prova.mp4";


    SRConfiguration configuration;
    configuration.enable_audio = false;
    configuration.enable_video = true;
    configuration.filename = filename;
    configuration.enable_crop = true;
    configuration.crop_info = SRCropRegion {SROffsetRational{{1,4},{1,4}},SRResolutionRational {{1,4},{1,4}} };
    /* a recorder is instantiated*/
    SRRecorder recorder(configuration);
    /*before setting crop the recorder must be initialized to retrieve capabilities from the devices*/
    recorder.initCapture();
    recorder.startCapture();

}