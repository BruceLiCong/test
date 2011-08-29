#!/usr/bin/python

def problem2(a,b,maxN):
    p = a;
    q = b;
    sum = 0;
    if (a & 0x1) == 0:
            sum += a;
    while q < maxN:
        print q;
        tmp = p
        if (q & 0x1) == 0:
            #print q;
            sum += q;
        p = q;
        q += tmp;

    return sum

print problem2(1,2,4000000);
