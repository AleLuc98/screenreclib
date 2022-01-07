//
// Created by Giulio Carota on 17/10/21.
//

#include "SCPPAudioInput.h"

/**
 * the constructor initialize the
 * input device with requested options
 */
SCPPAudioInput::SCPPAudioInput(char* audio_src, char* audio_url)  : SCPPInput(audio_src, audio_url) {
}

AVFormatContext* SCPPAudioInput::open(){
    //if one of them != nullptr then input already initialized
    if(inFormatContext != nullptr || inCodecContext!= nullptr || streamIndex != -1)
        return inFormatContext;

    AVInputFormat* inAInputFormat =nullptr;
    int value = 0;

    inFormatContext = avformat_alloc_context();

    //get input format
    inAInputFormat = av_find_input_format(device_src);
    value = avformat_open_input(&inFormatContext, device_url, inAInputFormat, &options);
    if (value != 0) {
        std::string msg = (std::string)"Cannot open selected device (" + device_url + ")";
        throw openSourceException(msg.c_str());
    }


    //get audio stream infos from context
    value = avformat_find_stream_info(inFormatContext, nullptr);
    if (value < 0) {
        throw streamInformationException("Cannot find the stream information");
    }

    //find the first audio stream with a given code
    streamIndex = -1;
    for (int i = 0; i < inFormatContext->nb_streams; i++){
        if (inFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        throw streamIndexException("Cannot find the video stream index.");

    }

    AVCodecParameters *params = inFormatContext->streams[streamIndex]->codecpar;
    AVCodec *inACodec = avcodec_find_decoder(params->codec_id);
    if (inACodec == nullptr) {
        throw findDecoderException("Cannot find the decoder.");

    }

    inCodecContext = avcodec_alloc_context3(inACodec);
    avcodec_parameters_to_context(inCodecContext, params);

    value = avcodec_open2(inCodecContext, inACodec, nullptr);
    if (value < 0) {
        throw openAVCodecException("Cannot open the av codec.");

    }

    return inFormatContext;
}

int SCPPAudioInput::getInputAudioStreamIndex() const {
    return streamIndex;
}

AVFormatContext *SCPPAudioInput::getInputAudioFormatContext() const {
    return inFormatContext;
}

AVCodecContext *SCPPAudioInput::getInputAudioCodecContext() const {
    return inCodecContext;
}

SCPPAudioInput::~SCPPAudioInput() = default;

