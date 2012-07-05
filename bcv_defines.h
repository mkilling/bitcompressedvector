#ifndef BCV_DEFINES_H
#define BCV_DEFINES_H

#define CACHE_LINE_SIZE 64

template<unsigned char B> struct suitable_vector_size { enum { value = 64 }; };
template<> struct suitable_vector_size<1> { enum { value = 8 }; };
template<> struct suitable_vector_size<2> { enum { value = 8 }; };
template<> struct suitable_vector_size<3> { enum { value = 8 }; };
template<> struct suitable_vector_size<4> { enum { value = 8 }; };
template<> struct suitable_vector_size<5> { enum { value = 8 }; };
template<> struct suitable_vector_size<6> { enum { value = 8 }; };
template<> struct suitable_vector_size<7> { enum { value = 8 }; };
template<> struct suitable_vector_size<8> { enum { value = 8 }; };
template<> struct suitable_vector_size<9> { enum { value = 16 }; };
template<> struct suitable_vector_size<10> { enum { value = 16 }; };
template<> struct suitable_vector_size<11> { enum { value = 16 }; };
template<> struct suitable_vector_size<12> { enum { value = 16 }; };
template<> struct suitable_vector_size<13> { enum { value = 16 }; };
template<> struct suitable_vector_size<14> { enum { value = 16 }; };
template<> struct suitable_vector_size<15> { enum { value = 16 }; };
template<> struct suitable_vector_size<16> { enum { value = 16 }; };
template<> struct suitable_vector_size<17> { enum { value = 32 }; };
template<> struct suitable_vector_size<18> { enum { value = 32 }; };
template<> struct suitable_vector_size<19> { enum { value = 32 }; };
template<> struct suitable_vector_size<20> { enum { value = 32 }; };
template<> struct suitable_vector_size<21> { enum { value = 32 }; };
template<> struct suitable_vector_size<22> { enum { value = 32 }; };
template<> struct suitable_vector_size<23> { enum { value = 32 }; };
template<> struct suitable_vector_size<24> { enum { value = 32 }; };
template<> struct suitable_vector_size<25> { enum { value = 32 }; };
template<> struct suitable_vector_size<26> { enum { value = 32 }; };
template<> struct suitable_vector_size<27> { enum { value = 32 }; };
template<> struct suitable_vector_size<28> { enum { value = 32 }; };
template<> struct suitable_vector_size<29> { enum { value = 32 }; };
template<> struct suitable_vector_size<30> { enum { value = 32 }; };
template<> struct suitable_vector_size<31> { enum { value = 32 }; };
template<> struct suitable_vector_size<32> { enum { value = 32 }; };

#endif // BCV_DEFINES_H