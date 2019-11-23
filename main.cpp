#include <stdio.h>
#include <string.h>
#include "BloomFilter.h"

int main()
{
    CBloomFilter bloom;

    char *test1 = "hehehehsadfasdalkdnsal fjlsdnflaskndflsakndf sdlfj ksdjf asdi jjjdfk asdlsdfsadfnsalg";
    char *test2 = "hehehehsadfasdalkdnsal fjlsdnflaskndflsakndf sdlfj ksdjf asdi jjjdfk asdlsdfsadfnsalg.";

    bloom.add(test1, strlen(test1));
    bloom.add(test2, strlen(test2));

    if (bloom.check(test1, strlen(test1)))
    {
        printf("bloom has test1!\n");
    }
    else
    {
        printf("bloom do not has test1!\n");
    }

    if (bloom.check(test2, strlen(test2)))
    {
        printf("bloom has test2!\n");
    }
    else
    {
        printf("bloom do not has test2!\n");
    }

    return 0;
}