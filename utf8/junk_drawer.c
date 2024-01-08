//
// Created by Sarale Goldberger on 12/31/2023.
//

#include <stdio.h>

int main() {
    for (int i = 0; i < 21; i++) {
        if (i==5) i+=10;
        printf("%d\n", i);

    }
    return 0;
}

// Returns the amount of characters in an ASCII string
int my_strlen(unsigned char *string) {
    int count = 0;
    while (*string != '\0') {
        count++;
        string++;
    }
    return count;
} //end my_strlen

int my_strcpy_upper(char *input, char *output) {
    while (*input != '\0') {
        // if character is lowercase convert to uppercase and append
        if (*input >= 'a' && *input <= 'z')
            *output = (*input - 'a' + 'A');
            // if it's already uppercase just append
        else
            *output = *input;
        output++;
        input++;
    }// end while
    *output++ = '\0';
    return 0;
}//end my_strcpy_upper

// Takes in a string corresponding to a currency and returns the UTF-8 encoded string and the Unicode code point in \uXXX notation
int my_utf8_currency(char *cur, char *utf8, char *unicode){
    char cur_upper[my_strlen(cur)+1];
    my_strcpy_upper(cur, cur_upper);
    if (cur == "USD") {
        utf8[0] = 0x24;
        utf8[1] = '\0';
//        my_utf8_decode(utf8, unicode);
        return 1;
    } //else if () {
//    }
    return 0;
}//end my_utf8_currency


int test_my_utf8_currency(char *input) {
    char utf8[5];
    char unicode[8];
    my_utf8_currency(input, utf8, unicode);
    printf("Currency: %s\tUTF-8: %s\tUnicode: %s\n", input, utf8, unicode);
    return 0;
}//end test_my_utf8_currency
int my_utf8_currency_tests() {
//    test_header("my_utf8_currency");
    test_my_utf8_currency("USD");
    test_my_utf8_currency("Usd");
    test_my_utf8_currency("usd");
}//end my_utf8_currency_tests


// Takes in a string corresponding to a language and returns a string which is hello in that language
int my_utf8_hello(){
    return 0;
}

// Returns 1 if the character indicates the end of the word/string
int EOS(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
} //end EOS

//unsigned char *my_utf8_charat(unsigned char *string, int index) {
//    // If index is out of range return NULL
//    if ((index >= my_utf8_strlen(string)) || (index < 0))
//        return NULL;
//
//    unsigned char output[7];
//    // increment string to point to the index position of the character we want to return
//    string += index;
//    // Determine the number of bytes and append the correct amount of bytes to output
//    int bytes = my_utf8_numbytes(string);  // What a helpful function!!
//    printf("bytes %d\n", bytes);
//    int i = 0;
//    while (i <= bytes) {
//        output[i++] = *string++;
//        printf("output %s  c %c 0x%X\n", output, *string, *string);
//    }
//    // Terminate output string and return a pointer to it
//    printf("i %d output %s\n", i, output);
//    output[i] = '\0';
//    unsigned char *ptr = output;
//    return ptr;
//}

//
//// Given a UTF8 encoded Hebrew string, return the corresponding gematria
//int my_utf8_gematria_encode(unsigned char *input){
//    int byte2, base = 0x8F, ch, count = 1;
//    static int offset = 0;
//    // Loop through the string
//    while (*input != '\0') {
//        // Make sure input is within the range {0xD7, 0x90} - {0xD7, 0xAF}
//        if (my_utf8_check(input) == 0) return 0;
//        if (my_utf8_numbytes(input) != 2) return 0;
//        byte2 = *(input+1);
//        printf("0x%x\n", byte2);
//        if ((*input < 0xD7) && (byte2 < 0x90 || byte2 > 0xAF)) return 0;
//
//        ch = byte2 - base - offset;
//        for (int i = 1; i < ch; i++) {
//            count++;
//            if (i == 11 || i == 14 || i == 16 || i == 20 || i == 22) count-=1;
//        }
//        input += 2;
//    }//end while
//    return count;
//}// end my_utf8_hebrew


/// INCORRECT COUNTING
//// Given a UTF8 encoded Hebrew string, return the corresponding gematria
//int my_utf8_gematria_encode(unsigned char *input){
//    int byte2, base = 0x8F, ch, count = 1;
//    static int offset = 0;
//    // Loop through the string
//    if (*input != '\0') {
//        // Make sure input is within the range {0xD7, 0x90} - {0xD7, 0xAF}
//        if (my_utf8_check(input) == 0) return 0;
//        if (my_utf8_numbytes(input) != 2) return 0;
//        byte2 = *(input+1);
//        printf("0x%x\n", byte2);
//        if ((*input < 0xD7) && (byte2 < 0x90 || byte2 > 0xAF)) return 0;
//
//        ch = byte2 - base - offset;
//        for (int i = 1; i < ch; i++) {
//            count++;
//            if (i == 11 || i == 14 || i == 16 || i == 20 || i == 22) count-=1;
//        }
//    }//end if
//    return count;
//}// end my_utf8_hebrew

//
//int my_utf8_strcmp(unsigned char *string1, unsigned char *string2) {
//    int numbytes1, numbytes2;
//    // Loop through the string
//    while (*string1 != '\0' && *string2 != '\0') {
//        numbytes1 = my_utf8_numbytes(string1);
//        numbytes2 = my_utf8_numbytes(string2);
//        // If string1's current character has more bytes than string2's, string1 is  lexicographically larger
//        if (numbytes1 > numbytes2)
//            return 1;
//        // If string1's current character has less bytes than string2's, string1 is  lexicographically smaller
//        if (numbytes1 < numbytes2)
//            return -1;
//        // If string1 and string2's current characters have the same number of bytes
//        // compare the character per byte
//        if (numbytes1 == numbytes2) {
//            for (int i = 1; i <= numbytes1; i++) {
//                if (*string1 > *string2)
//                    return 1;
//                if (*string1 < *string2)
//                    return -1;
//                string1++;
//                string2++;
//            }//end for
//        }//end if ==
//    }//end while
//
//    // if we fall out of the loop it means all characters checked are the same, and either;
//    // the strings are equal length, return 0
//    // string1 is longer than string2, return 1
//    // string1 is shorter than string2, return -1
//    printf("%c, %c\n", *string1, *string2);
//    if (*string1 == '\0' && *string2 == '\0') {
//        printf("same len\n");
//        return 0;
//    }
//    if (*string2 == '\0') {
//        printf("string 2 shorter\n");
//        return 1;
//    }
//    if (*string1 == '\0') {
//        printf("string 1 shorter\n");
//        return -1;
//    }
//    return 2; //function should never return 2
//}//end my_utf8_strcmp

//// Given an int, return a UTF8 encoded Hebrew character that corresponds to that int (i.e. its gematria)
//int my_utf8_gematria_decode(int ind, unsigned char *output, int *g){
//    unsigned int base = 0x8F;
//    // Make sure ind is a valid index
//    if ((ind < 1) || (ind > 27)) {
//        // If invalid, function returns 0x0
//        output[0] = '0';
//        output[1] = '\0';
//        output[2] = '\0';
//        return 0; //invalid input
//    }//end if invalid
//    // The first byte will always be 0xD7
//    output[0] = 0xD7;
//    // We need to account for final letters, which should be skipped over
//    if (ind > 10) base++; //=10
//    if (ind > 13) base++;
//    if (ind > 15) base++;
//    if (ind > 19) base++;
//    if (ind > 21) base++;
////    if (ind > 90) base+=100;
//    output[1] = (base + ind);
//    output[2] = '\0';
//    return 1;
//}// end my_utf8_hebrew


//int test_my_utf8_gematria_decode(int ind, int g, unsigned char *expected, int *exp_g) {
//    // Initialize an output string and invoke the function
//    unsigned char output[3];
//    my_utf8_gematria_decode(ind, output, g);
//    // Print the input, output, and expected output
//    printf("Index: %d\nCharacter:   ", ind);
//    my_utf8_printHex(output, 1);
//    printf("Gematria: %d\nExpected: ", g);
//    my_utf8_printHex(expected, 1);
//    printf("             %d\n", exp_g);
//    // print a line of '-' and assert if output matched expected output
//    int match = my_utf8_strcmp(expected, output);
//    match == 0 ? NUM_PASS++ : NUM_FAIL++;          // increment pass/fail counters
//    for (int i = 0; i < BUFFER; i++)
//        printf("-");
//    printf(" %s\n", match == 0 ? "PASSED" : "FAILED");
//    return 0;
//}//end test_my_utf8_gematria_decode
//int my_utf8_gematria_decode_tests(void) {
//    test_header("my_utf8_gematria_tests");
//    // Initialize the base UTF8 character, Aleph
//    char base[3] = {0xD7, 0x90, 0x0};
//    // Test for the aleph-bet
//    int incr = 1;
//    for (int i = 1; i < 28; i++) {
//        test_my_utf8_gematria_decode(i, base);
//        base[1]++;
//        if (i == 10 || i == 13|| i == 15 || i == 19 || i == 21) base[1]+=incr;
////        if (i == 11) {
////            incr = 10;
////        }
////        if (i == 100) {
////            incr = 100;
////        }
//    }//end test
//    // Test for invalid index
//    test_my_utf8_gematria_decode(28, "0");
//    test_my_utf8_gematria_decode(770, "0");
//}//end my_utf8_gematria_decode_tests