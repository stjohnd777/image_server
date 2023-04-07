const config = require ('../img_client_config.json')
const {StartImageSocketClient,requestImage} = require("./client_socket");

requestImage(config.server_ip, config.server_port, '0', '1356757191530').then(r => console.log(r))
