
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
       var log = message.text;
       fs.unlink('server.txt', (err)=>{
           if (err){
               console.log(err);
           }
           fs.writeFile('server.txt', message.text, (err) => {
               if (err) {
                   console.log(err);
               } else {

                   console.log('Data uploaded')
               }
           });
       });


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
    fs.readFile('server.txt', function read(err,data) {
        if (err){
            console.log(err);
        }
        res.render('home.hbs',{
            currentColor: 'Current Value: ' + data
        });

    });

});
server.listen(port, () => {
    console.log(`Server is up on port ${port.toString()}`);
});