import net from 'net'
import fs from 'fs'

const root = 'C:/Users/e438262/WebstormProjects/network'
const base_dir = `${root}/data/star-data/wFOV`
const img_type = "png"

function providerGetImageFromFile (cameraId, uuid) {
    const pathToFile = `${base_dir}/cam${cameraId}_${uuid}.png`
    return fs.readFileSync(pathToFile)
}


export function startImageSockeServer(port){
    let server  = net.createServer((socket) => {

        console.log("connected");

        socket.on('data', function (data) {

            const strData = data.toString()
            const theSplit = strData.split("|")
            const uuid = theSplit[0]
            const cameraId = theSplit[1]

            const imgData = providerGetImageFromFile(cameraId,uuid)

            // parse message

            console.log(`server received data ${imgData}`)
            socket.write(imgData)
        });
    })

    server.listen(port,()=>{
        console.log("listening")
    });

    return server;
}

startImageSockeServer(7767)
