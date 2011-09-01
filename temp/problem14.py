#!/usr/bin/python

def genCache(i, cache, n):
    c = 0
    cur = i
    flag = False

    if (cur >= n):
        flag = True
    else:
        if cache[cur] == 0:
            flag = True

    while flag:
        flag = False
        c = c + 1
        if(cur & 0x1) == 0:
            cur = ((cur + 1) * 3)
        else:
            cur = ((cur + 1) >> 1) - 1
        if (cur >= n):
            flag = True
        else:
            if cache[cur] == 0:
                flag = True
    res = c + cache[cur]

    if i < n:
        cache[i] = res

    return res

    
def problem14(n):
    cache = []
    cacheSize = n #config~~~
    i = 0
    while i < cacheSize:
        cache.append(0)
        i = i + 1

    cache[0] = 1
    i = 1
    s = 1
    p = 0
    while i < n:
        c = genCache(i, cache, cacheSize)
        if c > s:
            p = i
            s = c
        #print i
        i = i + 1
    return p + 1


print problem14(1000000)
