long boo (long arg1, long arg2) {
    long temp3 = (arg1 + 12);
    return temp3;
}
long foo (long arg1, long arg2) {
    long temp3 = (arg1 + 12);
    long temp4 = boo(24, temp3);
    long temp5 = (temp4 - temp3);
    return temp5;
}