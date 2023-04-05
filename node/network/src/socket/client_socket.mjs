import net from 'net'
import fs from 'fs'


const root = 'C:/Users/e438262/WebstormProjects/network'
const base_dir = `${root}/out`
const img_type = "png"

export async function requestImage(ip, port, cameraId = 0, uuid, callback = undefined) {
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
export async function startImageSocketClient(ip, port, cameraId = 0, uuid = Date.now()) {
    while (isRunning) {
        const data = await requestImage(ip, port, `${uuid}|${cameraId}`)
        console.log(data)
    }

}

requestImage("127.0.0.1", 7767, '0', '1356757191530').then(r => console.log('then'))


//startCameraServer("127.0.0.1", 7767, 0).then(r => console.log('then'))