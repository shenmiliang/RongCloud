package com.hl.rongclound

import android.Manifest
import android.os.Bundle
import android.view.SurfaceView
import android.view.View
import cn.rongcloud.rtc.api.RCRTCEngine
import cn.rongcloud.rtc.api.RCRTCRemoteUser
import cn.rongcloud.rtc.api.RCRTCRoom
import cn.rongcloud.rtc.api.callback.IRCRTCResultCallback
import cn.rongcloud.rtc.api.callback.IRCRTCResultDataCallback
import cn.rongcloud.rtc.api.callback.IRCRTCRoomEventsListener
import cn.rongcloud.rtc.api.callback.IRCRTCVideoOutputFrameListener
import cn.rongcloud.rtc.api.stream.RCRTCInputStream
import cn.rongcloud.rtc.base.RCRTCVideoFrame
import cn.rongcloud.rtc.base.RTCErrorCode
import com.android.huangl.permission.Permission
import com.android.huangl.permission.PermissionResult
import com.blankj.utilcode.util.ToastUtils
import io.rong.imlib.RongIMClient
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : BaseActivity() {
    private var mRcrtcRoom: RCRTCRoom? = null
    private var mLocalVideo: SurfaceView? = null //本地视图
    private var remoteVideo: SurfaceView? = null //远端视图
    private var bgImage: ByteArray? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        println("融云->onCreate->")
        initView()
        initPermission()
        initConnect()
    }

    private fun initView() {
        btnStart.setOnClickListener {
            joinRoom()
        }
        btnEnd.setOnClickListener {
            exitRCRoom()
        }

    }

    private fun exitRCRoom() {
        rcLargePreview.removeAllViews()
        rcSmallPreview.removeAllViews()
        mRcrtcRoom?.unregisterRoomListener()
        RCRTCEngine.getInstance().defaultVideoStream?.stopCamera()
        RCRTCEngine.getInstance().unregisterStatusReportListener()
        RCRTCEngine.getInstance().leaveRoom(null)

    }

    private fun joinRoom() {
        progressBar.visibility = View.VISIBLE
        RCRTCEngine.getInstance()
            .joinRoom("13243749987", object : IRCRTCResultDataCallback<RCRTCRoom>() {
                override fun onSuccess(rcrtcRoom: RCRTCRoom) {
                    runOnUiThread {
                        this@MainActivity.mRcrtcRoom = rcrtcRoom
                        rcrtcRoom.registerRoomListener(RoomEventsListener())
                        publishDefaultAVStream(rcrtcRoom)
                        subscribeAVStream(rcrtcRoom)
                    }

                }

                override fun onFailed(rtcErrorCode: RTCErrorCode) {
                    println("融云->加入房间失败>${rtcErrorCode.reason} : ${rtcErrorCode.name} : ${rtcErrorCode.value}")
                }
            })
        RCRTCEngine.getInstance().defaultVideoStream.setVideoFrameListener(object :
            IRCRTCVideoOutputFrameListener() {
            override fun processVideoFrame(rtcVideoFrame: RCRTCVideoFrame): RCRTCVideoFrame {
                //使用数据进行美颜/录像等处理后，需要把数据再返回给SDK做发送

                println("aaaaaaad1>" + rtcVideoFrame.data.size + "    w>" + rtcVideoFrame.width + "    >" + rtcVideoFrame.height + "    >")

                var res = bgImage?.let {
                        matting(rtcVideoFrame.width, rtcVideoFrame.height, rtcVideoFrame.data)
                }

                rtcVideoFrame.data = res
                return rtcVideoFrame


            }
        })
    }

    inner class RoomEventsListener : IRCRTCRoomEventsListener() {
        override fun onRemoteUserPublishResource(
            remoteUser: RCRTCRemoteUser?,
            streams: MutableList<RCRTCInputStream>?
        ) {
            runOnUiThread {
                showVideoView(remoteUser!!.userId, streams!!)
                subscribeStreams(streams)
            }

        }

        override fun onRemoteUserMuteAudio(
            remoteUser: RCRTCRemoteUser?,
            stream: RCRTCInputStream?,
            mute: Boolean
        ) {
        }

        override fun onRemoteUserMuteVideo(
            remoteUser: RCRTCRemoteUser?,
            stream: RCRTCInputStream?,
            mute: Boolean
        ) {
        }

        override fun onRemoteUserUnpublishResource(
            remoteUser: RCRTCRemoteUser?,
            streams: MutableList<RCRTCInputStream>?
        ) {
        }

        override fun onUserJoined(remoteUser: RCRTCRemoteUser?) {
        }

        override fun onUserLeft(remoteUser: RCRTCRemoteUser?) {
        }

        override fun onUserOffline(remoteUser: RCRTCRemoteUser?) {
        }

        override fun onPublishLiveStreams(streams: MutableList<RCRTCInputStream>?) {
        }

        override fun onUnpublishLiveStreams(streams: MutableList<RCRTCInputStream>?) {
        }

    }

    /**
     * 加入房间成功后，发布默认音视频流
     */
    private fun publishDefaultAVStream(rcrtcRoom: RCRTCRoom) {
        rcrtcRoom.localUser.publishDefaultStreams(object : IRCRTCResultCallback() {
            override fun onSuccess() {
                println("融云->发布资源成功")
            }

            override fun onFailed(rtcErrorCode: RTCErrorCode) {
                println("融云->发布资源失败:${rtcErrorCode.value},${rtcErrorCode.reason}")
            }
        })
    }

    /**
     * //加入房间成功后，如果房间中已存在用户且发布了音、视频流，就订阅远端用户发布的音视频流.
     */
    private fun subscribeAVStream(rtcRoom: RCRTCRoom?) {
        if (rtcRoom == null || rtcRoom.remoteUsers == null) {
            return
        }
        val inputStreams: MutableList<RCRTCInputStream> = ArrayList()
        for (remoteUser in mRcrtcRoom!!.remoteUsers) {
            if (remoteUser.streams.size == 0) {
                continue
            }
            val userStreams = remoteUser.streams
            showVideoView(remoteUser.userId, userStreams)
            inputStreams.addAll(remoteUser.streams)
        }
        if (inputStreams.size == 0) {
            return
        }
        subscribeStreams(inputStreams)
    }

    /**
     * 订阅流
     */
    private fun subscribeStreams(list: List<RCRTCInputStream?>?) {
        mRcrtcRoom!!.localUser.subscribeStreams(list, object : IRCRTCResultCallback() {
            override fun onSuccess() {
                println("融云->订阅成功")
            }

            override fun onFailed(rtcErrorCode: RTCErrorCode) {
                println("融云->订阅失败")
            }
        })
    }

    /**
     * 显示视频画面
     */
    private fun showVideoView(userId: String, streams: List<RCRTCInputStream>) {
        println("融云->UserId->$userId")
        runOnUiThread {
            progressBar.visibility = View.GONE
        }

        LawyerMeetingManager.instance.showVideoView(
            this,
            userId,
            streams,
            rcLargePreview,
            rcSmallPreview,
            {
                remoteVideo = it
            },
            {
                mLocalVideo = it
            })

        setZOrderOnTop(mLocalVideo, remoteVideo)
    }

    private fun setZOrderOnTop(sv1: SurfaceView?, sv2: SurfaceView?) {
        sv1?.setZOrderMediaOverlay(true)
        sv1?.setZOrderOnTop(true)
        sv2?.setZOrderMediaOverlay(false)
        sv2?.setZOrderOnTop(false)
    }

    private fun initPermission() {
        Permission.with(this)
            .addRequestCode(100)
            .addPermissions(
                Manifest.permission.CAMERA,
                Manifest.permission.RECORD_AUDIO
            )
            .addInterface(object : PermissionResult {
                override fun result(code: Int) {

                }

                override fun faild(code: Int) {
                    ToastUtils.showShort("需要摄像头权限跟麦克风权限")
                }
            }).submit()

    }

    private fun initConnect() {
        RongIMClient.connect(
            getToken(b),
            object : RongIMClient.ConnectCallback() {
                override fun onSuccess(userId: String) {
                    println("融云->连接成功>$userId")
                }

                override fun onError(code: RongIMClient.ConnectionErrorCode) {
                    println("融云->连接失败>${code?.value}: ${code?.name}: ${code?.ordinal}")
                }

                override fun onDatabaseOpened(databaseOpenStatus: RongIMClient.DatabaseOpenStatus) {
                    // 数据库打开失败
                }
            })
    }


    companion object {
        init {
            System.loadLibrary("face_ai")
        }
        //userID --  hl_test1  -- token -- NP4oX/6UXq2Kie7xV+atMw8IeW4HwNhJS4FHHk3OCEmU/au3UcNPiPTXGDAqJxDEV9HVbg==
        //userID --  hl_test2  -- token -- NP4oX12RxJ0QtbMTzHtnOvJf+YvSlwLsWsjICU3OCEmU/au36CZQbeVRb1AqJxDEV9HVbg==
        var b = true //true hl_test1  false hl_test2

        fun getUserId(b: Boolean): String {
            return if (b) {
                "hl_test1"
            } else "hl_test2"
        }

        fun getToken(b: Boolean): String {
            return if (b) {
                "NP4oX/6UXq2Kie7xV+atMw8IeW4HwNhJS4FHHk3OCEmU/au3UcNPiPTXGDAqJxDEV9HVbg=="
            } else "NP4oX12RxJ0QtbMTzHtnOvJf+YvSlwLsWsjICU3OCEmU/au36CZQbeVRb1AqJxDEV9HVbg=="
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        RongIMClient.getInstance().logout()
        exitRCRoom()
    }

    external fun matting(width: Int, height: Int, src: ByteArray): ByteArray

}