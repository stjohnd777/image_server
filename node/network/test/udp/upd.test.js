const UdpUtil = require("../../src/udp/upd_util.js");
const assert = require('assert');
describe('udp_ utils', function () {

    it('should send', function () {

        const aUdpChannel = new UdpUtil('localhost', 7767)

        let messageHandler =  (msg, info) => {
            console.log(`server got: ${msg} from ${info.address}:${info.port}`);
        }
        aUdpChannel.onMessage(messageHandler)

        ['one', 'two', 'three', 'four', 'five'].forEach(
            m => aUdpChannel.send(m, (m) => {
                console.log(`send ${m} success!`)
            })
        )

    });

});
