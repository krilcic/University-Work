var express = require('express');
var router = express.Router();

router.get('/', function (req, res) {
    res.render('cart', {
        title: 'Cart',
        cart: req.session.cart,
        name: 'Car Heaven'
    });
});

router.post('/add', function (req, res) {
    if (req.session.cart === undefined) {
        req.session.cart = {};
    }
    res.json({ succes: true });
});

router.post('/add/:id([0-9]{1,10})', function (req, res) {
    let name = req.body.name;
    let value = req.session.cart[name];

    if (value === undefined) {
        req.session.cart[name] = 1;
    } else {
        req.session.cart[name] = value + 1;
    }
    res.send("Added to cart");
});

router.delete('/remove/:id([0-9]{1,10})', function (req, res) {
    let name = req.body.name;

    if (req.session.cart === undefined) {
        req.session.cart = {};
    }

    let value = req.session.cart[name];

    if (!(value === undefined)) {
        if (value > 1) {
            req.session.cart[name] = value - 1;
            res.send("Removed 1 from cart");
        } else {
            delete req.session.cart[name];
            res.send("Removed product from cart");
        }
    }
});

router.get('/getAll', function (req, res) {
    let cart = req.session.cart;
    res.json(cart);
});

module.exports = router;