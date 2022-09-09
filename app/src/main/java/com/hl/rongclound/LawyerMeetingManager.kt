package com.hl.rongclound

import android.app.Activity
import androidx.constraintlayout.widget.ConstraintLayout
import cn.rongcloud.rtc.api.RCRTCEngine
import cn.rongcloud.rtc.api.callback.IRCRTCVideoInputFrameListener
import cn.rongcloud.rtc.api.stream.RCRTCInputStream
import cn.rongcloud.rtc.api.stream.RCRTCVideoInputStream
import cn.rongcloud.rtc.api.stream.RCRTCVideoView
import cn.rongcloud.rtc.base.RCRTCMediaType
import cn.rongcloud.rtc.base.RCRTCRemoteVideoFrame
import cn.rongcloud.rtc.base.RCRTCStreamType
import cn.rongcloud.rtc.core.RendererCommon


//                  _ooOoo_
//                 o8888888o
//                 88" . "88
//                 (| -_- |)
//                 O\  =  /O
//              ____/`---'\____
//            .'  \\|     |//  `.
//           /  \\|||  :  |||//  \
//          /  _||||| -:- |||||-  \
//          |   | \\\  -  /// |   |
//          | \_|  ''\---/''  |   |
//           \  .-\__  `-`  ___/-. /
//         ___`. .'  /--.--\  `. . __
//      ."" '<  `.___\_<|>_/___.'  >'"".
//    | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//    \  \ `-.   \_ __\ /__ _/   .-` /  /
//=====`-.____`-.___\_____/___.-`____.-'======
//                  `=---='
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//         佛祖保佑       永无BUG
/**
 * author: lynn on 2021/9/10 14:09
 * email:330159565@qq.com
 *
 * describe:
 */
class LawyerMeetingManager {
    companion object {
        val instance: LawyerMeetingManager by lazy(mode = LazyThreadSafetyMode.SYNCHRONIZED) { LawyerMeetingManager() }
    }

    fun showVideoView(
        activity: Activity,
        userid: String,
        userStreams: List<RCRTCInputStream>,
        rcLargePreview: ConstraintLayout,
        rcSmallPreview: ConstraintLayout,
        remoteCallback: (remoteVideoView: RCRTCVideoView) -> Unit,
        localCallback: (localVideoView: RCRTCVideoView) -> Unit
    ) {
        showSmallPreview(activity, rcSmallPreview, localCallback)
        for (inputStream in userStreams) {
            if (inputStream.mediaType == RCRTCMediaType.VIDEO) {
                //选择订阅大流或是小流。默认小流
                (inputStream as RCRTCVideoInputStream).streamType = RCRTCStreamType.NORMAL
                //创建VideoView并设置到stream
                val remoteVideoView = RCRTCVideoView(activity.applicationContext)
                remoteVideoView.tag = userid
                inputStream.videoView = remoteVideoView
                /*remoteVideoView.setZOrderOnTop(false)
                remoteVideoView.setZOrderMediaOverlay(false)*/
                //将远端视图添加至布局

                val lp: ConstraintLayout.LayoutParams = ConstraintLayout.LayoutParams(
                    ConstraintLayout.LayoutParams.MATCH_PARENT,
                    ConstraintLayout.LayoutParams.MATCH_PARENT
                )
                lp.startToStart = ConstraintLayout.LayoutParams.PARENT_ID
                lp.topToTop = ConstraintLayout.LayoutParams.PARENT_ID
                lp.endToEnd = ConstraintLayout.LayoutParams.PARENT_ID
                lp.bottomToBottom = ConstraintLayout.LayoutParams.PARENT_ID
                remoteVideoView.layoutParams = lp
                (inputStream as RCRTCVideoInputStream).videoView = remoteVideoView
                inputStream.setVideoFrameListener(object :
                    IRCRTCVideoInputFrameListener() {
                    override fun onFrame(videoFrame: RCRTCRemoteVideoFrame) {
                        var data =videoFrame.buffer as RCRTCRemoteVideoFrame.RTCBufferI420

                        println("融云->远端流>"+videoFrame.frameType
                                +"    >"+data.dataY+"   >"+data.width+"   >"+data.height)
                    }
                })
                inputStream.streamType = RCRTCStreamType.NORMAL
                //将远端视图添加至布局
                rcLargePreview.removeAllViews()
                rcLargePreview.addView(remoteVideoView)
                remoteCallback.invoke(remoteVideoView)
            }
        }
    }

    /**
     * 显示小屏画面
     */
    private fun showSmallPreview(
        activity: Activity,
        rcSmallPreview: ConstraintLayout,
        localCallback: (localVideoView: RCRTCVideoView) -> Unit
    ) {
        val rongRTCVideoView = RCRTCVideoView(activity.applicationContext)
        RCRTCEngine.getInstance().defaultVideoStream.videoView = rongRTCVideoView
        /*rongRTCVideoView.setZOrderOnTop(true)
        rongRTCVideoView.setZOrderMediaOverlay(true)*/
        rongRTCVideoView.scalingType = RendererCommon.ScalingType.SCALE_ASPECT_FILL
        rongRTCVideoView.tag = MainActivity.getUserId(MainActivity.b)
        rcSmallPreview.removeAllViews()
        val lp: ConstraintLayout.LayoutParams = ConstraintLayout.LayoutParams(
            ConstraintLayout.LayoutParams.MATCH_PARENT,
            ConstraintLayout.LayoutParams.MATCH_PARENT
        )
        lp.startToStart = ConstraintLayout.LayoutParams.PARENT_ID
        lp.topToTop = ConstraintLayout.LayoutParams.PARENT_ID
        lp.endToEnd = ConstraintLayout.LayoutParams.PARENT_ID
        lp.bottomToBottom = ConstraintLayout.LayoutParams.PARENT_ID
        rongRTCVideoView.layoutParams = lp
        rcSmallPreview.addView(rongRTCVideoView) //加入本地自己预览图
        RCRTCEngine.getInstance().defaultVideoStream.startCamera(null)
        localCallback.invoke(rongRTCVideoView)
    }

    /**
     * 是否禁用麦克风
     */
    fun isMicrophoneDisable(b: Boolean) {
        RCRTCEngine.getInstance().defaultAudioStream.isMicrophoneDisable = b
    }

}