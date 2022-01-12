#include <iostream>
#include <transcoding/SREncoder.h>
#include <muxing/SRMediaOutput.h>
#include <transcoding/SRVideoFilter.h>
#include "demuxing/SRVideoInput.h"
#include "demuxing/SRAudioInput.h"
#include "transcoding/SRDecoder.h"


int main() {
    AVPacket *inPacket, *outPacket;
    AVFrame *rawFrame, *scaled_frame;
    scaled_frame = av_frame_alloc();
    rawFrame = av_frame_alloc();
    inPacket = av_packet_alloc();
    outPacket = av_packet_alloc();
    avdevice_register_all();

    //Open two input devices


   //set output file for video only
    SROutputSettings outputSettings;
    outputSettings.video_codec = AV_CODEC_ID_MPEG4;
    outputSettings.audio_codec = AV_CODEC_ID_NONE;

    outputSettings.fps = 30;
    outputSettings.filename = "testfile.mp4";
    outputSettings.outscreenres =SRResolution{2560,1600};

    SRMediaOutput outputFile(outputSettings);
    SRVideoInput videoInput("avfoundation", "1:none", outputSettings.outscreenres, SROffset{0,0}, outputSettings.fps );
    SRDecoder videoDecoder;
    SREncoder videoEncoder;

    videoInput.open();
    videoDecoder.setDecoderContext(videoInput.getCodecContext());
    outputFile.initFile();
    videoEncoder.setEncoderContext(outputFile.getVideoCodecContext());

    SRVideoFilter videoFilter(outputFile.getVideoCodecContext(), videoInput.getCodecContext());
    videoFilter.init();

    long long int last = 0;
    printf("[SRlib - recording screen]\n");
    while(last/30 < 10 /*record for five sec*/) {


        if(videoInput.readPacket(inPacket) >= 0){
            last = inPacket->pts;
            videoDecoder.decodePacket(inPacket);
            while(videoDecoder.getDecodedFrame(rawFrame)>=0){
              //  printf("\t decodedFrame %lld\n", rawFrame->pts);

                scaled_frame = videoFilter.filterFrame(rawFrame);
                if(videoEncoder.encodeFrame(scaled_frame)<0){
                    printf("DROPPED");
                };
                while(videoEncoder.getEncodedPacket(outPacket)>=0) {
                //    printf("\t\t encodedPacket %lld\n", outPacket->pts);

                    if(outputFile.writePacket(outPacket, video /*passing a video packet*/)>=0){

                        // printf("PTS: %lld - Duration %lld\n", outPacket->pts, outPacket->duration);
                    }
                        //      printf("transcoded packet written on %s", outputFile.getFilename());
                    else{
                        printf("DROPPED");
                    }
                }

            }
            av_packet_unref(inPacket);

        }
    }

    return 0;
}
