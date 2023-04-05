import dgram from 'dgram.js'

export default class UdpUtil {

    constructor(host,port){
        
        this.host = host
        this.port = port
        this.server = dgram.createSocket('udp4');

        this.server.on('error', (err) => {
          console.log(`server error:\n${err.stack}`);
          this.server.close();
        });
         
        this.server.on('listening', () => {
          const address = this.server.address();
          console.log(`server listening ${address.address}:${address.port}`);
        });
        
        this.server.bind(port);
    }

    send (data, callback) {
        this.server.send(data, 0, data.length, this.port , this.host, (err, bytes) => {
           if (callback) {
            callback(data);
           }
        })
    }

    onMessage(handler) {
        this.server.on('message', (msg, rinfo) => {
            if ( handler) {
                handler(msg,rinfo);
            }
          })
    }
}

function test() {
    
    const aUdpChannel = new UdpUtil('localhost',7767)

    aUdpChannel.onMessage((msg,rinfo)=>{
        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
    })

    let sendThese = ['one', 'two','three', 'four', 'five']

    sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{ 
        console.log(`send ${m} success!`)}) 
    )

    sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{ 
        console.log(`send ${m} success!`)}) 
    )
}

test()