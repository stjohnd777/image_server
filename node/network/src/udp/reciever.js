const UdpUtil = require('upd_util')
function receiver( host,port,handler ) {
    const aUdpChannel = new UdpUtil(host,port)

    aUdpChannel.onMessage((msg,rinfo)=>{
        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
        if (handler) {
            handler(msg)
        }
    })
}

module.exports = receiver

