import UdpUtil from 'lm_udp'
 
 
 
async function send(sendThese) {
    
    const aUdpChannel = new UdpUtil('localhost',7767)

    sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{ 
        console.log(`send ${m} success!`)}) 
    )

}


