 long bar(long arg1, long arg2, long arg3) {
    long temp1 = 5 * arg1;
    long temp2 = arg2 / 2;
    long temp3 = temp2 / temp1;
    return temp3;
}
 
 long foo(long arg1, long arg2) {
    long temp1 = arg1 + arg2;
    long temp2 = bar(temp1, arg2, arg1);
    return temp2;
 }
