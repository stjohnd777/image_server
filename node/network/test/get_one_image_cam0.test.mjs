import {requestImage} from '../src/socket/client_socket.mjs'
import {startImageSockeServer} from "../src/socket/server_socket.mjs";

let host = "127.0.0.1"
let port = 7767
let cameraId = '0'
let uuid = '1356757191530'

beforeEach(() => {
    startImageSockeServer();
});

afterEach(() => {

});

test('get image from camera 0 with uuid', async () => {
    await pathToImgFile = requestImage(host, port, cameraId, uuid)
});