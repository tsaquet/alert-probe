/* 
 * Plug-in Manager
 * @author ECHOES Technologies (FPO)
 * @date 19/02/2012
 */

#include "plugin.h"

int verifExt(char s[])
{
    int i = strlen(s);

    if ((s[i - 5] == '.') && (s[i - 4] == 'j') && (s[i - 3] == 's') && (s[i - 2] == 'o') && (s[i - 1] == 'n'))
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int addBackslash(char *string)
{
    char *backslash = NULL, *rest = NULL, tmp[25500] = "";

    // Search \ character
    backslash = strchr(string, '\\');

    if (backslash != NULL)
    {
        // Value is String after equal, end of line
        rest = backslash + 1;

        // Replace = character by a end of String
        *backslash = '\0';

        strcpy(tmp, string);
        strcat(tmp, "\\\\");

        addBackslash(rest);

        strcat(tmp, rest);
        strcpy(string, tmp);
    }

    return (EXIT_SUCCESS);
}

int listPlugins(const char *plgDir, int *nbPlg, PlgList *plgList)
{
    DIR* dir = NULL;
    struct dirent* read = NULL;

    dir = opendir(plgDir);
    if (dir == NULL)
        return (EXIT_FAILURE);

    // Reading filename file by file
    while ((read = readdir(dir)))
    {
        if (!verifExt(read->d_name))
        {
            char plgPath[255] = "";
            //TODO: define max line by plugin
            json_char json[MAX_SIZE * 1000] = "";
            JSONNODE *n = NULL;

            // Put repository for plugin path
            strcpy(plgPath, plgDir);
            // Add filename for plugin path
            strcat(plgPath, read->d_name);

            printf("Loading plugin %s\n", plgPath);

            if (file2json(plgPath, json))
            {
                perror("file2json()");
                return (errno);
            }

            // Remove spaces and comments
            strcpy(json, json_strip_white_space(json));

            n = json_parse(json);

            if (json2llist(n, plgList))
            {
                perror("json2llist()");
                return (errno);
            }
            
            //TODO: llist to delete all n after addons
            //json_delete(n);

            // Count number of plugin
            (*nbPlg)++;
        }
    }

    if (closedir(dir))
    {
        perror("closedir()");
        return (errno);
    }
    return EXIT_SUCCESS;
}

int file2json(const char *plgPath, json_char* json)
{
    FILE* plgFile = NULL;
    char line[MAX_SIZE] = "";

    // Opening file
    plgFile = fopen(plgPath, "r");

    if (plgFile != NULL)
    {

        // Reading file line by lfileine
        while (fgets(line, MAX_SIZE, plgFile) != NULL)
        {
            addBackslash(line);
            strcat(json, line);
        }

        // Closing plugin file
        fclose(plgFile);

        if (!json_is_valid(json))
        {
            printf("Invalid JSON Node : %s\n", plgPath);
            return (EXIT_FAILURE);
        }
    }
    else
    {
        perror("fopen()");
        return (errno);
    }

    return (EXIT_SUCCESS);
}

int json2llist(JSONNODE *n, PlgList *plgList)
{
    if (n == NULL)
    {
        printf("Invalid JSON Node\n");
        return (EXIT_FAILURE);
    }

    JSONNODE_ITERATOR i = json_begin(n);

    // Get the node name and value as a string
    json_char *node_name = json_name(*i);

    PlgInfo* plgInfo = calloc(1, sizeof (PlgInfo));
    JSONNODE *sources, *searches;;

    while (i != json_end(n))
    {
        if (*i == NULL)
        {
            printf("Invalid JSON Node\n");
            return (EXIT_FAILURE);
        }

        // Get the node name and value as a string
        node_name = json_name(*i);

        // Find out where to store the values
        if (!strcmp(node_name, "id"))
        {
            plgInfo->idPlg = json_as_int(*i);
        }
        else if (!strcmp(node_name, "idAsset"))
        {
            plgInfo->idAsset = json_as_int(*i);
        }
        else if (!strcmp(node_name, "sources"))
        {
            sources = *i;
        }
        // Increment the iterator
        ++i;
    }

    if (sources == NULL)
    {
        printf("Invalid Plugin: no source\n");
        return (EXIT_FAILURE);
    }
    
    SrcList srcList = NULL;
    i = json_begin(sources);

    while (i != json_end(sources))
    {
        if (*i == NULL)
        {
            printf("Invalid JSON Node\n");
            return (EXIT_FAILURE);
        }

        SrcInfo* srcInfo = calloc(1, sizeof (SrcInfo));
        JSONNODE_ITERATOR j = json_begin(*i);

        while (j != json_end(*i))
        {
            if (*j == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }

            // Get the node name and value as a string
            node_name = json_name(*j);

            if (*j == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }
            if (!strcmp(node_name, "id"))
            {
                srcInfo->idSrc = json_as_int(*j);
            }
            else if (!strcmp(node_name, "idAddon"))
            {
                srcInfo->idAddon = json_as_int(*j);
            }
            else if (!strcmp(node_name, "params"))
            {
                srcInfo->params = *j;
            }
            else if (!strcmp(node_name, "searches"))
            {
                searches = *j;
            }
            // Increment the iterator
            ++j;
        }
/*
                char path[255];
                 j = json_begin(srcInfo->params);
                while (j != json_end(srcInfo->params))
                {
                    if (*j == NULL)
                    {
                        printf("Invalid JSON Node\n");
                        return (EXIT_FAILURE);
                    }
                    // Get the node name and value as a string
                    node_name = json_name(*j);
                    if (!strcmp(node_name, "path"))
                    {
                        json_char *node_value = json_as_string(*j);
                        strcpy(path, node_value);
                        json_free(node_value);
                    }
                    // Cleanup and increment the iterator
                    ++j;
                }
                printf("path: %s\n", path);
*/
        
        if (searches == NULL)
        {
            printf("Invalid Plugin: no searches\n");
            return (EXIT_FAILURE);
        }

        SearchList searchList = NULL;

        j = json_begin(searches);
        while (j != json_end(searches))
        {
            if (*j == NULL)
            {
                printf("Invalid JSON Node\n");
                return (EXIT_FAILURE);
            }
            // New element of linjed list
            SearchInfo* searchInfo = calloc(1, sizeof (SearchInfo));
            searchInfo->nxt = NULL;
            JSONNODE_ITERATOR k = json_begin(*j);

            while (k != json_end(*j))
            {
                if (*k == NULL)
                {
                    printf("Invalid JSON Node\n");
                    return (EXIT_FAILURE);
                }
                // Get the node name and value as a string
                node_name = json_name(*k);
                if (!strcmp(node_name, "id"))
                {
                    searchInfo->idSearch = json_as_int(*k);
                }
                else if (!strcmp(node_name, "idType"))
                {
                    searchInfo->idType = json_as_int(*k);
                }
                else if (!strcmp(node_name, "params"))
                {
                    searchInfo->params = *k;
                }
                else if (!strcmp(node_name, "period"))
                {
                    json_char *node_value = json_as_string(*k);
                    strcpy(searchInfo->period, node_value);
                    //TODO: transform period to int (second)
                    json_free(node_value);
                }
                else if (!strcmp(node_name, "staticValues"))
                {
                    searchInfo->staticValues = json_as_int(*k);
                }
                // Increment the iterator
                ++k;
            }
            // Assign the address of the next element in the new element
            searchInfo->nxt = searchList;

            // Update the pointer of linked list
            searchList = searchInfo;

            // Increment the iterator
            ++j;
        }

        srcInfo->searchList = searchList;
 

        // Assign the address of the next element in the new element
        srcInfo->nxt = srcList;

        // Update the pointer of linked list
        srcList = srcInfo;

        // Increment the iterator
        ++i;
    }

    plgInfo->srcList = srcList;
    
    // Assign the address of the next element in the new element
    plgInfo->nxt = *plgList;

    // Update the pointer of linked list
    *plgList = plgInfo;

    // Cleanup
    json_free(node_name);

    return (EXIT_SUCCESS);
}

int plugin(const char *plgDir, PlgList *plgList)
{
    // Plugins counter initialisation
    int nbPlg = 0;

    if (listPlugins(plgDir, &nbPlg, plgList))
    {
        perror("listPlugins()");
        return (errno);
    }
    printf("%d plugins load.\n", nbPlg);

    return (EXIT_SUCCESS);
}

