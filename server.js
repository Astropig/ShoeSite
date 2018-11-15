
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

io.on('connection', (socket) => {
   console.log("new user");

   socket.on('createMessage', (message) =>{
       io.emit('newMessage', {
           from: message.from,
           text: message.text
       });
   })
});

app.use((req, res, next) => {
    var now = new Date().toString();
    var log = `${now}: ${req.method} ${req.url} ${req.ip}`;

    console.log(log);
    fs.appendFile('server.log', log + '\n', (err) => {
        if (err){
            console.log('unable to append');
        }
    });
    next();
});
//http.createServer
app.get('/', (req, res) => {
    mytext = req.query.colorValue;
    console.log(mytext);

    res.render('home.hbs',{
        currentColor: 'Current Value: ' + mytext
    });
});
server.listen(port, () => {
    console.log(`Server is up on port ${port.toString()}`);
});