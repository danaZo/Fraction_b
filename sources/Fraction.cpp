#include "Fraction.hpp"
#include <cmath> // for round
#include <algorithm> // for gcd

using namespace std;

/*                              WEB SOURCES
    ======================================================================
    for input output overloading i was inspired from: 
    https://coding-champ.com/tutorials/c-plus-plus/operator-overloading-for-input-and-output
    for overflow handeling (to pass the student's test): 
    https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-overflow
    for converting float to fraction I used this video to get insparation: 
    https://www.youtube.com/watch?v=h3QFUCJ_blM&ab_channel=EricGitangu
    and : https://www.baeldung.com/cs/float-to-fraction
    ======================================================================
*/

namespace ariel {


    /*                          CONSTRUCTOR
    ======================================================================
            throwing exception when the denominator set to be 0
            reducing the fraction using the gcd from algorithm library 
            if the denominator received is negative i change it to be 
            positive and multiply the numerator by -1
    ======================================================================
    */

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

    /*                          CONSTRUCTOR
    ======================================================================
    */




    /*                      DEFAULT CONSTRUCTOR 
    ======================================================================
    */

    Fraction::Fraction(){
        this->numerator = 0;
        this->denominator = 1;
    }

    /*                      DEFAULT CONSTRUCTOR 
    ======================================================================
    */




    /*                 FLOAT TO FRACTION CONSTRUCTOR
    ======================================================================
     implementation of the float to Fraction constructor
     denominator set to 1 at the beginning
     the float is rounded to accuracy of 3 digits
     the counter used to not pass the limit of 3 digits
     in the while loop at every iteration i increase the rounded float 
     with mult of 10 and the denominator until the rounded float minus 
     the integer casting of the founded float is equal to 0 and the 
     counter is not 3
     when the while ends the numerator of the new fraction will be 
     the rounded form of the rounded float
     rounding again to make sure the number is integer
     and the denominator is set to be the denominator calculated 
     during the while loop
    ======================================================================
    */

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

    /*                 FLOAT TO FRACTION CONSTRUCTOR
    ======================================================================
    */




    /*                      GETTER AND SETTERS
    ======================================================================
    */
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
    /*                      GETTER AND SETTERS
    ======================================================================
    */




    /*                       OVERLOADING OF <<
    ======================================================================
     implementation of the overloading of the operator << so it will 
     print the fraction like this: 3/5
     also reducing the fraction using gcd
    ======================================================================
    */

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

    /*                       OVERLOADING OF <<
    ======================================================================
    */




    /*                       OVERLOADING OF >>
    ======================================================================
     implementation of the overloading of >> operator so it will read the 
     input fraction like this: 3/5
     throwing exceptions when the input invalid - only one number or not 
     integers and when the denominator is 0
    ======================================================================
    */

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

    /*                       OVERLOADING OF >>
    ======================================================================
    */




    /*                       OVERLOADING OF +
    ======================================================================
     implementation of the overloading of the + operator
    ======================================================================
    */

    Fraction Fraction::operator+(const Fraction& frac) const{
        Fraction result;

        // Check for overflow in numerator
        if (__builtin_add_overflow(this->getNumerator() * frac.getDenominator(),
                                    frac.getNumerator() * this->getDenominator(),
                                    &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(this->getDenominator(), frac.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    /*                       OVERLOADING OF +
    ======================================================================
    */




    /*                 OVERLOADING OF FRACTION + FLOAT
    ======================================================================
    imlementation of the overloading of the operator + for Fraction + float
    ======================================================================
    */ 

    Fraction operator+(const Fraction& frac, const float& flo){
        const Fraction floatFrac(flo);
        return frac + floatFrac;
    }

    /*                 OVERLOADING OF FRACTION + FLOAT
    ======================================================================
    */ 




    /*                 OVERLOADING OF FLOAT + FRACTION
    ======================================================================
    imlementation of the overloading of the operator + for float + Fraction
    ======================================================================
    */ 

    Fraction operator+(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac + frac;
    }

    /*                 OVERLOADING OF FLOAT + FRACTION
    ======================================================================
    */




    /*                       OVERLOADING OF -
    ======================================================================
    implementation of the overloading of the - operator
    ======================================================================
    */ 
    
    Fraction Fraction::operator-(const Fraction& frac) const{
        Fraction result;
    
        // Check for overflow in numerator
        if (__builtin_sub_overflow(this->getNumerator() * frac.getDenominator(), frac.getNumerator() * this->getDenominator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(this->getDenominator(), frac.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }
        
        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    /*                       OVERLOADING OF -
    ======================================================================
    */ 




    /*                       OVERLOADING OF FRACTION - FLOAT
    ======================================================================
    implementation of the overloading of the - operator for Fraction - float
    ======================================================================
    */ 
    
    Fraction operator-(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo); // convert float to fraction
        return frac - floatFrac;
    }

    /*                       OVERLOADING OF FRACTION - FLOAT
    ======================================================================
    */ 




    /*                       OVERLOADING OF FLOAT - FRACTION
    ======================================================================
    implementation of the overloading of the - operator for float - Fraction
    ======================================================================
    */
     
    Fraction operator-(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac - frac;
    }

    /*                       OVERLOADING OF FLOAT - FRACTION
    ======================================================================
    */




    /*                       OVERLOADING OF /
    ======================================================================
    implementation of the overloading of the / operator
    ======================================================================
    */
    
    Fraction Fraction::operator/(const Fraction& frac) const{
        if (frac.getNumerator() == 0) {
            throw runtime_error("Division by zero");
        }
        
        Fraction result;
        // Check for overflow in numerator
        if (__builtin_mul_overflow(this->getNumerator(), frac.getDenominator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(this->getDenominator(), frac.getNumerator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    /*                       OVERLOADING OF /
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION / FLOAT
    ======================================================================
    implementation of the overloading of the / operator for Fraction / float
    ======================================================================
    */
     
    Fraction operator/(const Fraction& frac, const float& flo){
        if (flo == 0) {
            throw runtime_error("Division by zero");
        }
        
        Fraction floatFrac(flo); // convert float to fraction
        return frac/floatFrac;
    }

    /*                  OVERLOADING OF FRACTION / FLOAT
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT / FRACTION
    ======================================================================
    implementation of the overloading of the / operator for float / Fraction
    ======================================================================
    */
     
    Fraction operator/(const float& flo, const Fraction& frac){

        if (frac.getNumerator() == 0) {
            throw runtime_error("Division by zero");
        }

        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac/frac;
    }

    /*                  OVERLOADING OF FLOAT / FRACTION
    ======================================================================
    */




    /*                          OVERLOADING OF *
    ======================================================================
    implementation of the overloading of the * operator
    ======================================================================
    */
    
    Fraction Fraction::operator*(const Fraction& frac) const{
        Fraction result;
        
        // Check for overflow in numerator
        if (__builtin_mul_overflow(this->getNumerator(), frac.getNumerator(), &result.numerator)) {
            throw std::overflow_error("Overflow in numerator");
        }

        // Check for overflow in denominator
        if (__builtin_mul_overflow(this->getDenominator(), frac.getDenominator(), &result.denominator)) {
            throw std::overflow_error("Overflow in denominator");
        }

        int gcd = __gcd(result.getNumerator(), result.getDenominator());
        result.numerator /= gcd;
        result.denominator /= gcd;

        return result;
    }

    /*                          OVERLOADING OF *
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT * FRACTION
    ======================================================================
    implementation of the overloading of the * operator for float * fraction
    ======================================================================
    */
     
    Fraction operator*(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo); // convert float to fraction
        return floatFrac*frac;
    }

    /*                  OVERLOADING OF FLOAT * FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION * FLOAT
    ======================================================================
    implementation of the overloading of the * operator for fraction * float
    ======================================================================
    */
     
    Fraction operator*(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo); // convert float to fraction
        return frac*floatFrac;
    }

    /*                  OVERLOADING OF FRACTION * FLOAT
    ======================================================================
    */




    /*                  OVERLOADING OF ++ PREFIX
    ======================================================================
    implementation for the prefix increment operator
    ======================================================================
    */
     
    Fraction& Fraction::operator++(){
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer + theDenomi);
        return *this;
    }

    /*                  OVERLOADING OF ++ PREFIX
    ======================================================================
    */




    /*                  OVERLOADING OF ++ POSTFIX
    ======================================================================
    implementation for the postfix increment operator
    the postfix increment operator should return the original value
    before it was incremented, 
    not the new value after the increment.
    ======================================================================
    */
     
    Fraction Fraction::operator++(int){
        Fraction temp(*this); 
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer + theDenomi);
        return temp; 
    }

    /*                  OVERLOADING OF ++ POSTFIX
    ======================================================================
    */




    /*                  OVERLOADING OF -- PREFIX
    ======================================================================
    implementation for the pre-decrement operator
    ======================================================================
    */
     
    Fraction& Fraction::operator--(){
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer - theDenomi);
        return *this;
    }

    /*                  OVERLOADING OF -- PREFIX
    ======================================================================
    */




    /*                  OVERLOADING OF -- POSTFIX
    ======================================================================
    implementation for the post-decrement operator
    ======================================================================
    */
     
    Fraction Fraction::operator--(int){
        Fraction temp(*this); 
        int theNumer = this->getNumerator();
        int theDenomi = this->getDenominator();
        this->setNumer(theNumer - theDenomi);
        return temp; 
    }

    /*                  OVERLOADING OF -- POSTFIX
    ======================================================================
    */




    /*                  OVERLOADING OF >=
    ======================================================================
    implementation for the >= operator
    ======================================================================
    */
     
    bool Fraction::operator>=(const Fraction& frac) const {
        int num1 = this->getNumerator() * frac.getDenominator();
        int num2 = frac.getNumerator() * this->getDenominator();
        return num1 >= num2;
    }

    /*                  OVERLOADING OF >=
    ======================================================================
    */



    /*                  OVERLOADING OF FLOAT >= FRACTION
    ======================================================================
    implementation for the >= operator between float to fraction
    ======================================================================
    */
     
    bool operator>=(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac >= frac;
    }

    /*                  OVERLOADING OF FLOAT >= FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION >= FLOAT
    ======================================================================
    implementation for the >= operator between fraction to float
    ======================================================================
    */
    bool operator>=(const Fraction& frac, float& flo){
        Fraction floatFrac(flo);
        return frac >= floatFrac;
    }

    /*                  OVERLOADING OF FRACTION >= FLOAT
    ======================================================================
    */




    /*                  OVERLOADING OF <=
    ======================================================================
    implementation for overloading the <= operator
    ======================================================================
    */
     
    bool Fraction::operator<=(const Fraction& frac) const{
        int num1 =  this->getNumerator() * frac.getDenominator();
        int num2 = frac.getNumerator() * this->getDenominator();
        return num1 <= num2;
    }

    /*                  OVERLOADING OF <=
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT <= FRACTION
    ======================================================================
    implementation for overloading the <= operator between float to fraction
    ======================================================================
    */
     
    bool operator<=(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac <= frac;
    }

    /*                  OVERLOADING OF FLOAT <= FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION <= FLOAT
    ======================================================================
    implementation for overloading the <= operator between fraction to float
    ======================================================================
    */

    bool operator<=(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac <= floatFrac;
    }

    /*                  OVERLOADING OF FRACTION <= FLOAT
    ======================================================================
    */

   


    /*                  OVERLOADING OF >
    ======================================================================
    implementation for the > operator
    ======================================================================
    */
    
    bool Fraction::operator>(const Fraction& frac) const{

        int numer1 = this->getNumerator();
        int denomi1 = this->getDenominator();
        int gcd = __gcd(numer1, denomi1);
        Fraction frac1_reduced(numer1/gcd,denomi1/gcd);

        int numer2 = frac.getNumerator();
        int denomi2 = frac.getDenominator();
        int gcd2 = __gcd(numer2, denomi2);
        Fraction frac2_reduced(numer2/gcd2,denomi2/gcd2);


        int num1 = frac1_reduced.getNumerator() * frac2_reduced.getDenominator();
        int num2 = frac2_reduced.getNumerator() * frac1_reduced.getDenominator();
        
        return num1 > num2;
    }

    /*                  OVERLOADING OF >
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION > FLOAT
    ======================================================================
    implementation for the > operator between fraction to float
    ======================================================================
    */
     
    bool operator>(const Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac.getNumerator() * floatFrac.getDenominator() > floatFrac.getNumerator() * frac.getDenominator();
    }

    /*                  OVERLOADING OF FRACTION > FLOAT
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT > FRACTION
    ======================================================================
    implementation for the > operator between float to fraction
    ======================================================================
    */
     
    bool operator>(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac > frac;
    }

    /*                  OVERLOADING OF FLOAT > FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF <
    ======================================================================
    implementation for the < operator
    ======================================================================
    */
     
    bool Fraction::operator<(const Fraction& frac) const{
        int numer1 = this->getNumerator();
        int denomi1 = this->getDenominator();
        int gcd = __gcd(numer1, denomi1);
        Fraction frac1_reduced(numer1/gcd,denomi1/gcd);

        int numer2 = frac.getNumerator();
        int denomi2 = frac.getDenominator();
        int gcd2 = __gcd(numer2, denomi2);
        Fraction frac2_reduced(numer2/gcd2,denomi2/gcd2);


        int num1 = frac1_reduced.getNumerator() * frac2_reduced.getDenominator();
        int num2 = frac2_reduced.getNumerator() * frac1_reduced.getDenominator();
        
        return num1 < num2;
    }

    /*                  OVERLOADING OF <
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT < FRACTION
    ======================================================================
    implementation for the < operator 
    between float to fraction : float < fraction
    ======================================================================
    */
     
    bool operator<(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac < frac;
    }

    /*                  OVERLOADING OF FLOAT < FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION < FLOAT
    ======================================================================
    implementation for the < operator 
    between fraction to float : fraction < float
    ======================================================================
    */
     
    bool operator<(Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac < floatFrac;
    }

    /*                  OVERLOADING OF FRACTION < FLOAT
    ======================================================================
    */




    /*                  OVERLOADING OF ==
    ======================================================================
    implementation for the == operator
    ======================================================================
    */
    
    bool Fraction::operator==(const Fraction& frac) const{
        int numer1 = this->getNumerator() * frac.getDenominator();
        int numer2 = frac.getNumerator() * this->getDenominator();

        if (numer1 > numer2) return false;
        else if (numer1 < numer2) return false;
        else return true;
    }

    /*                  OVERLOADING OF ==
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT == FRACTION
    ======================================================================
    implementation for the == operator
    float == fraction
    ======================================================================
    */
    bool operator==(const float& flo, const Fraction& frac){
        Fraction floatFrac(flo);
        return floatFrac == frac;
    }

    /*                  OVERLOADING OF FLOAT == FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION == FLOAT
    ======================================================================
    implementation for the == operator
    fraction == float
    ======================================================================
    */
    bool operator==(Fraction& frac, const float& flo){
        Fraction floatFrac(flo);
        return frac == floatFrac;
    }

    /*                  OVERLOADING OF FRACTION == FLOAT
    ======================================================================
    */




    /*                          OVERLOADING OF !=
    ======================================================================
    implementation for the != operator
    using the == operator overloading
    *this is a pointer to the current object being operated on 
    by the member function.
    ======================================================================
    */

    bool Fraction::operator!=(const Fraction& frac) const{
        return !(*this == frac);
    }

    /*                          OVERLOADING OF !=
    ======================================================================
    */




    /*                  OVERLOADING OF FLOAT != FRACTION
    ======================================================================
    implementation for the != operator
    float != fraction
    using the == operator overloading
    ======================================================================
    */

    bool operator!=(const float& flo, const Fraction& frac){
        return !(flo == frac);
    }

    /*                  OVERLOADING OF FLOAT != FRACTION
    ======================================================================
    */




    /*                  OVERLOADING OF FRACTION != FLOAT
    ======================================================================
    implementation for the != operator
    fraction != float
    using the == operator overloading
    ======================================================================
    */

    bool operator!=(Fraction& frac, const float& flo){
        return !(frac == flo);
    }

    /*                  OVERLOADING OF FRACTION != FLOAT
    ======================================================================
    */



    /*                              GCD
    ======================================================================
    implementation to find the Greatest Common Divisor (GCD) 
    of two integers using the Euclidean algorithm
    ======================================================================
    */
     
    int Fraction::getGCD(int first, int second){
        while (second != 0){
            int temp = second;
            second = first % second;
            first = temp;
        }
        return first;
    }

    /*                              GCD
    ======================================================================
    */




    /*                              LCM
    ======================================================================
    implementation to find the Least Common Multiple (LCM) 
    of two integers using the gcd
    ======================================================================
    */

    int Fraction::getLCM(int first, int second){
        return (first * second) / getGCD(first, second);
    }

    /*                              LCM
    ======================================================================
    */
}
