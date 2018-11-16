
const express = require('express');
const hbs = require('hbs');
const fs = require('fs');
const socketIO = require('socket.io');
const http =  require('http');
const port = process.env.PORT || 3000;
var app = express();
var server = http.createServer(app);
var io = socketIO(server);
app.set('view engine', 'hbs');
var mytext = " ";
fs.appendFile('server.log', ' ' );

io.on('connection', (socket) => {
   console.log("new user");

   socket.on('createMessage', (message) =>{
       io.emit('newMessage', {
           from: message.from,
           text: message.text
       });
       var log = message.text;
       try {
           fs.unlink('server.log');
           fs.appendFile('server.log', log );
       }catch (e) {
           fs.appendFile('server.log', log );
       }

   })
});

app.use((req, res, next) => {
    var now = new Date().toString();
    var log = `${now}: ${req.method} ${req.url} ${req.ip}`;

    console.log(log);
    next();
});
//http.createServer
app.get('/', (req, res) => {
    fs.readFile('server.log', function read(err,data) {
       if (err){
           throw err;
       }
        res.render('home.hbs',{
            currentColor: 'Current Value: ' + data
        });
        console.log(data);

    });


});
server.listen(port, () => {
    console.log(`Server is up on port ${port.toString()}`);
});