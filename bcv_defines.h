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

#define BUILD_MASK_HEADER \
        const uint64_t global_bit_masks[65] = { \
        0 /*CreateMask<0>::mask()*/, \
        CreateMask<1>::mask(), \
        CreateMask<2>::mask(), \
        CreateMask<3>::mask(), \
        CreateMask<4>::mask(), \
        CreateMask<5>::mask(), \
        CreateMask<6>::mask(), \
        CreateMask<7>::mask(), \
        CreateMask<8>::mask(), \
        CreateMask<9>::mask(), \
        CreateMask<10>::mask(), \
        CreateMask<11>::mask(), \
        CreateMask<12>::mask(), \
        CreateMask<13>::mask(), \
        CreateMask<14>::mask(), \
        CreateMask<15>::mask(), \
        CreateMask<16>::mask(), \
        CreateMask<17>::mask(), \
        CreateMask<18>::mask(), \
        CreateMask<19>::mask(), \
        CreateMask<20>::mask(), \
        CreateMask<21>::mask(), \
        CreateMask<22>::mask(), \
        CreateMask<23>::mask(), \
        CreateMask<24>::mask(), \
        CreateMask<25>::mask(), \
        CreateMask<26>::mask(), \
        CreateMask<27>::mask(), \
        CreateMask<28>::mask(), \
        CreateMask<29>::mask(), \
        CreateMask<30>::mask(), \
        CreateMask<31>::mask(), \
        CreateMask<32>::mask(), \
        CreateMask<33>::mask(), \
        CreateMask<34>::mask(), \
        CreateMask<35>::mask(), \
        CreateMask<36>::mask(), \
        CreateMask<37>::mask(), \
        CreateMask<38>::mask(), \
        CreateMask<39>::mask(), \
        CreateMask<40>::mask(), \
        CreateMask<41>::mask(), \
        CreateMask<42>::mask(), \
        CreateMask<43>::mask(), \
        CreateMask<44>::mask(), \
        CreateMask<45>::mask(), \
        CreateMask<46>::mask(), \
        CreateMask<47>::mask(), \
        CreateMask<48>::mask(), \
        CreateMask<49>::mask(), \
        CreateMask<50>::mask(), \
        CreateMask<51>::mask(), \
        CreateMask<52>::mask(), \
        CreateMask<53>::mask(), \
        CreateMask<54>::mask(), \
        CreateMask<55>::mask(), \
        CreateMask<56>::mask(), \
        CreateMask<57>::mask(), \
        CreateMask<58>::mask(), \
        CreateMask<59>::mask(), \
        CreateMask<60>::mask(), \
        CreateMask<61>::mask(), \
        CreateMask<62>::mask(), \
        CreateMask<63>::mask(), \
        CreateMask<64>::mask()}; \

#endif // BCV_DEFINES_H