/* *************************************************
 File: hw3.cpp
 Author: Cheechee Lin
 Date: October 29, 2015
 
Template functions, template classes and specialized templates.
 
 ************************************************* */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>

#ifndef MY_VECTOR_H
#define MY_VECTOR_H

//forward declaration of template class MyVector<>
template <typename T>
class MyVector;

//non-member function declaration: <<
template <typename T>
std::ostream& operator<<( std::ostream& out, const MyVector<T>& v );



// Interface
template <typename T>
class MyVector {
public:
    // 1. Default constructor
    MyVector();
    
    // 2. Destructor
    ~MyVector();
    
    // 3. Copy constructor
    MyVector( const MyVector& b );
    
    // 4. Operator=. The return is by reference to allow for chaining.
    MyVector& operator=( const MyVector& b );
    
    // Other member functions
    void push_back( T newValue );
    int size() const;
    
    // 5. These two functions are replaced by the [] operator
    // int position(int pos) const;
    // void set(int pos, int newValue);
    T& operator[](int n);
    T operator[](int n) const;

    
    // 7. Operator Overloading:
    //    operator+, operator+=,
    //    operator* as a member function to compute 'dot products'
    MyVector operator+( const MyVector<T>& b ) const ;
    MyVector& operator+=( const MyVector<T>& b );
    T operator*( const MyVector<T>& b ) const ;
    
    // 8. Relational operators >, >= , ==, !=, < and <=
    //    The norms of the vectors are used in <, <=, < and >=
    bool operator<=( const MyVector& b) const;
    bool operator>=( const MyVector& b) const;
    bool operator<( const MyVector& b) const;
    bool operator>( const MyVector& b) const;
    bool operator==( const MyVector& b) const;
    bool operator!=( const MyVector& b) const;
    
    
private:
    void reserve(int newCapacity);
    int compare( const MyVector<T>& b ) const;
    
    static const int INITIAL_SIZE=10;
    int currentCapacity;
    int numItems;
    T* theData;
};

#endif



// 1. Default constructor
template <typename T>
MyVector<T>::MyVector(){
    std::cout << "Standard constructor called..." << std::endl;
    currentCapacity = INITIAL_SIZE;
    numItems = 0;
    theData = new T[INITIAL_SIZE];
}

// 2. Destructor
template <typename T>
MyVector<T>::~MyVector(){
    std::cout << "Destructor called. Relasing memory..." << std::endl;
    delete[] theData;
    theData = 0 ;  // <-- prevents a compiler warning but less readable
    //theData = NULL ;
}

// 3. Copy constructor
template <typename T>
MyVector<T>::MyVector( const MyVector& b ){
    std::cout << "Copy constructor called..." << std::endl;
    // shallow copy non-pointers
    currentCapacity = b.currentCapacity;
    numItems = b.numItems;
    
    // deep copy pointers (if necessary)
    if ( b.theData ){  // same as if ( b.theData != NULL )
        theData = new T[b.currentCapacity];
        
        for (int i=0 ; i < b.numItems ; i++ )
            theData[i] = b.theData[i];
        
    }
    else
        theData = NULL;
    
}

// 4. Operator=
template <typename T>
MyVector<T>& MyVector<T>::operator=( const MyVector& b ){
    std::cout << "operator= called..." << std::endl;
    // check for self-assignment
    if ( this == &b )
        return *this;
    
    // de-allocate memory in current object
    delete[] this->theData;
    
    // shallow copy non-pointers
    currentCapacity = b.currentCapacity;
    numItems = b.numItems;
    
    // deep copy pointers (if necessary)
    if ( b.theData ){  // same as if ( b.theData != NULL )
        theData = new T[b.currentCapacity];
        
        for (int i=0 ; i < b.numItems ; i++ )
            theData[i] = b.theData[i];
        
    }
    else
        theData = NULL;
    
    return *this;
}


// **********************************************
// Implementation of member functions
// **********************************************

template <typename T>
int MyVector<T>::size() const {
    return numItems;
}

/*
 int MyVector::position(int pos) const {
 return theData[pos];
 }
 
 void MyVector::set(int pos, int newValue){
 theData[pos] = newValue;
 return;
 }
 */

template <typename T>
void MyVector<T>::push_back( T newValue ){
    // make sure there is enough space
    if ( numItems == currentCapacity )
        reserve( currentCapacity + 1 ); //
    
    theData[numItems] = newValue; // same as *(theData + numItems) = newValue
    numItems++;
    
    return;
}

template <typename T>
void MyVector<T>::reserve( int newCapacity ){
    if ( newCapacity > currentCapacity ){
        
        // Make sure we at least double the current capacity
        if ( newCapacity > 2*currentCapacity )
            currentCapacity = newCapacity;
        else
            currentCapacity *= 2;
        
        // request more space in the heap
        T* newData = new T[currentCapacity];
        
        //copy the data
        for ( int i=0 ; i < numItems ; i++ )
            newData[i] = theData[i]; // same as *(newData + i) = *(theData + i)
        
        // free memory from old location. The pointer survives
        delete[] theData;
        
        // update memory address
        theData = newData;
    }
    
    return;
}

// 5. These two functions are replaced by the [] operator
template <typename T>
T& MyVector<T>::operator[](int n){
    // if n is not a valid index do something
    return *(theData + n); // <-- same as theData[n]
}

template <typename T>
T MyVector<T>::operator[](int n) const {
    // if n is not a valid index do something
    return *(theData + n);
}

// 7. Exercise: Lesson 3, slide 20)
//    Overload operator+ and operator+=
template <typename T>
MyVector<T> MyVector<T>::operator+( const MyVector<T>& b ) const {
    MyVector c(b);           // We have a vector with the right size;
    for (int i=0; i<c.size() ; i++ )
        c[i] = theData[i] + b[i];
    
    return c;
}

// Implementaion based on operator= to avoid repetition of code
template <typename T>
MyVector<T>& MyVector<T>::operator+=( const MyVector<T>& b ){
    *this = *this + b;
    
    return *this;
}

// This is a product between two vectors and is a member function.
template <typename T>
T MyVector<T>::operator*( const MyVector<T>& b) const {
    // Assume *this and b have the same size.
    int sum = 0;
    for (int i=0 ; i<b.size() ; i++)
        sum += theData[i] * b[i];
    
    return sum;
}



/**************************************************
 Relational Operators
 
 To avoid repetition, the operators <, <=, > and >=
 are based on a private member function that
 returns an integer.
 The idea is to use the following equivalent
 statements:
 
 x > y  if and only if  (x - y) > 0
 x >= y if and only if  (x - y) >= 0
 x < y  if and only if  (x - y) < 0
 x <= y if and only if  (x - y) <= 0
 
 Implement a private member function 'compare' that
 returns:
 - a positive integer if implicit parameter is bigger than b
 - a nonnegative integer if implicit parameter is bigger or equal to b
 - a negatove integer if implicit parameter is less than b
 - a nonpositive integer if implicit parameter is less or equal to b
 ************************************************* */
template <typename T>
int MyVector<T>::compare( const MyVector<T>& b )const{
    return std::sqrt( (*this) * (*this) - b * b ) ;
    /* return (*this) * (*this) - b * b; // <-- also works. Why? */
}


template <typename T>
bool MyVector<T>::operator>( const MyVector<T>& b ) const {
    return compare(b) > 0 ; // <-- same as return this->compare(b) > 0; or *(this).compare(b);
}

//for string case
template<>
bool MyVector<std::string>::operator>( const MyVector<std::string>& b ) const {
    for (int i=0; i<b.size(); i++){
        return(theData[i]>b[i]);
    }
    return false;
}

template <typename T>
bool MyVector<T>::operator>=( const MyVector<T>& b ) const {
    return compare(b) >= 0 ;
}

//for string case
template<>
bool MyVector<std::string>::operator>=(const MyVector<std::string>& b) const{
    for (int i=0; i<b.size(); i++){
        if(theData[i]>b[i]) return true;
        if (theData[i]==b[i]) return true;
    }
    return false;
}

template <typename T>
bool MyVector<T>::operator<( const MyVector<T>& b ) const {
    return compare(b) < 0;
}

//for string case
template<>
bool MyVector<std::string>::operator<( const MyVector<std::string>& b ) const {
    for (int i=0; i<b.size(); i++){
        if(theData[i]<b[i])
            return true;
    }
    return false;
}

template <typename T>
bool MyVector<T>::operator<=( const MyVector<T>& b ) const {
    return compare(b) <= 0;
}

//for string case
template<>
bool MyVector<std::string>::operator<=(const MyVector<std::string>& b) const{
    for (int i=0; i<b.size(); i++){
        if(theData[i]<b[i]) return true;
        if (theData[i]==b[i]) return true;
    }
    return false;
}

template <typename T>
bool MyVector<T>::operator==( const MyVector<T>& b ) const {
    // False if different sizes
    if( size() != b.size() )
        return false;
    else{
        // False if at least one entry does not match
        for (int i=0 ; i<size() ; i++){
            if ( theData[i] != b[i] )
                return false;
        }
        // True if all entries match
        return true;
    }
}

//== for string case
template<>
bool MyVector<std::string>::operator==(const MyVector<std::string>& b) const{
    if ( size()!=b.size()) return false;
    else{
        for (int i=0; i<size(); i++){
            if(theData[i]!=b[i])
                return false;
        }
        return true;
    }
}

template <typename T>
bool MyVector<T>::operator!=( const MyVector<T>& b ) const {
    return !( *this == b );
}

//!= for string case
template<>
bool MyVector<std::string>::operator!=(const MyVector<std::string>&b) const{
    return!(*this==b);
}


/**************************************************
 NON MEMBER OPERATORS
 ************************************************* */
template <typename T>
MyVector<T> operator*( int a, MyVector<T> v){
    for ( int i=0 ; i < v.size() ; i++ ){
        v[i] = a * v[i];
    }
    return v; // v is a local copy of the vector
}

//operator * for string and MyVector<string>
MyVector<std::string> operator*(std::string a, MyVector<std::string> b){
    for (int i=0 ; i<b.size() ; i++)
        b[i]= a+ b[i];
    return b; // b is a local copy of the vector
}

//operator * for MyVector<string> and string
MyVector<std::string> operator*(MyVector<std::string> b, std::string a){
    for (int i=0 ; i<b.size() ; i++)
        b[i]= b[i]+ a;
    return b; // b is a local copy of the vector
}

template <typename T>
MyVector<T> operator*( MyVector<T> v , T a){
    return a * v;
}


//operator * for two objects of type MyVector<string>
template<>
std::string MyVector<std::string>::operator*(const MyVector<std::string>& b)const{
    std::string temp=" ";
    for (int i=0; i<b.size(); i++){
        temp+=theData[i]+b[i]+"+";
    }
    temp.pop_back();
    return temp;
}

//sqrt function for strings
std::string sqrt(std::string s){
    std::string temp= "sqrt( " +s+ " )";
    return temp;
}

// Output operator <<
template <typename T>
std::ostream& operator<<( std::ostream& out, const MyVector<T>& v ){
    for ( int i=0 ; i < v.size() ; i++ )
        out << std::setw(8) << v[i] ;
    out << std::endl;
    
    return out;
}

//Output operator << for string cases (output the elements of the vector separated by a blank space followed by an end of line character '\n'
std::ostream&operator<<(std::ostream& out, const MyVector<std::string>& v){
    for (int i=0; i<v.size(); i++)
        out<<v[i]<<" ";
    out<<std::endl;
    
    return out;
}