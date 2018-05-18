#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(void)
{
    /*
     *1. if you don't want to define the maxlenth of string, can do as follow.
     * char * test[5] = {"1111", "2222", "3333", "4444", "5555ddddd"};
     * test[5] the '5' is the number of elements
     * the point is the point of context
     *
     *2. define the 2 dimension
     * char str[3][20] = {"aaa", "xxxxxx", "pppp"};
     * 3 is the count fo elements
     * 20 is the maxlenth of the element
     * can't puts(str[0][1]). for str[0][1] is char.
     * just can printf("%c\n", str[0][1]. 
     * so you had better to use *str[3] to define string array.
     * -------------------------------------------
     * fix strcpy/malloc/strlen incompatible 
     * #include <string.h>
     */
    char * test[5] = {"1111", "2222", "3333", "4444", "5555"};
    char * (*p)[5] = &test;
    puts(p[0][0]);
    printf("%s\n", p[0][1]);
    
    int i;
    char * array[6];
    for(i=0; i<6; i++)
    {
        array[i] = (char *)malloc(sizeof(char));
    }
    strcpy(array[5], "pppp");
    puts(array[5]);

    char str[3][3] = {"aaa", "bbb", "ccc"};
    char str1[][3] = {"111", "222", "333"};
    puts(str[0]);  /* output is aaabbbccc */
    puts(str1[0]); /* output is 111222333 */
    //puts(str[0][2]);  segment error.
    //printf("%s\n",  &str[0][0]);
    printf("%c\n",  &str[0][0]);
    
    return 0;
}
