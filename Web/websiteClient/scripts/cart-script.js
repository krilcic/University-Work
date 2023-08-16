let cart = {};
const storedCart = localStorage.getItem('cart');

if (storedCart) {
  cart = JSON.parse(storedCart);
}

const cartButton = document.querySelector('.cart-search');
const cartTotal = cartButton.querySelector('span');
const cartTotall = document.querySelector('.cart-total');

let total = 0;

for (const productName in cart) {
  total += cart[productName].quantity;
}

if (total > 0) {
  cartTotal.textContent = total;
}

if (total === 0) {
  cartTotall.style.display = 'none';
} else {
  cartTotall.style.display = 'block';
}


const cartItems = JSON.parse(localStorage.getItem('cart')) || {};

function renderCartItems() {
  const cartItemsContainer = document.querySelector('#cart-items');
  cartItemsContainer.innerHTML = '';

  Object.values(cartItems).forEach(item => {
    const cartItemElement = document.createElement('div');
    cartItemElement.classList.add('cart-item');

    const nameElement = document.createElement('h3');
    nameElement.textContent = item.name;

    const removeButton = document.createElement('button');
    removeButton.textContent = '-';
    removeButton.addEventListener('click', () => {
      item.quantity--;
      localStorage.setItem('cart', JSON.stringify(cartItems));
      quantityElement.textContent = item.quantity;
      if (item.quantity === 0) {
        removeFromCart(item.name);
      } else {
        saveCart();
        renderCartItems();
      }
    });

    const quantityElement = document.createElement('h3');
    quantityElement.textContent = item.quantity;

    const addButton = document.createElement('button');
    addButton.textContent = '+';
    addButton.addEventListener('click', () => {
      item.quantity++;
      localStorage.setItem('cart', JSON.stringify(cartItems));
      quantityElement.textContent = item.quantity;
      saveCart();
    });

    cartItemElement.appendChild(nameElement);
    cartItemElement.appendChild(removeButton);
    cartItemElement.appendChild(quantityElement);
    cartItemElement.appendChild(addButton);
    cartItemsContainer.appendChild(cartItemElement);
  });
}

function saveCart() {
  localStorage.setItem('cart', JSON.stringify(cartItems));
  let cart = {};
  const storedCart = localStorage.getItem('cart');

  if (storedCart) {
    cart = JSON.parse(storedCart);
  }

  const cartButton = document.querySelector('.cart-search');
  const cartTotal = cartButton.querySelector('span');

  let total = 0;

  for (const productName in cart) {
    total += cart[productName].quantity;
  }

  if (total > 0) {
    cartTotal.textContent = total;
  }

  if (total === 0) {
    cartTotall.style.display = 'none';
  } else {
    cartTotall.style.display = 'block';
  }
}

function removeFromCart(productName) {
  delete cartItems[productName];
  saveCart();
  renderCartItems();
}


document.addEventListener('DOMContentLoaded', () => {
  renderCartItems();
});