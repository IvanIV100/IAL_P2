/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t **tmp = &tree;
  stack_bst_t stack;
  stack_bst_init(&stack);
  bool run = true;

  while (run){
    if ((*tmp) == NULL){
      run = false;
    } else {
      if ((*tmp)->key > key){
        tmp = &(*tmp)->right;
      } else if ((*tmp)->key < key){
        tmp = &(*tmp)->left;
      } else {
        *value = (*tmp)->value;
        run = false;
        return true;
      }
    }
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  bool run = true;
  bst_node_t **tmp = &(*tree);

  if ((*tmp)->key == '\0'){
    (*tmp)->key = key;
    (*tmp)->value = value;
    run = false;
  }

  while(run){
    if ((*tmp) == NULL){
      bst_init(&(*tmp));
      (*tmp)->key = key;
      (*tmp)->value = value;
      run = false;
    } else {
      if ((*tmp)->key < key){
        tmp = &(*tmp)->right;
      } else if ((*tmp)->key > key){
        tmp = &(*tmp)->left;
      } else {
        (*tmp)->value = value;
        run = false;
      }
    }
  }
  return;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t **curent = &(*tree);
  bst_node_t **parent = NULL;
  bool run = true;
  
  
  while (run){   
    if ((*curent)->right == NULL){
      if ((*curent)->left != NULL && (*parent) != NULL){
        (*parent)->right = (*curent)->left;
      }
      target->key = (*curent)->key;
      target->value = (*curent)->value;
      free((*curent));
      (*curent) = NULL;
      run = false;
    } else {
      parent = &(*curent);
      curent = &(*curent)->right;
    }
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *parent =NULL;
  bst_node_t *current = *tree;
  //bool run = bst_search(*tree, key, NULL);
  bool run = true;

  if (current == NULL) {
    run = false;
    return;
  }
  
  while(run){

    if (current->key == key){
      if (current->left != NULL && current->right != NULL){
        if (parent != NULL){
          bst_replace_by_rightmost((current), &(current)->left);
          current->right = (parent)->right->right;
          (current)->left = (parent)->right->left;
          (parent)->right = (current);
          run = false;
          return;
        } else {
          bst_node_t *tmp = current;
          bst_replace_by_rightmost((current), &(current)->left);
          current->right = tmp->right;
          current->left = tmp->left;
          tmp = NULL;
        }
        

      } 

      if ((current)->left == NULL && (current)->right == NULL){
        if ((parent) != NULL){
          if ((parent)->left->key == (current)->key){
            (parent)->left = NULL;
          } else {
            (parent)->right = NULL;
          }
        }
        free(current);
        (current) = NULL;
        run = false;
        return;
      }
      if ((current)->left == NULL && (current)->right != NULL) {
        if ((parent) != NULL){
          if ((parent)->left == (current)){
            (parent)->left = (current)->right;
          } else {
            (parent)->right = (current)->right;

          }
        } 
        free(current);
        (current) = NULL;
        run = false;
        return;
      }
      if ((current)->left != NULL && (current)->right == NULL) {
        if ((parent) != NULL){
          if ((parent)->left->key == (current)->key){
            (parent)->left = (current)->left;
          } else {
            (parent)->right = (current)->left;

          }
        } 
        free(current);
        (current) = NULL;
        run = false;
        return;
      }
    } else {
      if ((current)->key < key){
        parent = (current);
        current = (current)->right;
      } else if ((current)->key > key){
        parent = (current);
        current = (current)->left;
      } 
      if ( current == NULL){
        run = false;
        return;
      }
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
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);

  do {
    if ((*tree) == NULL){
      if(!stack_bst_empty(&stack)){
        (*tree) = stack_bst_top(&stack);
        stack_bst_pop(&stack);
      } 
    } else {
      if ((*tree)->right != NULL){
        stack_bst_push(&stack, (*tree)->right);
      }
      bst_node_t *tmp = (*tree);
      (*tree) = (*tree)->left;
      free(tmp);
      tmp = NULL;
    }
  } while ((*tree) != NULL || !stack_bst_empty(&stack));
  
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL){
    bst_add_node_to_items(tree, items);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  bst_node_t *current = tree;
  stack_bst_init(&stack);
  bool run = true;
  if (tree == NULL) return;

  bst_leftmost_preorder(tree, &stack, items);
  current = stack_bst_pop(&stack);
  while(run){
    if (current->right != NULL){
      bst_leftmost_preorder(current->right, &stack, items);
      current = stack_bst_pop(&stack);
    } else {
      if (stack_bst_empty(&stack)){
        run = false;
      } else {
        current = stack_bst_pop(&stack);
      }
    }
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }


}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  bst_node_t *current = tree;
  stack_bst_init(&stack);
  bool run = true;
  if (tree == NULL) return;
  bst_leftmost_inorder(tree, &stack);
  current = stack_bst_pop(&stack);
  bst_add_node_to_items(current, items);
  
  while(run){
    if (current->right != NULL){
      bst_leftmost_inorder(current->right, &stack);
      current = stack_bst_pop(&stack);
      bst_add_node_to_items(current, items);
    } else {
      if (stack_bst_empty(&stack)){ 
        run = false;
        return;
      }
      current = stack_bst_pop(&stack);
      bst_add_node_to_items(current, items);
    }
  }

}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {


}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
}
