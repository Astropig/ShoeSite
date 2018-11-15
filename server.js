const express = require('express');
const hbs = require('hbs');
const fs = require('fs');
const port = process.env.PORT || 3000;
var app = express();


app.set('view engine', 'hbs');
//app.use(express.static(__dirname + '/html'));

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

app.get('/', (req, res) => {
    var mytext = req.query.colorValue;
    console.log(mytext);
    res.render('home.hbs',{
        currentColor: 'Current Value: ' + mytext
    });
});
app.listen(port, () => {
    console.log(`Server is up on port ${port.toString()}`);
});