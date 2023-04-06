const myudp = require('../../src/udp/upd_util.js')
const UdpUtil = require("../../src/udp/upd_util.js");


test("test", () => {

    const aUdpChannel = new UdpUtil('localhost', 7767)

    aUdpChannel.onMessage((msg, rinfo) => {
        console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
    })



    ['one', 'two', 'three', 'four', 'five'].forEach(m => aUdpChannel.send(m, (m) => {
        console.log(`send ${m} success!`)
    }))


    sendThese.forEach(m => aUdpChannel.send(m, (m) => {
            console.log(`send ${m} success!`)
        })
    )
})

// function test() {
//
//     const aUdpChannel = new UdpUtil('localhost',7767)
//
//     aUdpChannel.onMessage((msg,rinfo)=>{
//         console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
//     })
//
//     let sendThese = ['one', 'two','three', 'four', 'five']
//
//     sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{
//         console.log(`send ${m} success!`)})
//     )
//
//     sendThese.forEach ( m => aUdpChannel.send(m,(m)=>{
//         console.log(`send ${m} success!`)})
//     )
// }
// test()