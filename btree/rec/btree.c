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
  *tree = malloc(sizeof(bst_node_t));
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
  if (tree != NULL) {
    if (tree->key == key) {
      *value = tree->value;
      return true;
    } 
    
    if (tree->key > key) {
      return bst_search(tree->left, key, value);
    } else {
      return bst_search(tree->right, key, value);
    }

  } else {
    return false;
  }
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
void bst_insert(bst_node_t **tree, char key, int value) { //check if i pointers are right
  if ((*tree)->key){ //check if condition works  
    if ((*tree)->key == key) {
      (*tree)->value = value;
    } else if ((*tree)->key > key) {
      if ((*tree)->left == NULL){
          bst_node_t *new_node; //check if i pointers are right and malloc works
          bst_init(&new_node);
          (*tree)->left = new_node;
      }
      bst_insert(&(*tree)->left, key, value);
    } else {
      if ((*tree)->right == NULL){
          bst_node_t *new_node; //check if i pointers are right and malloc works
          bst_init(&new_node);
          (*tree)->right = new_node;
      }
      bst_insert(&(*tree)->right, key, value);
    }
  } else {
    if (*tree == NULL){ //maybe err
      bst_node_t *new_node;
      bst_init(&new_node);
      *tree = new_node;
    }
    (*tree)->key = key;
    (*tree)->value = value;
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
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) { //check if null can come
  bst_node_t *right = (*tree)->right;
  bst_node_t *current = (*tree);
  if (right != NULL) {
    if (right->right != NULL) {
      bst_replace_by_rightmost(target, &right);
    } else {
      target->key = right->key;
      target->value = right->value;
      current->right = right->left;
      free(right);
      return;
    }
  }
  target->key = current->key;
  target->value = current->value; //maybe free(current)
  return;
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
  if ((*tree)->key == key){
    bst_replace_by_rightmost((*tree), &(*tree)->left);
    return;
  }
  if ((*tree)->key > key) {
    if ((*tree)->left != NULL) {
      bst_delete(&(*tree)->left, key);
    } else {
      return;
    }
  } else {
    if ((*tree)->right != NULL) {
      bst_delete(&(*tree)->right, key);
    } else {
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
  if ((*tree)->left == NULL && (*tree)->right == NULL) {
    free(*tree);
    return;
  }
  if ((*tree)->right != NULL) {
    bst_replace_by_rightmost((*tree), &(*tree)->right);
    bst_dispose(&(*tree));
  } 
  if ((*tree)->left != NULL) {
    bst_node_t *tmp = (*tree)->left;
    bst_dispose(tmp);
    free(tmp);
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
    bst_print_node(tree); //maybe delete
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
    bst_print_node(tree); //maybe delete
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
    bst_print_node(tree); //maybe delete
  }
  return;
}
