document.addEventListener("DOMContentLoaded", async function () {
  await fetch("/cart/add", {
    method: "POST",
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({})
  })
    .then(res => res.json())
    .then(data => {
      console.log("Add to cart response:", data);
    })
    .catch(error => {
      console.error("Add to cart error:", error);
    });

  async function updateCart() {
    try {
      const cart = await fetch('http://localhost:3000/cart/getall').then(response => response.json());

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

      var data = await fetch('http://localhost:3000/home/get').then(response => response.json());

      const addToCartButtons = document.querySelectorAll('.add-to-cart');
      const quantityElements = document.querySelectorAll('.quantity h5');

      quantityElements.forEach(element => {
        const productName = element.parentNode.parentNode.querySelector('h3').textContent;

        if (cart[productName]) {
          element.textContent = cart[productName];
        }
      });

      addToCartButtons.forEach(button => {
        button.addEventListener('click', async () => {
          const productName = button.parentNode.parentNode.querySelector('h3').textContent;
          let productId;
          for (const category of data.categories) {
            for (const product of category.products) {
              if (product.name === productName) {
                productId = product.id;
              }
            }
          }

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

            quantityElements.forEach(element => {
              if (element.parentNode.parentNode.querySelector('h3').textContent === productName) {
                element.textContent = updatedCart[productName];
              }
            });
          } catch (error) {
            console.error("Error:", error);
          }
        });
      });
    } catch (error) {
      console.error("Error:", error);
    }
  }

  updateCart();

  var data = await fetch('http://localhost:3000/home/get').then(response => response.json());
  const categoryButtons = document.querySelectorAll('.category-button');
  const productsContainer = document.querySelector('.products-container');
  const categoryElement = document.querySelector('.category');

  categoryButtons.forEach(button => {
    button.addEventListener('click', () => {
      const selectedCategory = data.categories.find(category => category.name === button.innerText);
      categoryElement.querySelector('p').textContent = selectedCategory.name;
      let id = selectedCategory.id;
      productsContainer.innerHTML = '';
      window.location.href = '/home/products/' + id;
    });
  });
});