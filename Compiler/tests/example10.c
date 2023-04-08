long foo(long arg1, long arg2)
{
    long temp1 = arg1 + arg2;
    long temp2 = arg2 - 3;
    long temp3 = temp1 + temp2;
    long temp4 = temp3 / temp3;
    long temp5 = -temp4;
    return temp1;
}
