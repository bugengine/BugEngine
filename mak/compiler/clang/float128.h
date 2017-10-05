struct __float128
{
    __float128(int);
    bool operator<(__float128) const;
    bool operator>(__float128) const;
    bool operator<=(__float128) const;
    bool operator>=(__float128) const;
    bool operator==(__float128) const;
    bool operator!=(__float128) const;
    float operator+(__float128) const;
    float operator-(__float128) const;
    float operator*(__float128) const;
    float operator/(__float128) const;
    float operator-() const;
};
