#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigInteger.h"

struct BigInt {
    char *number;
    int length;
    int sign;
};


// compares to BigInt with signs, returns: 0 if the first is bigger, 1 if the second is bigger, 2 if they are equal
int compareWithSign(const BigInt *a, const BigInt *b) {
    if (a->sign == 1 && b->sign == 0) {
        return 0;
    } else if (a->sign == 0 && b->sign == 1) {
        return 1;
    } else if (a->sign == 1 && b->sign == 1) {
        if (a->length > b->length) {
            return 1;
        } else if (b->length > a->length) {
            return 0;
        } else {
            for (int i = 0; i < a->length; i++) {
                if (a->number[i] == b->number[i]) {
                    continue;
                } else if (a->number[i] > b->number[i]) {
                    return 1;
                } else {
                    return 0;
                }
            }
            return 2;
        }
    } else {
        if (a->length > b->length) {
            return 0;
        } else if (b->length > a->length) {
            return 1;
        } else {
            for (int i = 0; i < a->length; i++) {
                if (a->number[i] == b->number[i]) {
                    continue;
                } else if (a->number[i] > b->number[i]) {
                    return 0;
                } else {
                    return 1;
                }
            }
            return 2;
        }
    }
}

// checks if a BigInt is zero
int BigInt_isZero(const BigInt *a) {
    BigInt *copy = malloc(sizeof(BigInt));
    if (copy == NULL) { perror("Malloc fail"); exit(-1); }
    copy->number = NULL;
    BigInt_copy(copy, a);
    BigInt_removeLeadingZeros(copy);
    if (copy->length == 1 && copy->number[0] == 0) {
        BigInt_delete(copy);
        return 1;
    } else {
        BigInt_delete(copy);
        return 0;
    }
}

// eradicates leading zero's
void BigInt_removeLeadingZeros(BigInt *a) {
    while (a->number[0] == 0 && a->length > 1) {
        for (int i = 0; i < a->length-1; i++) {
            a->number[i] = a->number[i+1];    
        }
        a->length--;
    }
    a->number = realloc(a->number, sizeof(char)*a->length);
    if (a->number == NULL) { perror("Realloc fail"); exit(-1); }
    if (a->number[0] == 0 && a->sign == 1) {
        a->sign = 0;
    }
}

// makes all the digits of a number zero
void BigInt_makeZero(BigInt *a) {
    for(int i = 0; i < a->length; i++) {
        a->number[i] = 0;
    }
}

// creates BigInt
BigInt* BigInt_create(const char* x) {
    BigInt* bigInt = malloc(sizeof(BigInt));
    if (bigInt == NULL) { perror("Malloc fail"); exit(-1); }
    if (x[0] == '-') {
        bigInt->sign = 1;
        bigInt->length = strlen(x)-1;

    } else {
        bigInt->sign = 0;
        bigInt->length = strlen(x);
    }
    bigInt->number = malloc(bigInt->length*sizeof(char));
    if (bigInt->number == NULL) { perror("Malloc fail"); exit(-1); }
    int numberIndex = 0;
    for (int i = 0; i < strlen(x); i++) {
        if (bigInt->sign && i == 0) {
            continue;   
        }
        bigInt->number[numberIndex++] = x[i]-48;
    }
    BigInt_removeLeadingZeros(bigInt);
    if (bigInt->number[0] == 0 && bigInt->sign == 1) {
        bigInt->sign = 0;
    }
    return bigInt;
}

// prints out BigInt
void BigInt_print(const BigInt* a) {
    if (a->sign == 1) {
       printf("-");
    }
    for (int i = 0; i < a->length; i++) {
        printf("%d", a->number[i]);
    }
    printf("\n");
}

// compares two BigInt without sign, returns: 0 if a is bigger, 1 if b is bigger, 2 if they are equal
int BigInt_compareWithoutSign(const BigInt* a, const BigInt* b) {
    BigInt *aCopy = malloc(sizeof(BigInt));
    if (aCopy == NULL) { perror("Malloc fail"); exit(-1); }
    aCopy->number = NULL;
    BigInt_copy(aCopy, a);
    BigInt *bCopy = malloc(sizeof(BigInt));
    if (bCopy == NULL) { perror("Malloc fail"); exit(-1); }
    bCopy->number = NULL;
    BigInt_copy(bCopy, b);
    BigInt_removeLeadingZeros(aCopy);
    BigInt_removeLeadingZeros(bCopy);
    if (aCopy->length > bCopy->length) {
        BigInt_delete(aCopy);
        BigInt_delete(bCopy);
        return 0;
    } else if (bCopy->length > aCopy->length) {
        BigInt_delete(aCopy);
        BigInt_delete(bCopy);
        return 1;
    } else {
        for (int i = 0; i < aCopy->length; i++) {
            if (aCopy->number[i] == bCopy->number[i]) {
                continue;
            } else if (aCopy->number[i] > bCopy->number[i]) {
                BigInt_delete(aCopy);
                BigInt_delete(bCopy);
                return 0;
            } else {
                BigInt_delete(aCopy);
                BigInt_delete(bCopy);
                return 1;
            }
        }
        BigInt_delete(aCopy);
        BigInt_delete(bCopy);
        return 2;
    }
}

// deletes BigInt
void BigInt_delete(BigInt *a) {
    free(a->number);
    free(a);
}

// copies BigInt
void BigInt_copy(BigInt *copy, const BigInt *original) {
    if (copy->number != NULL) {
        free(copy->number);
    }
    copy->number = malloc(sizeof(char)*original->length);
    if (copy->number == NULL) { perror("Malloc fail"); exit(-1); }
    for (int i = 0; i < original->length; i++) {
        copy->number[i] = original->number[i];
    }
    copy->sign = original->sign;
    copy->length = original->length;
}

// adds two BigInt
void BigInt_add (BigInt *addend1, const BigInt *addend2) {
    int compareResult = BigInt_compareWithoutSign(addend1, addend2);
    int sumSign = 0;
    if (addend1->sign == 1 && addend2->sign == 1) {
        sumSign = 1;        
    } else if (addend1->sign == 1 && addend2->sign == 0) {
        sumSign = 1;
        addend1->sign = 0;
        BigInt_subtract(addend1, addend2); 
        addend1->sign = sumSign;
        BigInt_removeLeadingZeros(addend1);
        return;
    } else if (addend1->sign == 0 && addend2->sign == 1) {
        sumSign = 0;
        BigInt_subtract(addend1, addend2);
        addend1->sign = sumSign;
        BigInt_removeLeadingZeros(addend1);
        return;
    }

    BigInt *sum = malloc(sizeof(BigInt));
    if (sum == NULL) { perror("Malloc fail"); exit(-1); }
    BigInt *biggerNumber = malloc(sizeof(BigInt));
    if (biggerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    biggerNumber->number = NULL;
    BigInt *smallerNumber = malloc(sizeof(BigInt));
    if (smallerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    smallerNumber->number = NULL;

    if (compareResult == 1) {
        BigInt_copy(biggerNumber, addend2);
        BigInt_copy(smallerNumber, addend1);
        sum->number = calloc(addend2->length+1, sizeof(char)); 
        if (sum->number == NULL) { perror("Calloc fail"); exit(-1); }
        sum->length = addend2->length+1;
    } else {
        BigInt_copy(biggerNumber, addend1);
        BigInt_copy(smallerNumber, addend2);
        sum->number = calloc(addend1->length+1, sizeof(char));
        if (sum->number == NULL) { perror("Calloc fail"); exit(-1); }
        sum->length = addend1->length+1;
    }

    BigInt_removeLeadingZeros(biggerNumber);
    BigInt_removeLeadingZeros(smallerNumber);

    int carry = 0;

    int indexBN = biggerNumber->length-1;
    int indexSN = smallerNumber->length-1;

    for (int i = sum->length-1; i >= 0; i--) {
        if (carry == 1) {
            sum->number[i] += 1;
            carry = 0;
        }
        if (indexBN >= 0 && indexSN >= 0) {
            sum->number[i] += biggerNumber->number[indexBN] + smallerNumber->number[indexSN];
        } else if (indexBN >= 0) {
            sum->number[i] += biggerNumber->number[indexBN];
        }
        if (sum->number[i] > 9) {
            carry = 1;
            sum->number[i] %= 10;
        }
        indexBN--;
        indexSN--;
    }

    BigInt_delete(biggerNumber);
    BigInt_delete(smallerNumber);
    BigInt_copy(addend1, sum);
    BigInt_delete(sum);
    addend1->sign = sumSign;
    BigInt_removeLeadingZeros(addend1);
}

void Div_subtract(BigInt *minuend, const BigInt *subtrahend) {
    int compareResult = BigInt_compareWithoutSign(minuend, subtrahend);

    BigInt *biggerNumber = malloc(sizeof(BigInt));
    if (biggerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    biggerNumber->number = NULL;
    BigInt *smallerNumber = malloc(sizeof(BigInt));
    if (smallerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    smallerNumber->number = NULL;

    if (compareResult == 1) {
        BigInt_copy(biggerNumber, subtrahend);
        BigInt_copy(smallerNumber, minuend);
    } else {
        BigInt_copy(biggerNumber, minuend);
        BigInt_copy(smallerNumber, subtrahend);
    }

    BigInt_removeLeadingZeros(smallerNumber);

    int indexSN = smallerNumber->length-1;

    int borrow = 0;

    for (int i = biggerNumber->length-1; i >= 0; i--) {
        if (borrow == 1) {
            biggerNumber->number[i]--;
            if (biggerNumber->number[i] < 0) {
                borrow = 1;
                biggerNumber->number[i] = 9;
            } else {
                borrow = 0;
            }
        }
        if (indexSN >= 0) {
            if (biggerNumber->number[i] < smallerNumber->number[indexSN]) {
                biggerNumber->number[i] += 10;    
                borrow = 1;
            }
            biggerNumber->number[i] -= smallerNumber->number[indexSN];
            indexSN--;
        }
        if (borrow == 0 && indexSN < 0) {
            break;
        }
    }
    for (int i = 0; i < biggerNumber->length; i++) {
        if (biggerNumber->number[i] != minuend->number[i]) {
            minuend->number[i] = biggerNumber->number[i];
        }
    }
    BigInt_delete(smallerNumber);
    BigInt_delete(biggerNumber);
}

// atimami skaiciai, rezultatas issaugomas summand1
void BigInt_subtract(BigInt *minuend, const BigInt *subtrahend) {
    BigInt *subtrahendCopy = malloc(sizeof(BigInt));
    if (subtrahendCopy == NULL) { perror("Malloc fail"); exit(-1); }
    subtrahendCopy->number = NULL;
    BigInt_copy(subtrahendCopy, subtrahend);
    int skirtumoZenklas = 0;
    int compareResult = BigInt_compareWithoutSign(minuend, subtrahend);
    if (minuend->sign == 1 && subtrahend->sign == 1) {
        if (compareResult == 1) {
            skirtumoZenklas = 0;
        } else if (compareResult == 0) {
            skirtumoZenklas = 1;
        }
    } else if (minuend->sign == 1 && subtrahend->sign == 0) {
        skirtumoZenklas = 1;
        minuend->sign = 0;
        BigInt_add(minuend, subtrahend);
        minuend->sign = skirtumoZenklas;
        BigInt_removeLeadingZeros(minuend);
        BigInt_delete(subtrahendCopy);
        return;
    } else if (minuend->sign == 0 && subtrahend->sign == 1) {
        subtrahendCopy->sign = 0;
        BigInt_add(minuend, subtrahendCopy);
        BigInt_removeLeadingZeros(minuend);
        BigInt_delete(subtrahendCopy);
        return;
    } else if (minuend->sign == 0 && subtrahend->sign == 0) {
        if (compareResult == 1) {
            skirtumoZenklas = 1;
        } else if (compareResult == 0) {
            skirtumoZenklas = 0;
        }
    }

    BigInt_delete(subtrahendCopy);

    BigInt *biggerNumber = malloc(sizeof(BigInt));
    if (biggerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    biggerNumber->number = NULL;
    BigInt *smallerNumber = malloc(sizeof(BigInt));
    if (smallerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    smallerNumber->number = NULL;

    if (compareResult == 1) {
        BigInt_copy(biggerNumber, subtrahend);
        BigInt_copy(smallerNumber, minuend);
    } else {
        BigInt_copy(biggerNumber, minuend);
        BigInt_copy(smallerNumber, subtrahend);
    }

    BigInt_removeLeadingZeros(biggerNumber);
    BigInt_removeLeadingZeros(smallerNumber);

    int indexSN = smallerNumber->length-1;

    int borrow = 0;

    for (int i = biggerNumber->length-1; i >= 0; i--) {
        if (borrow == 1) {
            biggerNumber->number[i]--;
            if (biggerNumber->number[i] < 0) {
                borrow = 1;
                biggerNumber->number[i] = 9;
            } else {
                borrow = 0;
            }
        }
        if (indexSN >= 0) {
            if (biggerNumber->number[i] < smallerNumber->number[indexSN]) {
                biggerNumber->number[i] += 10;    
                borrow = 1;
            }
            biggerNumber->number[i] -= smallerNumber->number[indexSN];
            indexSN--;
        }
        if (borrow == 0 && indexSN < 0) {
            break;
        }
    }
    BigInt_copy(minuend, biggerNumber);
    BigInt_delete(smallerNumber);
    BigInt_delete(biggerNumber);
    minuend->sign = skirtumoZenklas;
    BigInt_removeLeadingZeros(minuend);
}


void BigInt_multiply(BigInt *multiplicand, const BigInt *multiplier) {
    int productSign = 0;
    if (multiplicand->sign != multiplier->sign) {
        productSign = 1;    
    }

    BigInt *biggerNumber = malloc(sizeof(BigInt));
    if (biggerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    biggerNumber->number = NULL;
    BigInt *smallerNumber = malloc(sizeof(BigInt));
    if (smallerNumber == NULL) { perror("Malloc fail"); exit(-1); }
    smallerNumber->number = NULL;

    int compareResult = BigInt_compareWithoutSign(multiplicand, multiplier);

    if (compareResult == 1) {
        BigInt_copy(biggerNumber, multiplier);
        BigInt_copy(smallerNumber, multiplicand);
    } else {
        BigInt_copy(biggerNumber, multiplicand);
        BigInt_copy(smallerNumber, multiplier);
    }

    BigInt_removeLeadingZeros(biggerNumber);
    BigInt_removeLeadingZeros(smallerNumber);

    BigInt *summand = malloc(sizeof(BigInt));
    if (summand == NULL) { perror("Malloc fail"); exit(-1); }
    summand->number = calloc(biggerNumber->length+1, sizeof(char));
    if (summand->number == NULL) { perror("Calloc fail"); exit(-1); }
    summand->length = biggerNumber->length+1;
    summand->sign = 0;

    BigInt *product = BigInt_create("0");

    int carry = 0;
    int indexSummand;
    for (int i = smallerNumber->length-1; i >= 0; i--) {
        indexSummand = biggerNumber->length;
        for (int k = biggerNumber->length-1; k >= 0; k--) {
            if (carry != 0) {
                summand->number[indexSummand] += carry;
                carry = 0;
            }
            summand->number[indexSummand] += biggerNumber->number[k] * smallerNumber->number[i];
            if (summand->number[indexSummand] > 9) {
                carry = summand->number[indexSummand] / 10;
                summand->number[indexSummand] %= 10;
            }
            indexSummand--;
        }

        if (carry != 0) {
            summand->number[indexSummand] = carry;
            carry = 0;
        }

        BigInt_add(product, summand);

        if (i != 0) {
            summand->length++;
            summand->number = realloc(summand->number, sizeof(char)*summand->length);
            if (summand->number == NULL) { perror("Realloc fail"); exit(-1); }
            BigInt_makeZero(summand);        
        }
    }

    BigInt_delete(biggerNumber);
    BigInt_delete(smallerNumber);
    BigInt_delete(summand);
    BigInt_copy(multiplicand, product);
    multiplicand->sign = productSign;
    BigInt_delete(product);
}

void BigInt_divide(BigInt *dividend, const BigInt *divisor) {
    if (BigInt_isZero(divisor) == 1) {
        printf("Dalyba is nulio negalima\n");
        return; 
    }

    int quotientSign = 0;
    if (dividend->sign != divisor->sign) {
        quotientSign = 1;    
    }

    int compareResult = BigInt_compareWithoutSign(dividend, divisor);
    if (compareResult == 1) {
        BigInt *zero = BigInt_create("0");
        BigInt_copy(dividend, zero);
        BigInt_delete(zero);
        return;
    } else if (compareResult == 2) {
        BigInt *one = BigInt_create("1");
        BigInt_copy(dividend, one);
        BigInt_delete(one);
        dividend->sign = quotientSign;
        return;
    }
    
    BigInt *dividendCopy = malloc(sizeof(BigInt));
    if (dividendCopy == NULL) { perror("Malloc fail"); exit(-1); }
    dividendCopy->number = NULL;
    BigInt *divisorCopy = malloc(sizeof(BigInt));
    if (divisorCopy == NULL) { perror("Malloc fail"); exit(-1); }
    divisorCopy->number = NULL;
    BigInt_copy(dividendCopy, dividend);

    BigInt_removeLeadingZeros(dividendCopy);

    BigInt *quotient = malloc(sizeof(BigInt));
    if (quotient == NULL) { perror("Malloc fail"); exit(-1); }
    quotient->number = malloc(dividend->length*sizeof(char));
    if (quotient->number == NULL) { perror("Malloc fail"); exit(-1); }
    quotient->length = 1;

    dividendCopy->length = 1;

    int quotientDigit = 0;
    int quotientIndex = 0;

    int dividendCopyLength = 0;

    BigInt_copy(divisorCopy, divisor);
    

    while (1) {
        if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 0) {
dividendCopyDidesnis: ;
            BigInt *five = BigInt_create("5");
            BigInt_multiply(divisorCopy, five);
            BigInt_delete(five);
            quotientDigit += 5;
            if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 0) {
                while (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 0) {
                    BigInt_add(divisorCopy, divisor);
                    quotientDigit++;
                }
                if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 1) {
                    Div_subtract(divisorCopy, divisor);
                    quotientDigit--;
                }
                dividendCopyLength = dividendCopy->length;
                Div_subtract(dividendCopy, divisorCopy);
                quotient->number[quotientIndex++] = quotientDigit;
            } else if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 1) {
                while (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 1) {
                    BigInt_subtract(divisorCopy, divisor);
                    quotientDigit--;
                }
                Div_subtract(dividendCopy, divisorCopy);
                quotient->number[quotientIndex++] = quotientDigit;
            } else {
                Div_subtract(dividendCopy, divisorCopy);
                quotient->number[quotientIndex++] = quotientDigit;
            }
        } else if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 1) {
            int counter = 0;
            while (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 1) {
                dividendCopy->length++;
                counter++;
                if (counter % 2 == 0) {
                    quotient->number[quotientIndex++] = 0;
                    quotient->length++;
                    counter++;
                }
                int temp = dividendCopy->length;
                dividendCopy->length = dividend->length;
                if (BigInt_compareWithoutSign(dividendCopy, divisor) != 0) {
                    quotient->number[quotientIndex++] = 0;
                    quotient->length++;
                    goto divisionEnd;
                } 
                dividendCopy->length = temp;
            }
            if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 0) {
                goto dividendCopyDidesnis;
            } else if (BigInt_compareWithoutSign(dividendCopy, divisorCopy) == 2) {
                Div_subtract(dividendCopy, divisorCopy);    
                quotient->number[quotientIndex++] = 1;
            }
        } else {
            Div_subtract(dividendCopy, divisorCopy);
            quotient->number[quotientIndex++] = 1;
        }
        int temp = dividendCopy->length;
        dividendCopy->length = dividend->length;
        BigInt_copy(divisorCopy, divisor);
        if (BigInt_compareWithoutSign(dividendCopy, divisor) == 0) {
            dividendCopy->length = temp;
            quotient->length++;
            quotientDigit = 0;
        } else if (BigInt_compareWithoutSign(dividendCopy, divisor) == 2) {
            Div_subtract(dividendCopy, divisorCopy);
            quotient->number[quotientIndex++] = 1;
            break;
        } else {
            dividendCopy->length = temp;
            int counter = 0;
            while (dividendCopy->length <= dividend->length) {
                dividendCopy->length++;
                counter++;
                if (counter % 2 == 0) {
                    quotient->number[quotientIndex++] = 0;
                    quotient->length++;
                    counter++;
                }
            }
            break;    
        }
    }


divisionEnd:
    quotient->length = quotientIndex;
    BigInt_delete(dividendCopy);
    BigInt_delete(divisorCopy);
    BigInt_copy(dividend, quotient);
    BigInt_removeLeadingZeros(dividend);
    BigInt_delete(quotient);
    dividend->sign = quotientSign;
}

