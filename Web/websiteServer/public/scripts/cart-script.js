async function renderCartItems() {
  const cart = await fetch('http://localhost:3000/cart/getall').then(response => response.json());
  var data = await fetch('http://localhost:3000/home/get').then(response => response.json());

  const cartButton = document.querySelector('.cart-search');
  const cartTotal = cartButton.querySelector('span');
  const cartTotall = document.querySelector('.cart-total');

  let total = 0;
  for (const productName in cart) {
    total += cart[productName];
  }

  if (total > 0) {
    cartTotal.textContent = total;
  }

  if (total === 0) {
    cartTotall.style.display = 'none';
  } else {
    cartTotall.style.display = 'block';
  }

  const cartItemsContainer = document.querySelector('#cart-items');
  cartItemsContainer.innerHTML = '';
  if (total === 0) {
    cartItemsContainer.innerHTML = 'Vaša košarica je prazna!';
  }

  Object.entries(cart).forEach(item => {
    const cartItemElement = document.createElement('div');
    cartItemElement.classList.add('cart-item');

    const nameElement = document.createElement('h3');
    nameElement.textContent = item[0];

    let productName = nameElement.textContent;

    let productId;

    for (const category of data.categories) {
      for (const product of category.products) {
        if (product.name === productName) {
          productId = product.id;
        }
      }
    }

    const removeButton = document.createElement('button');
    removeButton.textContent = '-';
    removeButton.addEventListener('click', async () => {
      try {
        await fetch(`/cart/remove/${productId}`, {
          method: "DELETE",
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ name: productName })
        })
          .then(res => res.text())
          .then(data => {
            console.log("Request remove 1 complete! Response:", data);
          })
          .catch(error => {
            console.error("Error:", error);
          });

        const updatedCart = await fetch('http://localhost:3000/cart/getall').then(response => response.json());

        let updatedTotal = 0;

        for (const productName in updatedCart) {
          updatedTotal += updatedCart[productName];
        }

        if (updatedTotal > 0) {
          cartTotal.textContent = updatedTotal;
        }

        if (updatedTotal === 0) {
          cartTotall.style.display = 'none';
        } else {
          cartTotall.style.display = 'block';
        }
      } catch (error) {
        console.error("Error:", error);
      }

      quantityElement.textContent = item[1];
      renderCartItems();
    });

    const quantityElement = document.createElement('h3');
    quantityElement.textContent = item[1];

    const addButton = document.createElement('button');
    addButton.textContent = '+';

    addButton.addEventListener('click', async () => {
      try {
        await fetch(`/cart/add/${productId}`, {
          method: "POST",
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify({ name: productName })
        })
          .then(res => res.text())
          .then(data => {
            console.log("Request add complete! Response:", data);
          })
          .catch(error => {
            console.error("Error:", error);
          });

        const updatedCart = await fetch('http://localhost:3000/cart/getall').then(response => response.json());

        let updatedTotal = 0;

        for (const productName in updatedCart) {
          updatedTotal += updatedCart[productName];
        }

        if (updatedTotal > 0) {
          cartTotal.textContent = updatedTotal;
        }

        if (updatedTotal === 0) {
          cartTotall.style.display = 'none';
        } else {
          cartTotall.style.display = 'block';
        }
      } catch (error) {
        console.error("Error:", error);
      }

      quantityElement.textContent = item[1];
      renderCartItems();
    });

    cartItemElement.appendChild(nameElement);
    cartItemElement.appendChild(removeButton);
    cartItemElement.appendChild(quantityElement);
    cartItemElement.appendChild(addButton);
    cartItemsContainer.appendChild(cartItemElement);
  });
}

document.addEventListener('DOMContentLoaded', () => {
  renderCartItems();
});