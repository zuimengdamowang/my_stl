//
// Created by boyuan on 2022/4/28.
//

#ifndef TINYSTL_UTILITY_H
#define TINYSTL_UTILITY_H



namespace tt {


    template<class T>
    void swap(T& a, T& b){
        T temp = a;
        a = b;
        b = temp;
    }





}  // namespace tt







#endif //TINYSTL_UTILITY_H
