#ifndef BIGINTEGER_H
#define BIGINTEGER_H

struct BigInt;
typedef struct BigInt BigInt;

void Div_subtract(BigInt *minuend, const BigInt *subtrahend);
BigInt* BigInt_create(const char *a);
void BigInt_subtract(BigInt *minuend, const BigInt *subtrahend);
void BigInt_add(BigInt *addend1, const BigInt *addend2);
void BigInt_multiply(BigInt *multiplicant, const BigInt *multiplier);
void BigInt_divide(BigInt *dividend, const BigInt *divisor);
void BigInt_removeLeadingZeros(BigInt *a);
void BigInt_print(const BigInt* a);
void BigInt_delete(BigInt *a);
void BigInt_copy(BigInt *copy, const BigInt *original);
void BigInt_makeZero(BigInt *a);
int BigInt_isZero(const BigInt *a);
int BigInt_compareWithoutSign(const BigInt* a, const BigInt* b);
int BigInt_compare(const BigInt *a, const BigInt *b);

#endif
