#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int id;
    char name[51];
    float price;
    int quantity;
} Product;

typedef enum
{
    ADD_PRODUCT = 1,
    VIEW_ALL_PRODUCTS,
    UPDATE_QUANTITY,
    SEARCH_PRODUCT_BY_ID,
    SEARCH_PRODUCT_BY_NAME,
    SEARCH_PRODUCT_BY_PRICE,
    DELETE_PRODUCT,
    EXIT
} MenuOptions;

void displayMenu();
void displayAllProducts(const Product *inventory, int count);
void makeChoice(int choice, Product **inventory, int *count, int *capacity);

void inputProductDetails(Product *Product);
void addProduct(Product **inventory, int *count, int *capacity);
void updateQuantity(Product *inventory, int count);
void searchProductByID(const Product *inventory, int count);
void searchProductByName(const Product *inventory, int count);
void searchByPriceRange(const Product *inventory, int count);
void deleteProduct(Product **inventory, int *count);

void displayMenu()
{
    printf("\n==========INVENTORY MENU===========\n");
    printf("1. Add new Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product By ID\n");
    printf("5. Search Product By Name\n");
    printf("6. Search Product By Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

void makeChoice(int choice, Product **inventory, int *count, int *capacity)
{
    switch (choice)
    {
    case ADD_PRODUCT:
        printf("\nEnter new product details: \n");
        addProduct(inventory, count, capacity);
        break;

    case VIEW_ALL_PRODUCTS:
        displayAllProducts(*inventory, *count);
        break;

    case UPDATE_QUANTITY:
        updateQuantity(*inventory, *count);
        break;

    case SEARCH_PRODUCT_BY_ID:
        searchProductByID(*inventory, *count);
        break;

    case SEARCH_PRODUCT_BY_NAME:
        searchProductByName(*inventory, *count);
        break;

    case SEARCH_PRODUCT_BY_PRICE:
        searchByPriceRange(*inventory, *count);
        break;

    case DELETE_PRODUCT:
        deleteProduct(inventory, count);
        break;

    case EXIT:
        printf("Memory relessed succesfully. Exiting program...\n");
        break;

    default:
        break;
    }
}

void displayAllProducts(const Product *inventory, int count)
{
    if (count <= 0)
    {
        printf("Inventory is empty.\n");
        return;
    }
    printf("\n=====PRODUCT LIST======\n");
    for (int index = 0; index < count; index++)
    {
        printf("Product Id: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[index].id,
               inventory[index].name,
               inventory[index].price,
               inventory[index].quantity);
    }
}

void inputProductDetails(Product *Product)
{
    printf("Product Id: ");
    scanf("%d", &Product->id);

    printf("Product Name: ");
    scanf(" %[^\n]", Product->name);

    printf("Product Price: ");
    scanf("%f", &Product->price);

    printf("Product Quantity: ");
    scanf("%d", &Product->quantity);
}

void addProduct(Product **inventory, int *count, int *capacity)
{
    int newCapacity = (*capacity) + 1;

    Product *temp = (Product *)realloc(*inventory, newCapacity * sizeof(Product));

    if (temp == NULL)
    {
        printf("Memory reallocation failed! CAnnot add product\n");
        return;
    }

    *inventory = temp;
    *capacity = newCapacity;

    printf("\nEnter new product details: \n");
    inputProductDetails(&(*inventory)[*count]);

    (*count)++;

    printf("Product added successfully!\n");
}

void updateQuantity(Product *inventory, int count)
{
    int IDToUpdate;
    int foundID = 0;

    printf("Enter product ID to update quantity: ");
    scanf("%d", &IDToUpdate);

    for (int index = 0; index < count; index++)
    {
        if (inventory[index].id == IDToUpdate)
        {
            int newQuantity;

            printf("Current quantity: %d\n", inventory[index].quantity);
            printf("Enter new quantity: ");
            scanf("%d", &newQuantity);

            if (newQuantity < 0)
            {
                printf("Eroor: quantity cant be negative");
                return;
            }

            inventory[index].quantity = newQuantity;

            printf("Quntity updated successfully!\n");
            foundID = 1;
            break;
        }
    }
    if (!foundID)
    {
        printf("Product with ID %d not found", IDToUpdate);
    }
}

void searchProductByID(const Product *inventory, int count)
{
    int targetID;
    int foundProduct = 0;

    printf("Enter Product ID to search: ");
    scanf("%d", &targetID);

    for (int index = 0; index < count; index++)
    {
        if (inventory[index].id == targetID)
        {
            printf("Product Found: ");
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[index].id,
                   inventory[index].name,
                   inventory[index].price,
                   inventory[index].quantity);
            foundProduct = 1;
            break;
        }
    }
    if (!foundProduct)
    {
        printf("Product with ID %d not found \n", targetID);
    }
}

void searchProductByName(const Product *inventory, int count)
{
    char nameToSearch[51];
    int foundName = 0;

    printf("Enter name to search (partially allowed): ");
    scanf(" %[^\n]", nameToSearch);

    printf("Product Found: \n");

    for (int index = 0; index < count; index++)
    {
        if (strstr(inventory[index].name, nameToSearch) != NULL)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[index].id,
                   inventory[index].name,
                   inventory[index].price,
                   inventory[index].quantity);
            foundName++;
        }
    }
    if (foundName == 0)
    {
        printf("No products found with name '%s'\n", nameToSearch);
    }
    else
    {
        printf("Total %d products found.\n", foundName);
    }
}

void searchByPriceRange(const Product *inventory, int count)
{
    float minimumPrice, maximumPrice;
    int foundPriceRange = 0;

    printf("Enter minimum price: ");
    scanf("%f", &minimumPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maximumPrice);

    if (minimumPrice > maximumPrice)
    {
        printf("Error: Minimum price is grater than maximum price!\n");
        return;
    }

    printf("Products in price range: \n");

    for (int index = 0; index < count; index++)
    {
        if (inventory[index].price >= minimumPrice && inventory[index].price <= maximumPrice)
        {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[index].id,
                   inventory[index].name,
                   inventory[index].price,
                   inventory[index].quantity);
            foundPriceRange++;
        }
    }

    if (foundPriceRange == 0)
    {
        printf("No products found in price range %.2f to %.2f", minimumPrice, maximumPrice);
    }
    else
    {
        printf("Total %d products found in price range\n", foundPriceRange);
    }
}

void deleteProduct(Product **inventory, int *count)
{
    int IDToDelete;
    int foundID = 0;
    int deleteIndex = -1;
    int shouldContinue = 1;

    if (*count == 0)
    {
        printf("Inventory is empty!");
        shouldContinue = 0;
    }
    if (shouldContinue)
    {
        printf("Enter Product ID to delete: ");
        scanf("%d", &IDToDelete);

        for (int index = 0; index < *count; index++)
        {
            if ((*inventory)[index].id == IDToDelete)
            {
                foundID = 1;
                deleteIndex = index;
                break;
            }
        }

        if (!foundID)
        {
            printf("Product with ID %d not found \n", IDToDelete);
            shouldContinue = 0;
        }
    }

    if (shouldContinue)
    {
        for (int index = deleteIndex; index < *count - 1; index++)
        {
            (*inventory)[index] = (*inventory)[index + 1];
        }
        (*count)--;

        if (*count > 0)
        {
            Product *temp = (Product *)realloc(*inventory, (*count) * sizeof(Product));
            if (temp != NULL)
            {
                *inventory = temp;
            }
        }
        printf("Product deleted successfully! \n");
    }
}

int main()
{
    Product *inventory = NULL;
    int count = 0;
    int capacity = 0;
    int choice;
    int initial;
    bool programShouldContinue = true;

    printf("Enter initial number of products: ");
    scanf("%d", &initial);

    if (initial < 1 || initial > 100)
    {
        printf("Error: Initial must be betweeen 1 and 100\n");
        return 1;
    }

    if (programShouldContinue)
    {
        inventory = (Product *)calloc(initial, sizeof(Product));

        if (inventory == NULL)
        {
            printf("Memory allocation failed!\n");
            programShouldContinue = false;
        }
    }

    if (programShouldContinue)
    {
        capacity = initial;
        count = initial;

        for (int index = 0; index < count; index++)
        {
            printf("\nEnter details for products %d: \n", index + 1);
            inputProductDetails(&inventory[index]);
        }

        do
        {
            displayMenu();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            makeChoice(choice, &inventory, &count, &capacity);
        } while (choice != EXIT);

        free(inventory);
    }

    if (programShouldContinue)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
