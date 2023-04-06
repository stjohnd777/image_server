const net = require ( 'net')
const fs = require ( 'fs' )
const config = require ('../img_server_config.json')

function providerGetImageFromFilePath (cameraId, uuid) {
    const pathToFile = `${config.server_img_dir}/cam${cameraId}_${uuid}.png`
    return fs.readFileSync(pathToFile)
}

function providerGetImageFromNearestTime (cameraId, time) {

    fs.readdir (config.server_img_dir, (err,files)=>{

        let leastDeltaFile = files[0]
        let leastDelta = 999999999
        files.forEach( file =>{
            const { birthtime } = fs.statSync(file)
            const diff = time > birthtime ? time-birthtime : birthtime -time
            if ( diff < leastDelta ) {
                leastDeltaFile = file
                leastDelta = diff
            }
        })
        const pathToFile = `${config.server_img_dir}/${leastDeltaFile.getName()}`
        return fs.readFileSync(pathToFile)

    })

}


function startImageSocketServer(port){
    let server  = net.createServer((socket) => {

        console.log("connected");

        socket.on('data', function (data) {

            const strData = data.toString()
            const theSplit = strData.split("|")
            const uuid = theSplit[0]
            const cameraId = theSplit[1]

            const imgData = providerGetImageFromFilePath(cameraId,uuid)

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

module.exports = {
    provider : providerGetImageFromFilePath,
    StartImageSocketServer: startImageSocketServer
}
