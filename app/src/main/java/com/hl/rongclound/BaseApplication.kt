package com.hl.rongclound

import android.app.Application
import android.widget.Toast
import cn.rongcloud.rtc.api.RCRTCConfig
import cn.rongcloud.rtc.api.RCRTCEngine
import io.rong.common.utils.SSLUtils
import io.rong.imlib.RongIMClient
import java.security.SecureRandom
import java.security.cert.CertificateException
import java.security.cert.X509Certificate
import javax.net.ssl.SSLContext
import javax.net.ssl.X509TrustManager


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
 * author: lynn on 2022/9/6 10:02
 * email:330159565@qq.com
 *
 * describe:
 */
class BaseApplication: Application() {
    override fun onCreate() {
        super.onCreate()
        initSSLSocketFactory()
        RongIMClient.setServerInfo("https://rongyun.yuwugongkai.com:11444", "https://rongyun.yuwugongkai.com:11446")
        RongIMClient.init(this,"p5tvi99k7ty34")
        val config = RCRTCConfig.Builder.create()
        RCRTCEngine.getInstance().init(this, config.build())
        //是否硬解码
        config.enableHardwareDecoder(true)
        //是否硬编码
        config.enableHardwareEncoder(true)
        config.enableEncoderTexture(false)
        RCRTCEngine.getInstance().init(this, config.build())
        connectionStatusListener()
    }

    private fun connectionStatusListener() {
        RongIMClient.setConnectionStatusListener { connectionStatus ->
            println("融云->连接状态>"+connectionStatus.message+"   >"+connectionStatus.value+"   >"+connectionStatus.name)
            if (connectionStatus == RongIMClient.ConnectionStatusListener.ConnectionStatus.KICKED_OFFLINE_BY_OTHER_CLIENT) {
                //被其他提出时，需要返回登录界面
                RongIMClient.getInstance().logout()
            } else if (connectionStatus == RongIMClient.ConnectionStatusListener.ConnectionStatus.TIMEOUT) {
                println("融云->超时>")
            }
        }
    }

    /**
     * 对所有站点的信任
     *
     * @return SSLSocketFactory工厂对象
     */
    fun initSSLSocketFactory() {
        //创建加密上下文
        var sslContext: SSLContext? = null
        try {
            //这里要与服务器的算法类型保持一致TSL/SSL
            sslContext = SSLContext.getInstance("SSL")
            val xTrustArray = arrayOf(initTrustManager())
            sslContext.init(null, xTrustArray, SecureRandom())
            SSLUtils.setSSLContext(sslContext)
            SSLUtils.setHostnameVerifier { s, sslSession -> true }
        } catch (e: java.lang.Exception) {
            e.printStackTrace()
        }
    }


    /**
     * 生成TrustManager信任管理器类
     *
     * @return X509TrustManager
     */
    fun initTrustManager(): X509TrustManager {
        return object : X509TrustManager {
            override fun getAcceptedIssuers(): Array<X509Certificate> {
                return arrayOf()
            }

            @Throws(CertificateException::class)
            override fun checkServerTrusted(chain: Array<X509Certificate>, authType: String) {
            }

            @Throws(CertificateException::class)
            override fun checkClientTrusted(chain: Array<X509Certificate>, authType: String) {
            }
        }
    }
}