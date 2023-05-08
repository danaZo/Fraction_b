#include "Fraction.hpp"

#include <cmath> // for round
using namespace std;
#include <algorithm>
#include <iomanip>
#include <sstream>


/*  Web sources:
    for input output overloading i was inspired from: https://coding-champ.com/tutorials/c-plus-plus/operator-overloading-for-input-and-output
    for overflow handeling (to pass the student's test): https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
    for converting float to fraction I used this video to get insparation: https://www.youtube.com/watch?v=h3QFUCJ_blM&ab_channel=EricGitangu
*/

namespace ariel {

    // implementation of the constructor
    Fraction::Fraction(int numer, int denomi){
        
        if (denomi == 0) {
            throw invalid_argument("Denominator cannot be zero");
        }

        // Reduce the fraction
        int gcd = __gcd(numer, denomi);
        numer /= gcd;
        denomi /= gcd;

        // i dont want fraction like this : 1/-2 its horrible
        // it should be -1/2
        if (denomi < 0) {
            numer = -numer;
            denomi = -denomi;
        }

        this->numerator = numer;
        this->denominator = denomi;
    }

    // implementation of default constructor
    Fraction::Fraction(){
        this->numerator = 0;
        this->denominator = 1;
    }

    // implementation of the float to Fraction constructor
    Fraction::Fraction(float flo){
        int denominator = 1;
        
        float floRounded = round(flo * 1000) / 1000; // round to 3 digits of accuracy

        int count = 0;
        
        while (floRounded - (int)floRounded != 0 && count != 3) {
            floRounded *= 10;
            denominator *= 10;
            count ++;
        }

        int numer = round(floRounded);
        
        this->numerator = numer;
        
        this->denominator = denominator;
        
    }

    // numerator getter
    int Fraction::getNumerator() const{
        return this->numerator;
    }

    // numerator setter
    void Fraction::setNumer(int n){
        this->numerator = n;
    }

    // denominator getter
    int Fraction::getDenominator() const{
        return this->denominator;
    }

    // denominator setter
    void Fraction::setDenomi(int n){
        this->denominator = n;
    }

    // implementation of the overloading of the operator << so it will print the fraction like this: 3/5
    ostream& operator<<(ostream& _os, const Fraction& fraction) {
        int gcd = __gcd(fraction.getNumerator(), fraction.getDenominator());
        int numer = fraction.getNumerator() / gcd;
        int denomi = fraction.getDenominator() / gcd;

        // i dont want fraction like this : 1/-2 its horrible
        // it should be -1/2
        if (denomi < 0) {
            numer = -numer;
            denomi = -denomi;
        }

        _os << numer << '/' << denomi;
        return _os;
    }

    // implementation of the overloading of >> operator so it will read the input fraction like this: 3/5
    istream& operator>>(istream& _is, Fraction& fraction){
        _is >> fraction.numerator >> fraction.denominator;

        if (_is.fail()) {
            throw runtime_error("Invalid input: Expected two numbers separated");
        }

        if (fraction.denominator == 0) {
            throw runtime_error("Invalid input: Denominator cannot be zero");
        }

        return _is;
    }

    // implementation of the overloading of the + operator
    Fraction operator+(const Fraction& frac1, const Fraction& frac2){
        Fraction result;

        // Check for overflow in numerator
        if (__builtin_add_overflow(frac1.getNumerator() * frac2.getDenominator(),
                                    frac2.getNumerator() * frac1.getDenominator(),
                                    &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(frac1.getDenominator(), frac2.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    // imlementation of the overloading of the operator + for Fraction + float
    Fraction operator+(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac + floatFrac;
    }

    // imlementation of the overloading of the operator + for float + Fraction 
    Fraction operator+(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac + frac;
    }

    // implementation of the overloading of the - operator
    Fraction operator-(const Fraction& frac1, const Fraction& frac2){
        Fraction result;
    
        // Check for overflow in numerator
        if (__builtin_sub_overflow(frac1.getNumerator() * frac2.getDenominator(), frac2.getNumerator() * frac1.getDenominator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(frac1.getDenominator(), frac2.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }
        
        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    // implementation of the overloading of the - operator for Fraction - float
    Fraction operator-(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo); // convert float to fraction
        return frac - floatFrac;
    }

    // implementation of the overloading of the - operator for float - Fraction
    Fraction operator-(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac - frac;
    }

    //implementation of the overloading of the / operator
    Fraction operator/(const Fraction& frac1, const Fraction& frac2){
        if (frac2.getNumerator() == 0) {
            throw runtime_error("Division by zero");
        }
        
        Fraction result;
        // Check for overflow in numerator
        if (__builtin_mul_overflow(frac1.getNumerator(), frac2.getDenominator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(frac1.getDenominator(), frac2.getNumerator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    // implementation of the overloading of the / operator for Fraction / float
    Fraction operator/(const Fraction& frac, const float& flo){
        if (flo == 0) {
            throw runtime_error("Division by zero");
        }
        
        Fraction floatFrac(flo); // convert float to fraction
        return frac/floatFrac;
    }

    // implementation of the overloading of the / operator for float / Fraction
    Fraction operator/(const float& flo, const Fraction& frac){

        if (frac.getNumerator() == 0) {
            throw runtime_error("Division by zero");
        }

        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac/frac;
    }

    //implementation of the overloading of the * operator
    Fraction operator*(const Fraction& frac1, const Fraction& frac2){
        Fraction result;
        
        // Check for overflow in numerator
        if (__builtin_mul_overflow(frac1.getNumerator(), frac2.getNumerator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(frac1.getDenominator(), frac2.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    // implementation of the overloading of the * operator for float * fraction
    Fraction operator*(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac*frac;
    }

    // implementation of the overloading of the * operator for fraction * float
    Fraction operator*(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo); // convert float to fraction
        return frac*floatFrac;
    }

    // implementation for the prefix increment operator
    Fraction& Fraction::operator++(){
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer + theDenomi);
        return *this;
    }

    // implementation for the postfix increment operator
    // the postfix increment operator should return the original value
    // before it was incremented, 
    // not the new value after the increment.
    Fraction Fraction::operator++(int){
        Fraction temp(*this); 
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer + theDenomi);
        return temp; 
    }

    // implementation for the pre-decrement operator
    Fraction& Fraction::operator--(){
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer - theDenomi);
        return *this;
    }

    // implementation for the post-decrement operator
    Fraction Fraction::operator--(int){
        Fraction temp(*this); 
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer - theDenomi);
        return temp; 
    }

    // implementation for the >= operator
    bool operator>=(const Fraction& frac1, const Fraction& frac2){
        int num1 = frac1.getNumerator() * frac2.getDenominator();
        int num2 = frac2.getNumerator() * frac1.getDenominator();
        return num1 >= num2;
    }

    // implementation for the >= operator between float to fraction
    bool operator>=(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac >= frac;
    }

    // implementation for overloading the <= operator
    bool operator<=(const Fraction& frac1, const Fraction& frac2){
        int num1 = frac1.getNumerator() * frac2.getDenominator();
        int num2 = frac2.getNumerator() * frac1.getDenominator();
        return num1 <= num2;
    }

    // implementation for overloading the <= operator between float to fraction
    bool operator<=(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac <= frac;
    }

    // implementation for the > operator
    bool operator>(const Fraction& frac1, const Fraction& frac2){

        int numer1 = frac1.getNumerator();
        int denomi1 = frac1.getDenominator();
        int gcd = __gcd(numer1, denomi1);
        Fraction frac1_reduced(numer1/gcd,denomi1/gcd);

        int numer2 = frac2.getNumerator();
        int denomi2 = frac2.getDenominator();
        int gcd2 = __gcd(numer2, denomi2);
        Fraction frac2_reduced(numer2/gcd2,denomi2/gcd2);


        int num1 = frac1_reduced.getNumerator() * frac2_reduced.getDenominator();
        int num2 = frac2_reduced.getNumerator() * frac1_reduced.getDenominator();
        
        return num1 > num2;
    }

    // implementation for the > operator between fraction to float
    bool operator>(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac.getNumerator() * floatFrac.getDenominator() > floatFrac.getNumerator() * frac.getDenominator();
    }

    // implementation for the > operator between float to fraction
    bool operator>(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac > frac;
    }

    // implementation for the < operator
    bool operator<(const Fraction& frac1, const Fraction& frac2){
        int numer1 = frac1.getNumerator();
        int denomi1 = frac1.getDenominator();
        int gcd = __gcd(numer1, denomi1);
        Fraction frac1_reduced(numer1/gcd,denomi1/gcd);

        int numer2 = frac2.getNumerator();
        int denomi2 = frac2.getDenominator();
        int gcd2 = __gcd(numer2, denomi2);
        Fraction frac2_reduced(numer2/gcd2,denomi2/gcd2);


        int num1 = frac1_reduced.getNumerator() * frac2_reduced.getDenominator();
        int num2 = frac2_reduced.getNumerator() * frac1_reduced.getDenominator();
        
        return num1 < num2;
    }

    // implementation for the < operator between float to fraction : float < fraction
    bool operator<(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac < frac;
    }

    // implementation for the < operator between fraction to float : fraction < float
    bool operator<(Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac < floatFrac;
    }

    // implementation for the == operator
    bool operator==(const Fraction& frac1, const Fraction& frac2){
        int numer1 = frac1.getNumerator() * frac2.getDenominator();
        int numer2 = frac2.getNumerator() * frac1.getDenominator();

        if (numer1 > numer2) return false;
        else if (numer1 < numer2) return false;
        else return true;
    }

    bool operator==(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac == frac;
    }

    bool operator==(Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac == floatFrac;
    }

    // implementation to find the Greatest Common Divisor (GCD) of two integers using the Euclidean algorithm
    int Fraction::getGCD(int first, int second){
        while (second != 0){
            int temp = second;
            second = first % second;
            first = temp;
        }
        return first;
    }

    int Fraction::getLCM(int first, int second){
        return (first * second) / getGCD(first, second);
    }
}