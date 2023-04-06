const config = require ("../img_server_config.json")
const {StartImageSocketServer} = require("./server_socket");

let socket = StartImageSocketServer(config.server_port)