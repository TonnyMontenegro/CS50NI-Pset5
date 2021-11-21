// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 2000


typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *Htable[HASHTABLE_SIZE];

unsigned int contador = 0;

bool carga = false;

/*
    Hash function via reddit user delipity:
    https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
*/
//funcion hash que retorna la llave
int hash_it(char *needs_hashing)
{
    unsigned int hash = 0;

    for (int i = 0, n = strlen(needs_hashing); i < n; i++)
    {
        hash = (hash << 2) ^ needs_hashing[i];
    }

    return hash % HASHTABLE_SIZE;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    //lee el tamaño de la palabra
    int tam = strlen(word);

    //palabra temporal con un espacio extra para el \0
    char Ptemp[tam + 1];

    //for que se encarga de pasar a minusculas todas las letras de nuestra palabra
    for (int i = 0; i < tam; i++)
    {
        Ptemp[i] = tolower(word[i]);
    }

    Ptemp[tam] = '\0';

    int H = hash_it(Ptemp);

    //la cabeza de nuestro puntero
    node *head = Htable[H];

    //mientras la cabeza no este vacia
    while (head != NULL)
    {
        //comparara la palabra con la palabra temporal y si son identicas retorna true
        if (strcmp(head->word, Ptemp) == 0)
        {
            return true;
        }

        else
        {
            //sino pasa a la siguente
            head = head->next;
        }
    }

    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //limpia la hashtable para usarla
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        Htable[i] = NULL;
    }

    //abre nuestro diccionario
    FILE *inptr = fopen(dictionary, "r");

    //si el diccionario es nulo,esta vacio o no existe imprime un error
    if (inptr == NULL)
    {
        printf("no se puede abrir el dicionario. \n");
        return false;
    }

    //crea un ciclo "infinito"
    while ((2 + 2) == 4)
    {
        node *Nnode = malloc(sizeof(node));

        //si no se pudo reservar el espacio de memoria se imprime un error
        if (Nnode == NULL)
        {
            printf("no se pudo crear el nuevo nodo. \n");
            return false;
        }

        //escanea la palabra dentro del diccionario
        fscanf(inptr, "%s", Nnode->word);

        //limpia el mnodo siguente
        Nnode->next = NULL;

        //si se llega al final del archivo se rompe el ciclo "infinito"
        if (feof(inptr))
        {
            free(Nnode);
            break;
        }

        //aumenta el contador de cantidad de palabras
        contador ++;

        //H es igual a la key de el nuevo nodo apuntando hacia la palabra
        int H = hash_it(Nnode->word);

        //crea nuestra cabecera principal
        node *Mhead = Htable[H];

        if (Mhead == NULL)
        {
            Htable[H] = Nnode;
        }

        else
        {
            Nnode->next = Htable[H];
            Htable[H] = Nnode;
        }
    }

    //cierra el archivo
    fclose(inptr);
    //activa el switch de que la carga termino
    carga = true;
    //retorna true
    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //solo en caso de que la carga sea true se procedera a dar el valor del contador de palabras
    if (carga == true)
    {
        return contador;
    }

    //de lo contrario se retornara 0
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    //desde el indice 0 hasta nuestro tamaño maximo de hashtable
    for (int D = 0; D < HASHTABLE_SIZE; D++)
    {
        //nuestro nodo head es igual a la hashtable en la posicion de nuestra iteracion
        node *head = Htable[D];

        //si la cabeza no es nula procede a limpiar memoria
        while (head != NULL)
        {
            //iguala un nodo temporal a la cabeza
            node *temp = head;

            //la cabeza apunta a la siguente posicion
            head = head->next;
            //libera nuestro nodo temporal y vuelve a hacer lo mismo hasta liberar toda la memoria
            free(temp);

            /* o eso es lo esperado debido a que el check50 capta como
            que siempre queda memoria sin liberar
            y a el staff inlcuso le da ese mismo problema*/
        }
    }
    carga = false;
    return true;
}
