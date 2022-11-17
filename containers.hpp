#pragma once
#include <type_traits>

namespace Assert
{
    template<class ...Ts> struct void_t { using type = void; }; 

    
    template<typename T>
    struct IsContainer{
        template<typename U, class = void> struct HasBegin { static const bool value = false; };
        template<typename U> struct HasBegin<U, typename void_t<decltype(std::declval<T>().begin())>::type> { static const bool value = true; };
        
        template<typename U, class = void> struct HasEnd { static const bool value = false; };
        template<typename U> struct HasEnd<U, typename void_t<decltype(std::declval<T>().end())>::type> { static const bool value = true; };
        
        static const bool value = HasBegin<T>::value && HasBegin<T>::value;
        static_assert(value, "Type must begin and end methods");
    };

}





// A method for checking if a class has a certain static member function
// template<class ...Ts> struct void_t { using type = void; }; 
// template<typename T>
// class AdditiveIdenityExists{
//     template<typename U, class = void> struct AdditiveIdenityExists_impl { static const bool value = false; };
//     template<typename U> struct AdditiveIdenityExists_impl<U, typename void_t<decltype(U::Zero())>::type> { static const bool value = true; };
//     static_assert(AdditiveIdenityExists_impl<T>::value, "Type must have an additive idenity represented with a static Zero method");
// };