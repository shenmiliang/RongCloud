#ifndef CAMERAAPPLICATION_NATIVE_LIB_H
#define CAMERAAPPLICATION_NATIVE_LIB_H
#endif

#include <jni.h>
#include <memory>
#include <string>
#include <chrono>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavfilter/buffersink.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/opt.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#ifdef  __cplusplus
}
#endif

const char *rvm_model = "rvm_mobilenetv3_fp32.onnx";
static const char *filter_descr = "scale=78:24,transpose=cclock";
/* other way:
   scale=78:24 [scl]; [scl] transpose=cclock // assumes "[in]" and "[out]" to be input output pads respectively
 */

AVFilterContext *buffersink_ctx;
AVFilterContext *buffersrc_ctx;
AVFilterGraph *filter_graph;

//static int64_t last_pts = AV_NOPTS_VALUE;

static int init_filters(const char *filters_descr, int in_width, int in_height)
{
    char args[512];
    int ret = 0;
    const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    const AVFilter *buffersink = avfilter_get_by_name("buffersink");
    AVFilterInOut *outputs = avfilter_inout_alloc();
    AVFilterInOut *inputs  = avfilter_inout_alloc();
   // AVRational time_base = fmt_ctx->streams[video_stream_index]->time_base;
    enum AVPixelFormat pix_fmts[] = { AV_PIX_FMT_NV21, AV_PIX_FMT_NONE };

    filter_graph = avfilter_graph_alloc();
    if (!outputs || !inputs || !filter_graph) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    snprintf(args, sizeof(args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             in_width,
             in_height,
             AV_PIX_FMT_NV21,
             1,
             25,
             1,
             1);

    ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in", args, NULL, filter_graph);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "Cannot create buffer source\n");
        goto end;
    }

    /* buffer video sink: to terminate the filter chain. */
    ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                                       NULL, NULL, filter_graph);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "Cannot create buffer sink\n");
        goto end;
    }

    ret = av_opt_set_int_list(buffersink_ctx,
                              "pix_fmts",
                              pix_fmts,
                              AV_PIX_FMT_NONE,
                              AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "Cannot set output pixel format\n");
        goto end;
    }

    outputs->name       = av_strdup("in");
    outputs->filter_ctx = buffersrc_ctx;
    outputs->pad_idx    = 0;
    outputs->next       = NULL;


    inputs->name       = av_strdup("out");
    inputs->filter_ctx = buffersink_ctx;
    inputs->pad_idx    = 0;
    inputs->next       = NULL;

    if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
                                        &inputs, &outputs, NULL)) < 0)
        goto end;

    if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
        goto end;

    end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    return ret;
}

static int do_it(unsigned char* frame_buffer_in, unsigned char* dest, int size, int in_width, int in_height)
{
    int ret = 0;
    AVPacket packet;
    AVFrame *frame_in;
    AVFrame *frame_out;

    unsigned char *frame_buffer_out;

    frame_in = av_frame_alloc();
    frame_buffer_in = (unsigned char *)av_malloc(size);
    av_image_fill_arrays(frame_in->data, frame_in->linesize,frame_buffer_in,
                         AV_PIX_FMT_NV21,in_width, in_height, 1);

    frame_out = av_frame_alloc();
    frame_buffer_out = (unsigned char *)av_malloc(size);
    av_image_fill_arrays(frame_out->data, frame_out->linesize,frame_buffer_out,
                         AV_PIX_FMT_NV21,in_width, in_height, 1);

    frame_in->width = in_width;
    frame_in->height = in_height;
    frame_in->format = AV_PIX_FMT_NV21;
    frame_in->data[0] = frame_buffer_in;
    frame_in->data[1] = frame_buffer_in + in_width * in_height;
    frame_in->data[2] = frame_buffer_in + in_width * in_height * 5 / 4;

    if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "Error while add frame\n");
        goto end;
    }

    ret = av_buffersink_get_frame(buffersink_ctx, frame_out);
    if (ret < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "get frame error\n");
        goto end;
    }

    printf("Process 1 frame!\n");
    memcpy(dest, frame_out->data, size);
    av_frame_unref(frame_out);

    end:
    avfilter_graph_free(&filter_graph);
    av_frame_free(&frame_in);
    av_frame_free(&frame_out);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        __android_log_print(ANDROID_LOG_ERROR, "NATIVE", "Error occurred: %s\n", av_err2str(ret));
    }

    return ret;
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_hl_rongclound_MainActivity_matting(JNIEnv *env, jobject thiz, jint width, jint height,
                                            jbyteArray src) {
    int ret = init_filters(filter_descr, width, height);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "MATTING", "init filter error %s\n", av_err2str(ret));
        return src;
    }

    jboolean isCopy = false;
    jbyte * buffer = env->GetByteArrayElements(src, &isCopy);
    env->ReleaseByteArrayElements(src, buffer, 0);

    int dest_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    unsigned char dest[dest_size];
    ret = do_it((unsigned char*)buffer, dest, dest_size, width, height);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_DEBUG, "MATTING", "Filter frame error %s\n", av_err2str(ret));
        return src;
    }

    int len = height*3/2*width;
    jbyteArray by = env->NewByteArray(len);
    env->SetByteArrayRegion(by, 0, len, reinterpret_cast<const jbyte*>(dest));
    return by;
}