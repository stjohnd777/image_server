const net = require('net')
const fs = require( 'fs')



let base_dir = 'C:/Users/e438262/WebstormProjects/network/out'
let img_type = "png"

async function requestImage(ip, port, cameraId = 0, uuid, callback = undefined) {
    const socket = net.Socket();
    socket.connect(port, ip);
    let request = `${uuid}|${cameraId}`
    socket.write(request);
    if (callback) {
        try {
            socket.on('data', (buffer) => {
                console.log(`received ${buffer.len} bytes`);
                callback(buffer)
            });
        } catch (e) {
            throw e
        }
    } else {
        return new Promise((resolve, reject) => {
            try {
                socket.on('data', (buffer) => {
                    const pathToImgFile = `${base_dir}/cam${cameraId}/${uuid}.${img_type}`
                    fs.writeFile(pathToImgFile, buffer, err => {
                        console.log(`${buffer.len} bytes written to ${pathToImgFile}`);
                        resolve(pathToImgFile)
                    });
                });
            } catch (e) {
                reject(e)
            }
        })
    }
}

let isRunning = true
async function StartImageSocketClient(ip, port, cameraId = 0, delay = 1000) {
    setInterval (async ()=>{
        let uuid = Date.now()
        const pathToImgFile = await requestImage(ip, port, `${uuid}|${cameraId}`)
        console.log(`path to retrieved image: ${pathToImgFile} : delay ${delay}`)
    }, delay)

}

module.exports = {
    requestImage : requestImage,
    StartImageSocketClient : StartImageSocketClient
}

// requestImage("127.0.0.1", 7767, '0', '1356757191530').then(r => console.log('then'))
//startCameraServer("127.0.0.1", 7767, 0).then(r => console.log('then'))