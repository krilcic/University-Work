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

document.addEventListener("DOMContentLoaded", function () {
  const categoryButtons = document.querySelectorAll('.category-button');
  const productsContainer = document.querySelector('.products-container');
  const categoryElement = document.querySelector('.category');

  categoryButtons.forEach(button => {
    button.addEventListener('click', () => {
      productsContainer.innerHTML = '';

      const selectedCategory = data.categories.find(category => category.name === button.innerText);

      selectedCategory.products.forEach(product => {
        const productElement = `
          <div class="product">
            <img src="images/${product.image}" alt="${product.name}">
            <h3>${product.name}</h3>
            <div class="product-hover">
              <button class="add-to-cart" data-id="${product.name}">
                <img src="images/cart.jpg" alt="Add to Cart">
              </button>
            </div>
            <div class="quantity">
              <h5 data-name="${product.name}">${product.quantity}</h5>
            </div>
          </div>
        `;

        productsContainer.insertAdjacentHTML('beforeend', productElement);
      });

      categoryElement.querySelector('p').textContent = selectedCategory.name;

      const addToCartButtons = document.querySelectorAll('.add-to-cart');
      const quantityElements = document.querySelectorAll('.quantity h5');

      quantityElements.forEach(element => {
        const productName = element.parentNode.parentNode.querySelector('h3').textContent;

        if (cart[productName]) {
          element.textContent = cart[productName].quantity;
        }
      });

      addToCartButtons.forEach(button => {
        button.addEventListener('click', () => {
          const productName = button.parentNode.parentNode.querySelector('h3').textContent;

          if (!cart[productName]) {
            cart[productName] = {
              name: productName,
              quantity: 1
            };
          } else {
            cart[productName].quantity++;
          }

          localStorage.setItem('cart', JSON.stringify(cart));

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

          quantityElements.forEach(element => {
            if (element.parentNode.parentNode.querySelector('h3').textContent === productName) {
              element.textContent = cart[productName].quantity;
            }
          });
        });
      });
    });
  });
});