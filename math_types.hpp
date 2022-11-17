#pragma once
#include <type_traits>

namespace Types{

    template<typename T, typename std::enable_if<std::is_fundamental<T>::value, int>::type = 0>
    T Zero() { return 0; }
    
    template<typename T, typename std::enable_if<!std::is_fundamental<T>::value, int>::type = 0>
    T Zero() = delete;
    
    template<typename T, typename std::enable_if<std::is_fundamental<T>::value, int>::type = 0>
    T One() { return 1; }
    
    template<typename T, typename std::enable_if<!std::is_fundamental<T>::value, int>::type = 0>
    T One() = delete;

}


namespace Assert {
    using namespace Types;

    template<typename ...Ts> struct void_t { using type = void; }; 
    struct None {}; 

    template<typename T, typename Arg> None operator== (const T&, const Arg&);
    template<typename T, typename Arg> None operator!= (const T&, const Arg&);
    template<typename T, typename Arg> None operator< (const T&, const Arg&);
    template<typename T, typename Arg> None operator+ (const T&, const Arg&);
    template<typename T, typename Arg> None operator- (const T&, const Arg&);
    template<typename T, typename Arg> None operator* (const T&, const Arg&);
    template<typename T, typename Arg> None operator/ (const T&, const Arg&);
    template<typename T> None operator- (const T&);
    
    // Has operator defined
    #define OPERATOR_EXISTS(NAME, OP) \
        template<typename T, typename Arg = T> \
        struct NAME { \
                static const bool value = !std::is_same<decltype(std::declval<T>() OP std::declval<Arg>()), None>::value; \
                static_assert(value, "Operator " #OP " must exist."); \
        }
    
    OPERATOR_EXISTS(NotEqualsExists, !=);
    OPERATOR_EXISTS(SubtractExists, -);
    OPERATOR_EXISTS(MultiplyExists, *);
    OPERATOR_EXISTS(EqualsExists, ==);
    OPERATOR_EXISTS(DivideExists, /);
    OPERATOR_EXISTS(AddExists, +);
    OPERATOR_EXISTS(Orderable, <);
    
    #undef OPERATOR_EXISTS



    // Has Attribute CommutativeOverAddition set to true
    template<typename T>
    struct IsCommutativeOverAddition {
        template<typename U, class = void> struct HasAttribute { static const bool value = std::is_fundamental<U>::value; };
        template<typename U> struct HasAttribute<U, typename void_t<decltype(U::CommutativeOverAddition)>::type> { static const bool value = (bool)U::CommutativeOverAddition; };
        static const bool value = HasAttribute<T>::value;
        static_assert(value, "Type must have a static property stating that CommutativeOverAddition is true");
    };

    // Has Attribute CommutativeOverMultiplication set to true
    template<typename T>
    struct IsCommutativeOverMultiplication {
        template<typename U, class = void> struct HasAttribute { static const bool value = std::is_fundamental<U>::value; };
        template<typename U> struct HasAttribute<U, typename void_t<decltype(U::CommutativeOverMultiplication)>::type> { static const bool value = (bool)U::CommutativeOverMultiplication; };
        static const bool value = HasAttribute<T>::value;
        static_assert(value, "Type must have a static property stating that CommutativeOverMultiplication is true");
    };

    // Has Attribute IsAssociativeOverAddition set to true
    template<typename T>
    struct IsAssociativeOverAddition {
        template<typename U, class = void> struct HasAttribute { static const bool value = std::is_fundamental<U>::value; };
        template<typename U> struct HasAttribute<U, typename void_t<decltype(U::AssociativeOverAddition)>::type> { static const bool value = (bool)U::AssociativeOverAddition; };
        static const bool value = HasAttribute<T>::value;
        static_assert(value, "Type must have a static property stating that AssociativeOverAddition is true");
    };

    // Has Attribute IsAssociativeOverMultiplication set to true
    template<typename T>
    struct IsAssociativeOverMultiplication {
        template<typename U, class = void> struct HasAttribute { static const bool value = std::is_fundamental<U>::value; };
        template<typename U> struct HasAttribute<U, typename void_t<decltype(U::AssociativeOverMultiplication)>::type> { static const bool value = (bool)U::AssociativeOverMultiplication; };
        static const bool value = HasAttribute<T>::value;
        static_assert(value, "Type must have a static property stating that IsAssociativeOverMultiplication is true");
    };

    // Has Attribute IsAssociativeOverMultiplication set to true
    template<typename T>
    struct IsDistributive {
        template<typename U, class = void> struct HasAttribute { static const bool value = std::is_fundamental<U>::value; };
        template<typename U> struct HasAttribute<U, typename void_t<decltype(U::Distributive)>::type> { static const bool value = (bool)U::Distributive; };
        static const bool value = HasAttribute<T>::value;
        static_assert(value, "Type must have a static property stating that IsAssociativeOverMultiplication is true");
    };
    
    // uniary operator exitsts
    template<typename T>
    struct NegateExists { 
        static const bool value = !std::is_same<decltype(-std::declval<T>()), None>::value;
        static_assert(value, "Operator uniary - must exist.");
    };

    // Has Zero<T> specilaized
    template<typename T>
    struct AdditiveIdenityExists{
        template<typename U, typename = void> struct AdditiveIdenityExists_impl { static const bool value = false; };
        template<typename U> struct AdditiveIdenityExists_impl<U, typename void_t<decltype(Zero<U>())>::type > { static const bool value = true; };
        static const bool value = AdditiveIdenityExists_impl<T>::value; 
        static_assert(value, "Type must specialize Types::Zero()");
    };
    
    // Has One<T> specilaized
    template<typename T>
    struct MultiplicativeIdenityExists{
        template<typename U, typename = void> struct MultiplicativeIdenityExists_impl { static const bool value = false; };
        template<typename U> struct MultiplicativeIdenityExists_impl<U, typename void_t<decltype(One<U>())>::type > { static const bool value = true; };
        static const bool value = MultiplicativeIdenityExists_impl<T>::value;
        static_assert(value, "Type must specialize Types::One()");
    };

    template<typename T>
    struct SemiGroupOverAddition {
        static const bool value = AddExists<T>::value && IsAssociativeOverAddition<T>::value;
        static_assert(value, "Type must have + operator defined");
    };

    template<typename T>
    struct SemiGroupOverMultiplication {
        static const bool value = MultiplyExists<T>::value && IsAssociativeOverMultiplication<T>::value;
        static_assert(value, "Type must have * operator defined");
    };

    template<typename T>
    struct IsMonoidOverAddition { 
        static const bool value = SemiGroupOverAddition<T>::value && AdditiveIdenityExists<T>::value;
        static_assert(value, "Type must have + and Zero<T> defined");
    };

    template<typename T>
    struct IsMonoidOverMultilication {
        static const bool value = SemiGroupOverMultiplication<T>::value && MultiplicativeIdenityExists<T>::value;
        static_assert(value, "Type must have * and One<T> defined");
    };

    template<typename T>
    struct IsGroupOverAddition {
        static const bool value = IsMonoidOverAddition<T>::value && SubtractExists<T>::value;
        static_assert(value, "Type must be a monoid, and have a - operator");
    };

    template<typename T>
    struct IsGroupOverMultiplication {
        static const bool value = IsMonoidOverMultilication<T>::value && DivideExists<T>::value;
        static_assert(value, "Type must be a monoid, and have a / operator");
    };

    template<typename T>
    struct CommutativeGroupOverAddition {
        static const bool value = IsGroupOverAddition<T>::value && IsCommutativeOverAddition<T>::value;
        static_assert(value, "Type must be a group and also be commutative");
    };

    template<typename T>
    struct CommutativeGroupOverMultiplication {
        static const bool value = IsGroupOverMultiplication<T>::value && IsCommutativeOverMultiplication<T>::value;
        static_assert(value, "Type must be a group and also be commutative");
    };
 
    template<typename T>
    struct Ring {
        static const bool value = CommutativeGroupOverAddition<T>::value && SemiGroupOverMultiplication<T>::value && IsDistributive<T>::value;
        static_assert(value, "should be a ring");
    };

    template<typename T>
    struct UnitalRing {
        static const bool value = Ring<T>::value && MultiplicativeIdenityExists<T>::value;
        static_assert(value, "should be a ring with a multiplicative identity");
    };

    template<typename T>
    struct CommutativeRing {
        static const bool value = UnitalRing<T>::value && IsCommutativeOverMultiplication<T>::value;
        static_assert(value, "should be a unital ring with a commutative multiplication");
    };


    //TODO: Implement conversions between vectors, also implement addition and scalar multiplication



    // Vectorspace
    // closed vector addition and scalar multiplication
    // the axioms hold 
    // T is a vectorspace over U iff 
    // T is a commutative group over addition
    // U * T is defined and is a commutative, associative operation with an identity element

    //TODO: check for associativity and commutativity, and also for the distributive property
    template<typename Vec, typename Scalar>
    struct IsVectorSpace {
        static const bool value = 
            CommutativeGroupOverAddition<Vec>::value
            && MultiplyExists<Scalar, Vec>::value
            && MultiplicativeIdenityExists<Scalar>::value;
        static_assert(value, "Vec over Scalar must satisfy all the axioms of a vectorspace!");
    };

}



// namespace Types {
//     using  i8 = int8_t;
//     using i16 = int16_t;
//     using i32 = int32_t;
//     using i64 = int64_t;

//     using  u8 = uint8_t;
//     using u16 = uint16_t;
//     using u32 = uint32_t;
//     using u64 = uint64_t;

//     using f32 = float;
//     using f64 = double;
    
//     template<typename T>
//     struct v2 {
//         union{
//             struct{
//                 T x;
//                 T y;
//             };
//             T v[2];
//         };
//     };


//     template<typename T>
//     struct v3 {
//         union{
//             struct{
//                 union {T x; T r};
//                 union {T y; T g};
//                 union {T z; T b};
//             };
//             T v[3];
//         };
//     };

//     template<typename T>
//     struct v4 {
//         union{
//             struct{
//                 union {T x; T r};
//                 union {T y; T g};
//                 union {T z; T b};
//                 union {T w; T a};
//             };
//             T v[4];
//         };
//     };
// }








// A method for checking if a class has a certain static member function
// template<class ...Ts> struct void_t { using type = void; }; 
// template<typename T>
// class AdditiveIdenityExists{
//     template<typename U, class = void> struct AdditiveIdenityExists_impl { static const bool value = false; };
//     template<typename U> struct AdditiveIdenityExists_impl<U, typename void_t<decltype(U::Zero())>::type> { static const bool value = true; };
//     static_assert(AdditiveIdenityExists_impl<T>::value, "Type must have an additive idenity represented with a static Zero method");
// };