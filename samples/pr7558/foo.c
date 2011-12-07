void foo (unsigned char x){
    unsigned int ab[2*x];
    ab[0] = 5 + x;
    ab[1] = ab[0] + 10;
}
