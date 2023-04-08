long boo(long arg1, long arg2)
{
    long temp1 = 7 + 5;
    long temp2 = 4 * 6;
    long temp3 = arg1 + temp1;
    long temp4 = arg2 * temp2;
    long temp5 = temp4 - temp3;
    return temp3;
}

long foo(long arg1, long arg2) 
{
    long temp1 = 7 + 5;
    long temp2 = 4 * 6;
    long temp3 = arg1 + temp1;
    long temp4 = boo(temp2, temp3);
    long temp5 = temp4 - temp3;
    return temp5;
}
