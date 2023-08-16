const express = require('express');
const app = express();
const path = require('path');
const session = require('express-session');

app.use(express.urlencoded({ extended: true }));
app.use(express.json());

app.use(express.static(path.join(__dirname, 'public')));

app.use(session({
    secret: 'secret',
    resave: false,
    saveUninitialized: true,
}));

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

let kategorije = ['Work', 'Movies and books', 'Fun', 'Sports']
let globalCounter = 0;

app.get('/', (req, res) => {
    if (req.session.podsjetnik === undefined) {
        req.session.podsjetnik = {};

        for (let i of kategorije) {
            req.session.podsjetnik[i] = [];
        }

        req.session.counter = 0;
    }

    res.render('home',
        {
            kategorije: kategorije,
            podsjetnik: req.session.podsjetnik,
            globalCounter: globalCounter,
            selfCounter: req.session.counter
        }
    )
});

app.post('/add', (req, res) => {
    let kategorija = req.body.dropdown;
    let aktivnost = req.body.activity;

    globalCounter++;
    req.session.counter++;

    let todo = {};
    todo.aktivnost = aktivnost;
    const dat = new Date();
    todo.date = dat.toLocaleDateString();

    req.session.podsjetnik[kategorija].push(todo);

    res.redirect('/');
});

app.post('/delete', (req, res) => {
    let kategorija = req.body.katbrisanje;
    let index = req.body.redbrisanje;
    let novi = [];

    globalCounter--;
    req.session.counter--;

    for (let i = 0; i < req.session.podsjetnik[kategorija].length; i++) {
        if (i != index) {
            novi.push(req.session.podsjetnik[kategorija][i]);
        }
    }

    req.session.podsjetnik[kategorija] = novi;

    res.redirect('/')
});

const PORT = 8080;
app.listen(PORT, () => {
    console.log(`Listening on port ${PORT}!`);
})