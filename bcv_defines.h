#ifndef BCV_DEFINES_H
#define BCV_DEFINES_H

#define CACHE_LINE_SIZE 64

template<unsigned char B> struct data_size { enum { value = 64 }; };
template<> struct data_size<1> { enum { value = 8 }; };
template<> struct data_size<2> { enum { value = 8 }; };
template<> struct data_size<3> { enum { value = 8 }; };
template<> struct data_size<4> { enum { value = 8 }; };
template<> struct data_size<5> { enum { value = 8 }; };
template<> struct data_size<6> { enum { value = 8 }; };
template<> struct data_size<7> { enum { value = 8 }; };
template<> struct data_size<8> { enum { value = 8 }; };
template<> struct data_size<9> { enum { value = 16 }; };
template<> struct data_size<10> { enum { value = 16 }; };
template<> struct data_size<11> { enum { value = 16 }; };
template<> struct data_size<12> { enum { value = 16 }; };
template<> struct data_size<13> { enum { value = 16 }; };
template<> struct data_size<14> { enum { value = 16 }; };
template<> struct data_size<15> { enum { value = 16 }; };
template<> struct data_size<16> { enum { value = 16 }; };
template<> struct data_size<17> { enum { value = 32 }; };
template<> struct data_size<18> { enum { value = 32 }; };
template<> struct data_size<19> { enum { value = 32 }; };
template<> struct data_size<20> { enum { value = 32 }; };
template<> struct data_size<21> { enum { value = 32 }; };
template<> struct data_size<22> { enum { value = 32 }; };
template<> struct data_size<23> { enum { value = 32 }; };
template<> struct data_size<24> { enum { value = 32 }; };
template<> struct data_size<25> { enum { value = 32 }; };
template<> struct data_size<26> { enum { value = 32 }; };
template<> struct data_size<27> { enum { value = 32 }; };
template<> struct data_size<28> { enum { value = 32 }; };
template<> struct data_size<29> { enum { value = 32 }; };
template<> struct data_size<30> { enum { value = 32 }; };
template<> struct data_size<31> { enum { value = 32 }; };
template<> struct data_size<32> { enum { value = 32 }; };

#endif // BCV_DEFINES_H