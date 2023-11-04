/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  (*tree) = malloc(sizeof(struct bst_node));
  (*tree)->key ='\0';
  (*tree)->value = 0; //check if 0 is ok (maybe NULL
  (*tree)->left = NULL;
  (*tree)->right = NULL;

  return;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
 if (tree->key == key) {
    value = &(tree)->value;
    return true;
  } else if (tree->key > key) {
    if (tree->left != NULL) {
      bst_search(tree->left, key, value);
    } else {
      return false;
    }
  } else if (tree->key < key) {
    if (tree->right != NULL) {
      bst_search(tree->right, key, value);
    } else {

      return false;
    }
  } else {
    return false;
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree)->key == '\0') {
    (*tree)->key = key;
    (*tree)->value = value;
    return;
  }
  if ((*tree)->key == key) {
    (*tree)->value = value;
    return;
  } else {
    if ((*tree)->key > key) {
      if ((*tree)->left != NULL) {
        bst_insert(&(*tree)->left, key, value);
      } else {
        bst_node_t *newNode;
        bst_init(&newNode);
        newNode->key = key;
        newNode->value = value;
        (*tree)->left = newNode;
        return;
      }
    } else if ((*tree)->key < key) {

      if ((*tree)->right != NULL) {
        bst_insert(&(*tree)->right, key, value);
      } else {
        bst_node_t *newNode;
        bst_init(&newNode);
        newNode->key = key;
        newNode->value = value;
        (*tree)->right = newNode;
        return;
      }
    }
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree) == NULL ) return;
  if ((*tree)->right == NULL ){
    return;
  }

  bst_node_t *next = (*tree)->right;

  if (next->right != NULL) {
    bst_replace_by_rightmost(target, &next);
    return;
  } else {
    if (next->left != NULL){
      (*tree)->right = next->left;
    }
    target->key = next->key;
    target->value = next->value;
    free(next);
    (*tree)->right = NULL;
    return;
  }
}


/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t oldOne = **tree;

  if ((*tree)->left != NULL){
    if ((*tree)->left->key == key){

      if ((*tree)->left->left == NULL && (*tree)->left->right == NULL){
        free((*tree)->left);
        (*tree)->left = NULL;
        return;
      }
      if ((*tree)->left->left != NULL && (*tree)->left->right == NULL){
        printf("*tree.left.key: %c\n", (*tree)->left->key);
        free((*tree)->left);
        printf("*tree.left.key: %c\n", (*tree)->left->key);
        (*tree)->left = oldOne.left->left;
        printf("*tree.left.key: %c\n", (*tree)->left->key);
        return;
      }
      if ((*tree)->left->left == NULL && (*tree)->left->right != NULL){
        free((*tree)->left);
        (*tree)->left = oldOne.left->right;
        return;
      }
    } 
  } 

  if ((*tree)->right != NULL){
    if ((*tree)->right->key == key){

      if ((*tree)->right->left == NULL && (*tree)->right->right == NULL){
        free((*tree)->right);
        (*tree)->right = NULL;
        return;
      }
      if ((*tree)->right->left != NULL && (*tree)->right->right == NULL){
        bst_replace_by_rightmost((*tree)->right, &(*tree)->right->left);
        free((*tree)->right);
        (*tree)->right->left = oldOne.right->left; 
        return;
      }
      if ((*tree)->right->left == NULL && (*tree)->right->right != NULL){
        free((*tree)->right);
        (*tree)->right = oldOne.right->right;
        return;
      }
      if ((*tree)->right->left != NULL && (*tree)->right->right != NULL){
        bst_replace_by_rightmost((*tree)->right, &(*tree)->right->left);
        (*tree)->right->left = oldOne.right->left;
        (*tree)->right->right = oldOne.right->right;
        
        return;
      }
    } 
  }

  if ( (*tree)->key == key){
    if ((*tree)->left != NULL && (*tree)->right != NULL){
      bst_replace_by_rightmost((*tree), &(*tree)->left);
      (*tree)->right = oldOne.right;
      (*tree)->left = oldOne.left;
      return;
    } else if ((*tree)->left != NULL && (*tree)->right == NULL){
        bst_replace_by_rightmost((*tree), &(*tree)->left);
        return;
    } else if ((*tree)->left == NULL && (*tree)->right != NULL){
      (*tree) = oldOne.right;
      return;
    }
  } else {
    if ((*tree)->key > key) {
      if ((*tree)->left != NULL) {
        bst_delete(&(*tree)->left, key);
      }
      return;
    } else if ((*tree)->key < key) {
      if ((*tree)->right != NULL) {
        bst_delete(&(*tree)->right, key);
      }
      return;
    } 
  }

}


/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if ((*tree) == NULL) return;
  if ((*tree)->left != NULL && (*tree)->right != NULL){
      bst_dispose(&(*tree)->right);
      bst_dispose(&(*tree)->left);
      
    }
  
  
  if ((*tree)->left != NULL && (*tree)->right == NULL){
    bst_dispose(&(*tree)->left);
    (*tree)->left = NULL;
  }
  if ((*tree)->left == NULL && (*tree)->right != NULL){
    bst_dispose(&(*tree)->right);
    (*tree)->right = NULL;
  }
  if ((*tree)->left == NULL && (*tree)->right == NULL){
    free((*tree));
    (*tree) = NULL;
    return;
  }
  
  return;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL) {
    bst_add_node_to_items(tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
  return;
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
  }
  return;
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL){
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
  return;
}
