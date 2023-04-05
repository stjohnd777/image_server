import UdpUtil from 'lm_udp'
 
 
function receiver( f ) {
    
    const aUdpChannel = new UdpUtil('localhost',7767)

    aUdpChannel.onMessage((msg,rinfo)=>{
        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
        if (f) {
            f(msg)
        }
    })


}

