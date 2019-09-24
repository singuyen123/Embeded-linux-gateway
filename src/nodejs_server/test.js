const PORT = 8080;
var assert = require('assert');
var express = require('express');
var cookieParser = require('cookie-parser');
var app = express();
app.use(express.static(__dirname + '/'));
var ip = require('ip');
var http = require('http');					//#include thu vien http -
var socketio = require('socket.io');			//#include thu vien socketio
var server = http.createServer(app);
var io = socketio(server);

server.listen(PORT, function () {
  console.log("Server running at address: " + ip.address() + ":" + PORT)
})

app.get('/', function (req, res) {
  res.sendfile('html/hello.html');
});
var allWebClients = [];
var mqtt = require('mqtt')
var mqtt_client = mqtt.connect('mqtt://test.mosquitto.org')

mqtt_client.on('connect', function () {
  mqtt_client.subscribe('presence', function (err) {
    
    if (!err) {
      mqtt_client.publish('presence', 'Hello mqtt');
    }
  })
})

mqtt_client.on('message', function (topic, message) {
  // message is Buffer
  console.log(message.toString())
  allWebClients.forEach(function(socket) {
    socket.emit("test", "msg from mqtt");
  })
})

io.on('connection', function (socket) {	//'connection' (1) nay khac gi voi 'connection' (2)
      console.log("Connected"); //In ra windowm console la da co mot Socket mqtt_client ket noi thanh cong.
      allWebClients.push(socket);
      //Gui di lenh 'welcome' voi mot tham so la mot bien JSON. Trong bien JSON nay co mot tham so va tham so do ten la message. Kieu du lieu cua tham so là mot chuoi.

      socket.on("type", function (message) {
        console.log("abc");
      })

      socket.on('disconnect', function() {
        console.log('Got disconnect!');
        var i = allWebClients.indexOf(socket);
        allWebClients.splice(i, 1);
      });
    })