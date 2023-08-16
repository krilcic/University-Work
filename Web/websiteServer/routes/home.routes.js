var express = require('express');
var router = express.Router();
const data = require('../data/mydata');

router.get('/', function (req, res) {
    res.render('home', {
        title: 'Home',
        show_categories: false,
        show_products: false,
        data: data,
        name: 'Car Heaven'
    });
});

router.get('/getCategories', function (req, res) {
    res.json(data.categories);
    console.log(data.categories);
});

router.get('/getProducts/:id([0-9]{1,10})', function (req, res) {
    res.json(data.categories[req.params.id - 1].products);
});

router.get('/Categories', function (req, res) {
    res.render('home', {
        title: 'Categories',
        show_categories: true,
        show_products: false,
        data: data,
        name: 'Car Heaven'
    });
});

router.get('/Products/:id([0-9]{1,10})', function (req, res) {
    res.render('home', {
        title: 'Products',
        show_categories: true,
        show_products: true,
        data: data,
        id: req.params.id,
        cat: data.categories[req.params.id - 1],
        name: data.categories[req.params.id - 1].name
    });
});

router.get('/get', function (req, res) {
    res.json(data);
});

module.exports = router;