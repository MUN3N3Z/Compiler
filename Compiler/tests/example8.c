long bar(long arg1)
{
    long temp1 = 3 * 4;
    long temp2 = arg1 + temp1;
    long temp3 = temp2 * temp2;
    return temp3;
}

long foo(long arg1, long arg2)
{
    long temp1 = bar(arg1);
    return temp1;
}
