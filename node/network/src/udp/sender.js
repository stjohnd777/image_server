const  UdpUtil  = require('upd_util')

async function send(sendList) {
    const aUdpChannel = new UdpUtil('localhost',7767)

    sendList.forEach ( m => aUdpChannel.send(m,(m)=>{
        console.log(`send ${m} success!`)}) 
    )
}

module.exports = send

