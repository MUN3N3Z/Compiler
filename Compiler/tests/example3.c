
long foo(long arg1, long arg2) 
{
    long temp1 = arg1 & arg2;
    long temp2 = temp1 | 2;
    long temp3 = temp2 ^ arg1;
    long temp4 = temp3 << 2;
    return temp4; 
}
