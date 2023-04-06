const dgram = require( 'dgram.js')

class UdpUtil {

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

module.exports = UdpUtil

