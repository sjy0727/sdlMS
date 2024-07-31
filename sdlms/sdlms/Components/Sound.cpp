#include "Sound.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

Sound* Sound::load(wz::Node* node)
{
    // 从map缓存中获取对象
    if (sound_map.contains(node))
    {
        return sound_map[node];
    }
    // 如果缓存中不存在，则创建新的对象
    return new Sound(node);
}

Sound::Sound(wz::Node* node)
{
    // 如果节点不为空
    if (node != nullptr)
    {
        // 如果节点类型为UOL
        if (node->type == wz::Type::UOL)
        {
            // 将节点转换为UOL类型
            node = dynamic_cast<wz::Property<wz::WzUOL>*>(node)->get_uol();
        }

        // 将节点转换为WzSound类型
        auto sound = dynamic_cast<wz::Property<wz::WzSound>*>(node);
        // 获取原始数据
        auto data = sound->get_raw_data();

        // 定义缓冲区数据结构
        struct buffer_data
        {
            uint8_t* ptr;
            size_t   size; ///< 缓冲区中剩余的大小
        } bd {data.data(), data.size()};

        // 分配AVIOContext缓冲区
        auto         avio_ctx_buffer = av_malloc(0x1000);
        AVIOContext* ioCtx           = avio_alloc_context((uint8_t*)avio_ctx_buffer,
                                                0x1000,
                                                0,
                                                &bd,
                                                [](void* opaque, uint8_t* buf, int buf_size) -> int {
                                                    // 将内部缓冲区数据复制到buf
                                                    struct buffer_data* bd = (struct buffer_data*)opaque;
                                                    buf_size               = FFMIN(buf_size, bd->size);

                                                    if (!buf_size)
                                                        return AVERROR_EOF;

                                                    /* copy internal buffer data to buf */
                                                    memcpy(buf, bd->ptr, buf_size);
                                                    bd->ptr += buf_size;
                                                    bd->size -= buf_size;

                                                    return buf_size;
                                                },
                                                nullptr,
                                                nullptr);

        // 打开输入文件并读取音频流信息
        AVFormatContext* formatContext = avformat_alloc_context();
        formatContext->pb              = ioCtx;

        // 如果打开文件失败
        if (avformat_open_input(&formatContext, nullptr, nullptr, nullptr) != 0)
        {
            // 处理打开文件失败的情况
            return;
        }
        // 如果找不到音频流信息
        if (avformat_find_stream_info(formatContext, nullptr) < 0)
        {
            // 处理找不到音频流信息的情况
            return;
        }

        const AVCodec* codec;
        // 找到音频流索引
        int audioStreamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
        // 如果找不到音频流
        if (audioStreamIndex == -1 || !codec)
        {
            // 打开音频解码器并分配解码上下文
            // 处理找不到音频流的情况
            return;
        }
        AVCodecParameters* codecParameters = formatContext->streams[audioStreamIndex]->codecpar;

        // 分配解码上下文
        AVCodecContext* codecContext = avcodec_alloc_context3(codec);
        if (!codecContext)
        {
            // 处理无法分配解码上下文的情况
            return;
        }
        // 设置解码器参数
        if (avcodec_parameters_to_context(codecContext, codecParameters) < 0)
        {
            // 处理无法设置解码器参数的情况
            return;
        }
        // 打开解码器
        if (avcodec_open2(codecContext, codec, nullptr) < 0)
        {
            // 处理无法打开解码器的情况
            return;
        }

        // 解码并存储PCM数据
        std::vector<uint8_t> pcmData;
        // 解码音频帧
        AVPacket* packet = av_packet_alloc();
        AVFrame*  frame  = av_frame_alloc();

        // 分配SwrContext
        SwrContext* swrContext = swr_alloc();
        // 音频格式  输入的采样设置参数
        AVSampleFormat inFormat = codecContext->sample_fmt;
        // 出入的采样格式
        AVSampleFormat outFormat = AV_SAMPLE_FMT_S16;
        // 输入采样率
        int inSampleRate = codecContext->sample_rate;
        // 输出采样率
        int outSampleRate = inSampleRate;

        // 设置SwrContext参数
        swr_alloc_set_opts2(&swrContext,
                            &codecContext->ch_layout,
                            outFormat,
                            outSampleRate,
                            &codecContext->ch_layout,
                            inFormat,
                            inSampleRate,
                            0,
                            nullptr);

        // 初始化SwrContext
        if (swr_init(swrContext) < 0)
        {
            return;
        }

        // 获取输出通道数
        int outChannelCount = codecContext->ch_layout.nb_channels;

        // 分配输出缓冲区
        uint8_t* out_buffer = (uint8_t*)av_malloc(2 * outSampleRate);
        // 读取音频帧
        while (av_read_frame(formatContext, packet) >= 0)
        {
            // 发送音频包到解码器
            if ((avcodec_send_packet(codecContext, packet)) >= 0)
            {
                auto error = avcodec_receive_frame(codecContext, frame);
                // 如果解码出错
                if (error == AVERROR(EAGAIN) || error == AVERROR_EOF || error < 0)
                {
                    return;
                }
                // 如果解码成功
                else if (error == 0)
                {
                    // 转换音频格式
                    swr_convert(
                        swrContext, &out_buffer, outSampleRate * 2, (const uint8_t**)frame->data, frame->nb_samples);
                    // 获取输出缓冲区大小
                    int size =
                        av_samples_get_buffer_size(nullptr, outChannelCount, frame->nb_samples, AV_SAMPLE_FMT_S16, 1);

                    // 将输出缓冲区数据存储到pcmData
                    std::vector<uint8_t> out(out_buffer, out_buffer + size);

                    pcmData.insert(pcmData.end(), out.begin(), out.end());

                    // 释放frame
                    av_frame_unref(frame);
                }
                // 释放packet
                av_packet_unref(packet);
            }
        }
        // 释放输出缓冲区
        av_freep(&out_buffer);

        // 释放frame和packet
        av_frame_free(&frame);
        av_packet_free(&packet);

        // 释放SwrContext
        swr_free(&swrContext);

        // 释放解码上下文
        avcodec_free_context(&codecContext);
        // 关闭输入文件
        avformat_close_input(&formatContext);

        // 将pcmData存储到pcm_data
        pcm_data = pcmData;
        // 设置采样率
        freq = outSampleRate;
        // 设置偏移量
        offset = 0;
        // 设置延迟
        delay = 0;
        // 将当前Sound对象存储到sound_map中
        sound_map[node] = this;
    }
}