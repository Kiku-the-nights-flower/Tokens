//
// Created by Kiku on 12/5/2022.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctime>

typedef struct arm {
    char name;
    int tokens[32] = {0};
    int tokenAmount;
    int pointer;

    arm() {
        name = '\0';
        tokenAmount = 0;
        pointer = 0;
    }

    bool addToken(int val) {
        if (tokenAmount == 32) {
            return false;
        }
        tokens[tokenAmount] = val;
        tokenAmount++;
        return true;
    }

    int *takeToken() {
        if (pointer <= tokenAmount) {
            return &tokens[pointer++];
        } else
            return nullptr;
    }

    int takeTokenAt(int index) const {
        return tokens[index];
    }
} Arm;

typedef struct cross {
    arm arms[4];

    cross() {
        for (auto &i: arms) {
            i = arm();
        }
    }
} Cross;


typedef struct treeNode {
    treeNode *children;
    int childrenAmount;
    int childrenSize;
    int value;
    int valForPlayer;
    char armName;
    int armIndex;
    bool path;
    treeNode *parent;

    explicit treeNode(const int val) {
        value = val;
        children = (treeNode *) (malloc(4 * sizeof(treeNode)));
        childrenAmount = 0;
        childrenSize = 4;
        parent = nullptr;
        valForPlayer = 0;
        armName = '\0';
        armIndex = -1;
        path = false;
    }

    void setvalue(int val, char name, int index) {
        value = val;
        armName = name;
        armIndex = index;
    }

    void addNode(treeNode node) {
        node.parent = this;
        children[childrenAmount] = node;
        childrenAmount = childrenAmount + 1;
    }

    void destroy() const {
        for (int i = 0; i < childrenAmount; ++i) {
            children[i].destroy();
        }
        free(children);
    }



    /*void print(int indent) const {
        for (int i = 0; i < indent; ++i) {
            std::cout << " ";
        }
        std::cout << value << std::endl;
        for (int i = 0; i < childrenAmount; ++i) {
            children[i].print(indent + 1);
        }
    }
    */



} TreeNode;


bool isNumber(char *in) {
    char *tmp = (char *) malloc(INT32_MAX * sizeof(char));
    sprintf(tmp, "%d", atoi(in));
    bool ret = strlen(in) == strlen(tmp);
    free(tmp);
    return ret;
}


char *getBetween(char delim, char ndelim, char *inp) {
    char *p1, *p2;
    char *res;
    p1 = strchr(inp, delim);
    if (p1 && (strlen(inp) - 1 != 0)) {
        p2 = strchr(inp + 1, ndelim);
        if (p2) {
            size_t resLength = strlen(p1 + 1) - strlen(p2);
            res = (char *) malloc(resLength + 1); // include the null char at end
            strncpy(res, p1 + 1, resLength);
            res[resLength] = '\0'; //terminating null char at end
            return res;
        }
        return nullptr;
    }
    return nullptr;
}

//reallocs a string, then nulls the new
char *reallocArr(char *arr, size_t prevSize, size_t size) {
    arr = (char *) realloc(arr, size);
    memset(arr + prevSize, 0, size - prevSize);
    return arr;
}

//allocates an empty string
char *allocString(size_t size) {
    char *s = (char *) malloc(size);
    memset(s, 0, size);
    return s;
}


//converts 1,2,-3 into array {1,2,-3} and saves it into arm (target)
bool convertIntoTokens(char *input, arm *target) {
    const char delim = ',';
    int pointer = 0;
    char *find;
    while ((find = strchr(input + pointer +1, delim)) != nullptr) {
        size_t length = strlen(input + pointer + 1) - strlen(find);
        printf("%s\n", find);
        pointer += length + 1;
    }

    return true;

}


bool findSides(char *input, cross *crs) {
    char *formatted = getBetween(' ', ' ', input);
    size_t pointer = 0;
    int counter = 0;
    while (formatted != nullptr) {
        char id;
        sscanf(formatted, "%c : { %*s }", &id);
        char *res = getBetween('{', '}', formatted);
        (*crs).arms[counter].name = id;
        if (!convertIntoTokens(res, &((*crs).arms[counter]))) {
            free(res);
            return false;
        }
        counter++;
        pointer += strlen(formatted) + 1;
        free(res);
        free(formatted);
        if (pointer >= strlen(input)) {
            break;
        }
        formatted = getBetween(' ', ' ', input + pointer);
    }

    return true;
}

//TODO: get rid of strtok as it is unstable in testing
// ex. takes only - of negative number, forgets that there is a number at all making the program ever so slightly unusable
/*
bool findSides(char *input, cross *crs) {
    const char delim[2] = "\n";
    char * tmp = (char *) malloc(sizeof(char) * 700);
    int counter = 0;
    while (token != nullptr) {
        char id;
        sscanf(token, "%c : { %s }", &id);
        char *res = getBetween('{', '}', token);
        (*crs).arms[counter].name = id;
        if (!convertIntoTokens(res, &((*crs).arms[counter]))) {
            free(res);
            return false;
        }
        counter++;
        token = strtok_r(nullptr, delim, &save1);
        free(res);
    }
    return true;
}
*/



// standardizes input, splits it by arms so the findSides function can take it
// also realizes if the input is faulty
char *readInput() {
    char *tmp = allocString(20);
    size_t inputSize = 20 * sizeof(char);
    size_t innerLength = 0;
    char c;
    while ((c = fgetc(stdin)) != EOF) {
        if (c != '\n') {
            if (innerLength + 2 >= inputSize) {
                tmp = reallocArr(tmp, inputSize, inputSize * 2);
                inputSize *= 2;
            }
            if (c == 'N' || c == 'E' || c == 'W' || c == 'S') {
                tmp[innerLength] = ' ';
                innerLength++;
            }
            if (c == ' ' &&
                (tmp[innerLength - 1] == ' ' || tmp[innerLength - 1] == ',' || tmp[innerLength - 1] == '{')) {
                continue;
            }

            if (c == '}' && tmp[innerLength - 1] == ' ') {
                tmp[innerLength - 1] = c;
                continue;
            }

            if (c == ',' && tmp[innerLength - 1] == ',') {
                free(tmp);
                return nullptr;
            }

            if (c == ',' && tmp[innerLength - 1] == ' ') {
                tmp[innerLength - 1] = c;
                continue;
            }

            if (c == ' ' && tmp[innerLength - 1] == ':') {
                continue;
            }

            tmp[innerLength] = c;
            innerLength++;
        }
    }
    tmp[innerLength] = ' ';
    char *input = allocString(innerLength + 1);
    memcpy(input, tmp, innerLength + 1);
    free(tmp);
    input[innerLength + 1] = '\0';
    return input;
}

//takes approx 0.068s
void evalTree(treeNode *root) {
    if (root->childrenAmount == 0) {
        root->valForPlayer = root->value;
        return;
    }

    int highestChildVal = INT32_MIN;

    for (int i = 0; i < root->childrenAmount; ++i) {
        evalTree(&root->children[i]);
        if (root->children[i].valForPlayer > highestChildVal) {
            highestChildVal = root->children[i].valForPlayer;
        }
    }
    root->valForPlayer = root->value - highestChildVal;
}


treeNode *generateUniverse(cross c, treeNode *root, int depth, int nDepth, int eDepth, int wDepth, int sDepth) {
    if (depth != 0) {
        for (int i = 0; i < 4; ++i) {
            arm a = c.arms[i];
            switch (a.name) {
                case 'N' :
                    if (nDepth < a.tokenAmount) {
                        int val = a.takeTokenAt(nDepth);
                        treeNode node = treeNode(val);
                        node.setvalue(val, a.name, nDepth);
                        generateUniverse(c, &node, depth - 1, nDepth + 1, eDepth, wDepth, sDepth);
                        root->addNode(node);
                    }
                    break;
                case 'E':
                    if (eDepth < a.tokenAmount) {
                        int val = a.takeTokenAt(eDepth);
                        treeNode node = treeNode(val);
                        node.setvalue(val, a.name, eDepth);
                        generateUniverse(c, &node, depth - 1, nDepth, eDepth + 1, wDepth, sDepth);
                        root->addNode(node);
                    }
                    break;

                case 'W' :
                    if (wDepth < a.tokenAmount) {
                        int val = a.takeTokenAt(wDepth);
                        treeNode node = treeNode(val);
                        node.setvalue(val, a.name, wDepth);
                        generateUniverse(c, &node, depth - 1, nDepth, eDepth, wDepth + 1, sDepth);
                        root->addNode(node);
                    }
                    break;
                case 'S' :
                    if (sDepth < a.tokenAmount) {
                        int val = a.takeTokenAt(sDepth);
                        treeNode node = treeNode(val);
                        node.setvalue(val, a.name, sDepth);
                        generateUniverse(c, &node, depth - 1, nDepth, eDepth, wDepth, sDepth + 1);
                        root->addNode(node);
                    }
                    break;
            }

        }
    }
    return root->children;
}


void walk(TreeNode *root, int *Asum, int *Bsum, bool player) {
    TreeNode tmp = root->children[0];
    int VALUE = INT32_MIN;
    for (int i = 0; i < root->childrenAmount; ++i) {
        if (root->children[i].valForPlayer > VALUE) {
            tmp = root->children[i];
            VALUE = tmp.valForPlayer;
        }
    }

    if (player) {
        printf("A: %c[%d] (%d)\n", tmp.armName, tmp.armIndex, tmp.value);
        *Asum = *Asum + tmp.value;
    } else {
        printf("B: %c[%d] (%d)\n", tmp.armName, tmp.armIndex, tmp.value);
        *Bsum = *Bsum + tmp.value;
    }
    if (tmp.childrenAmount > 0) {
        walk(&tmp, Asum, Bsum, !player);
    }

}


//TODO: reduce the depth of the game tree by making only specific depth, evaluate it, then pick the right move,
// continue with the picked node to generate/eval the tree
// should drastically reduce the time needed for computing

int tokens() {
    cross c = cross();
    char *input = readInput();
    if (input == nullptr) {
        free(input);
        printf("Nespravny vstup.\n");
        return 1;
    }

    /*  if (!findSides(input, &c)) {
          free(input);
          printf("Nespravny vstup.\n");
          return 1;
      }*/

    treeNode root = treeNode(0);
    clock_t gen = clock();
    generateUniverse(c, &root,
                     c.arms[0].tokenAmount + c.arms[1].tokenAmount + c.arms[2].tokenAmount + c.arms[3].tokenAmount, 0,
                     0, 0, 0);
    clock_t genEnd = clock();


    clock_t evalStart = clock();
    evalTree(&root);
    clock_t evalEnd = clock();

    // takes 1.5s because of the massive depth
    // also allocates a LOT of memory
    printf("genUniverse time: %lf", ((double) (genEnd - gen)) / CLOCKS_PER_SEC);

    printf("evalTree time: %lf", ((double) (evalEnd - evalStart)) / CLOCKS_PER_SEC);

    int asum = 0;
    int bsum = 0;
    walk(&root, &asum, &bsum, true);
    printf("Celkem A/B: %d/%d\n", asum, bsum);

    //free memory
    free(input);
    root.destroy();

    return 0;

}
