const express = require('express');
const app = express();
var path = require('path');
const session = require('express-session');
const FileStore = require('session-file-store')(session);

app.use(express.urlencoded({ extended: false }));
app.use(express.json());

app.use(session({
    secret: 'secret',
    store: new FileStore(),
    saveUninitialized: true,
    cookie: { maxAge: 600000 }
}));

var hr = require('./routes/home.routes');
var cr = require('./routes/cart.routes');

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));

app.use('/home', hr);
app.use('/cart', cr);

app.listen(3000);