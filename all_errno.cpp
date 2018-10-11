/* Function: obtain the errno string
*	char *strerror(int errno)
*/

#include <stdio.h>
#include <string.h>     //for strerror()
//#include <errno.h>
int main()
{
    int tmp = 0;
    for(tmp = 0; tmp <=256; tmp++)
    {
        printf("errno: %2d\t%s\n",tmp,strerror(tmp));
    }
    return 0;
}
