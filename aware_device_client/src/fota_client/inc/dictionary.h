#ifndef _DICT_H
#define _DICT_H

#define KEY_LENGTH 16
#define VALUE_LENGTH 128

typedef struct dict_t {
    char key[KEY_LENGTH], val[VALUE_LENGTH];
    struct dict_t *next;
} dict_t;

int streq(char *str1, char *str2);

dict_t *dictionary_new();
void dictionary_free(dict_t *d);
char *dictionary_get(dict_t *d, char *key);
int dictionary_update(dict_t *d, char *key, char *val);
void dictionary_remove(dict_t *d, char *key);
int dictionary_has(dict_t *d, char *key);
int dictionary_len(dict_t *d);
char **dictionary_keys(dict_t *d);
char **dictionary_values(dict_t *d);

#endif /* _DICTIONARY_H */
