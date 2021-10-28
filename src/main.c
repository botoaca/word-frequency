#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false = 0, true = 1 } bool;

int pairs_len = 0;
int physical_dimension = 16;

struct pair
{
    char* key; // string
    int value; // appearences
};

bool
insert_pair(struct pair** p_arr, char* key, int val)
{
    /*
        reallocating for each new pair is not efficient
        with it's big O notation being O(n)
        
        // *p_arr = realloc(*p_arr, sizeof(struct pair) * (pairs_len + 1));

        instead, we have a minimum size and double it if
        we need more space. this improves the big O notation
        to O(log in base 2 of n)
    */
    if (pairs_len + 1 > physical_dimension)
    {
        physical_dimension *= 2;
        *p_arr = realloc(*p_arr, sizeof(struct pair) * physical_dimension);
    }

    if (*p_arr == NULL)
        return false;
    (*p_arr)[pairs_len].key = key;
    (*p_arr)[pairs_len].value = val;
    pairs_len++;
    return true;
}

int
main(int argc, char** argv)
{
    /* allocating space for the pair array, starting by a minimum size */
    struct pair* pairs = malloc(sizeof(struct pair) * physical_dimension /* 16 */);

    /* instructions for usage */
    if (argc < 2)
    {
        printf("usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    /*
        reading the input file into the
        content variable
    */
    FILE* in = fopen(argv[1], "r");
    if (in == NULL)
    {
        printf("Error opening file\n");
        return EXIT_FAILURE;
    }
    fseek(in, 0, SEEK_END);
    int f_len = ftell(in);
    fseek(in, 0, SEEK_SET);
    char* content = malloc(f_len + 1);
    fread(content, f_len, 1, in);
    content[f_len] = '\0';
    fclose(in);

    /* removing all dots and commas from the content */
    int i = 0;
    while (content[i] != '\0')
    {
        if (content[i] == '.' || content[i] == ',') 
        {
            memmove(content + i, content + i + 1, strlen(content + i + 1) + 1);
            f_len--;
        }
        else i++;
    }

    /*
        splitting the content into words then inserting them
        into the pairs array if they are not already there
        and incrementing their appearences if they are
    */
    char* token = strtok(content, " ");
    while (token != NULL)
    {
        bool found = false;
        for (int i = 0; i < pairs_len; i++)
        {
            if (!strcmp(token, pairs[i].key))
            {
                found = true;
                pairs[i].value++;
                break;
            }
        }

        if (!found)
        {
            if (!insert_pair(&pairs, token, 1))
            {
                printf("Error inserting word\n");
                return EXIT_FAILURE;
            }
        }

        token = strtok(NULL, " ");
    }

    /* printing the pairs */
    for (int i = 0; i < pairs_len; i++)
    {
        printf("\n%s: %d", pairs[i].key, pairs[i].value);
    }
    printf("\n--------------------\n%d word(s)", pairs_len);

    return EXIT_SUCCESS;
}