#include "bcv.h"
#include "Timer.h"
#include "PapiTracer.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

#define BITS 63

#include <emmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>


void pshufb_test(long SIZE)
{
    __m128i input, mask;

    // 0, 1, 2, 3
    input = _mm_set_epi64((__m64) 0ll, (__m64) 100384ll);

    // now shuffle the bytes
    //v1 = 0..4, v2 = 5..9, v3 = 10..14, v4=15..19

    // two byte is the max copy for this element sie of 5 bit
    // 1,2 ->  0,1   clear 2 3 offset 0, mask 5
    // 1,2 ->  4,5   clear 6 7 offset 5, mask 5
    // 2,3 ->  8,9   clear 10,11 offset 2 bit, mask 5
    // 2,3 ->  12,13 clear 14,15 offset 7 bit, mask 5
    mask = _mm_set_epi8(
                        0x80, 0x80,
                        0x02, 0x01,
                        0x80, 0x80,
                        0x02, 0x01,
                        0x80, 0x80,
                        0x01, 0x00,
                        0x80, 0x80,
                        0x01, 0x00
                        );

    // Shuffle
    __m128i result = _mm_shuffle_epi8(input, mask);

    std::cout << (uint64_t) _mm_extract_epi64(result,0) << std::endl;
    std::cout << (uint64_t) _mm_extract_epi64(result,1) << std::endl;

    // Align by independent shifts
    __m128i mult_mask = _mm_set_epi32(1,32,4,128);
    __m128i mult = _mm_mullo_epi32(result, mult_mask);

    std::cout << (uint64_t) _mm_extract_epi64(mult,0) << std::endl;
    std::cout << (uint64_t) _mm_extract_epi64(mult,1) << std::endl;

    // Combined shift right
    __m128i shifter = _mm_srli_epi32(mult, 7);

    // And mask everything
    int a,b,c,d;
    a = _mm_extract_epi32(shifter, 0);
    b = _mm_extract_epi32(shifter, 1);
    c = _mm_extract_epi32(shifter, 2);
    d = _mm_extract_epi32(shifter, 3);

    std::cout << " " << (a & 31)
            << " " << (b & 31)
            << " " << (c & 31)
            << " " << (d & 31) << std::endl;
}


void test_set(long SIZE)
{
    std::cout << "[TEST ] set/get interleaved ..." << std::flush;
    BitCompressedVector<int, BITS> v(SIZE);
    for(size_t i=0; i < SIZE; ++i)
    {
        int a = i % (1UL << BITS);
        v.set(i, a);
        assert(a == v.get(i));
    }
    std::cout << " OK" << std::endl;
}

void test_get(long SIZE)
{
    std::cout << "[TEST ] set/get separated ..." << std::flush;
    BitCompressedVector<int, BITS> v(SIZE);
    for(size_t i=0; i < SIZE; ++i)
    {
        int a = i % (1UL << BITS);
        v.set(i, a);
    }

    for(size_t i=0; i < SIZE; ++i)
    {
        int a = i % (1UL << BITS);
        assert(a == v.get(i));
    }
    std::cout << " OK" << std::endl;
}

void test_mget(long SIZE)
{
    std::cout << "[TEST ] set/mget separated ..." << std::flush;
    long sum = 0, sum2 = 0;
    BitCompressedVector<int, BITS> v(SIZE);
    for(size_t i=0; i < SIZE; ++i)
    {
        int a = i % (1UL << BITS);
        v.set(i, a);
        sum += a;
    }

    size_t alloca = BitCompressedVector<int, BITS>::mget_array_size;
    int *tmp = (int*) malloc(sizeof(int) * alloca);

    for(size_t i=0; i < SIZE; )
    {
        size_t actual;
        v.mget(i, (int*) tmp, &actual);
        for(size_t j=0; j < actual; ++j, ++i)
        {
            int a = i % (1UL << BITS);
            sum2 += tmp[j];

            assert(a == tmp[j]);
        }



    }
    free(tmp);
    assert(sum == sum2);
    std::cout << " OK" << std::endl;
}

template<class C>
void fill(C& v, size_t size)
{
    for(size_t i=0; i < size; ++i)
        v[i] = i % (1UL << BITS);
}

void performance(size_t size)
{
    BitCompressedVector<int, BITS> v(size);
    std::vector<int> v2(size);

    fill(v, size);
    fill(v2, size);

    double a,b,c,d,e;

    Timer t;
    long long res = 0;

    ///////////////////////////////////////////////////////////////////////////
    t.start();
    for(size_t i=0; i < size; i+=1)
    {
        //printf("%d\n", v.get(i));
        res += v.get(i);
    }
    t.stop();
    std::cout << res << " get time " << (a = t.elapsed_time()) << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    res = 0;
    t.start();
    for(size_t i=0; i < size; i+=1)
    {
        res += v[i];
    }
    t.stop();
    std::cout << res << " get[] time " << (b = t.elapsed_time()) << std::endl;


    ///////////////////////////////////////////////////////////////////////////
    res = 0;
    size_t alloca = BitCompressedVector<int, BITS>::mget_array_size;;
    int *tmp = (int*) malloc(sizeof(int) * alloca);

    size_t actual;
    t.start();
    //int flags = PapiTracer::start();
    for(size_t i=0; i < size; )
    {
        v.mget(i, tmp, &actual);
        for(size_t j=0; j < actual; ++j)
        {
            //printf("%d\n", tmp[j]);
            res += tmp[j];
        }

        i += actual;

    }
    //PapiTracer::result_t papi = PapiTracer::stop(flags);
    t.stop();
    std::cout << res << " mget time " << (c = t.elapsed_time()) << std::endl;
    //std::cout << papi.first << " " << papi.second << std::endl;
    free(tmp);

    ///////////////////////////////////////////////////////////////////////////
    res = 0;
    t.start();
    for(size_t i=0; i < size; i+=16)
    {
        res += v2[i];
        res += v2[i+1];
        res += v2[i+2];
        res += v2[i+3];
        res += v2[i+4];
        res += v2[i+5];
        res += v2[i+6];
        res += v2[i+7];
        res += v2[i+8];
        res += v2[i+9];
        res += v2[i+10];
        res += v2[i+11];
        res += v2[i+12];
        res += v2[i+13];
        res += v2[i+14];
        res += v2[i+15];
    }
    t.stop();
    std::cout << res << " vector time " << (e = t.elapsed_time()) << std::endl;
}


int main(int argc, char* argv[])
{
    // Setting size
    long SIZE = atol(argv[1]);

    //pshufb_test(SIZE);

    test_set(SIZE);
    test_get(SIZE);
    test_mget(SIZE);
    performance(SIZE);

	return 0;
}