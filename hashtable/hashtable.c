/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>//delete

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL) return;
  for (int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }

}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL) return NULL;

  ht_item_t **tmp;
  int hashCode = get_hash(key);

  tmp = &(*table)[hashCode];
  while(*tmp != NULL){
    if (strcmp((*tmp)->key, key) == 0) return *tmp;
    tmp = &(*tmp)->next;
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if(table == NULL || key == NULL) {
    return;
  }
  ht_item_t **tmp = NULL;
  ht_item_t *item = NULL;
  int hashCode = get_hash(key);

  if ((item = ht_search(table, key)) != NULL) {
    item->value = value;
    return;
  } 

  tmp = &(*table)[hashCode];
  if (*tmp != NULL) {
    item = *tmp;
  }
  (*tmp) = malloc(sizeof(struct ht_item));
  (*tmp)->key = key;
  (*tmp)->value = value;
  (*tmp)->next = item;

}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL) return NULL;
  
  ht_item_t *foundItem = ht_search(table, key);
  if (foundItem == NULL){
    return NULL;
  } else {
    return &(foundItem->value);
  }
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (table == NULL || key == NULL) return;

  int hashcode = get_hash(key);
  ht_item_t **tmp = &(*table)[hashcode];
  ht_item_t *delete = NULL;
  
  if (tmp != NULL) {
    delete = *tmp;

   while (*tmp != NULL && strcmp((*tmp)->key, key)) {
      tmp = &(*tmp)->next;
      delete = *tmp;
    
    } 
    *tmp = (*tmp)->next;
    free(delete);
    delete = NULL;
  } else {
    return;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  if (table == NULL) return;
  
  ht_item_t *tmp;
  ht_item_t *next;
  for (int i = 0; i < HT_SIZE; i++){
    tmp = (*table)[i];
    if (tmp != NULL){
      do {
        next = tmp->next;
        free(tmp);
        tmp = next;
      } while (tmp != NULL);
      (*table)[i] = NULL;
      
    }
  }
}
