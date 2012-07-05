#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "bcv_defines.h"
#include "mask.h"

#include <stdint.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

// SSE requirements
#include <emmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>


#ifndef NDEBUG

#define DEBUG(msg) std::cout << msg << std::endl;
#define DEBUG_M128(m) std::cout << (uint64_t) _mm_extract_epi64(m, 0) << "  " << (uint64_t) _mm_extract_epi64(m, 0) << std::endl;

#else

#define DEBUG(msg)

#endif



BUILD_MASK_HEADER;
/*

 This class provides a numeric bit compressed vector.
 Basically it provides a drop-in replacement for the standard std::vector. However,
 the number of bits allocated per value cannot be changed afterwards.


*/
template<typename T, uint8_t B>
class BitCompressedVector
{

public:

    typedef T   value_type;
    typedef T&  value_type_ref;
    typedef T*  value_type_ptr;


    /*
    * Constructor
    */
    BitCompressedVector(size_t size) : _reserved(size)
    {
        _allocated_blocks = (size * B) / (sizeof(data_t) * 8) + 1;
        posix_memalign((void**)&_data, 128, _allocated_blocks * sizeof(data_t));
        //printf("allocated: %u\n", _data);
        for (int i = 0; i < _allocated_blocks; ++i)
            _data[i] = _mm_setzero_si128();
        //memset(_data, 0, _allocated_blocks * sizeof(data_t));


    }

    ~BitCompressedVector()
    {
        free(_data);
    }

    /*
     *  Original get method based on the index
     */
    inline value_type get(const size_t index) const;


    /*

    This method returns a list of extracted values from the vector.The number
    of elements is variadic end depends on the number of elements inside a
    single block.

    Typicallay we try to extract at least a single cache line

     */
    inline void mget(const size_t index, value_type_ptr data, size_t *actual) const;

    /*
     *  Set method to set a value
     */
    inline void set(const size_t index, const value_type v);




    /*
        This small class is a simple proxy class that let's us handle reference
        values to indizes in the bitvector without actually having a direct reference
    */
    struct BitVectorProxy
    {
        size_t _index;
        BitCompressedVector<T, B> *_vector;

        BitVectorProxy(size_t idx, BitCompressedVector<T, B> *v): _index(idx), _vector(v)
        {}

        // Implicit conversion operator used for rvalues of T
        inline operator const T () const
        {
            return _vector->get(_index);
        }

        // Usins the Proxy to set the value using the subscript as an lvalue
        inline BitVectorProxy& operator= (const T& rvalue)
        {
            _vector->set(_index, rvalue);
            return *this;
        }

    };

    /*
     * Shortcut method for get(size_t index)
     */
    inline const BitVectorProxy operator[] (const size_t index) const
    {
        return BitVectorProxy(index, this);
    }

    inline BitVectorProxy operator[] (const size_t index)
    {
        return BitVectorProxy(index, this);
    }





private:


    typedef uint8_t byte;
    typedef __m128i data_t;

    // function pointer helper
    typedef data_t (*mask_fun_ptr)(void);


    // Check if we are really 64bit
    static const uint8_t _width = sizeof(data_t) * 8;
    static const uint64_t _num_blocks = CACHE_LINE_SIZE / sizeof(data_t);

    // Pointer to the data
    data_t *_data;

    size_t _data_size;

    size_t _reserved;

    size_t _allocated_blocks;

    // get the position of an index inside the list of data values
    inline size_t _getPos(size_t index) const
    {
        return (index * B) / _width;
    }

    // get the offset of an index inside a block
    inline size_t _getOffset(size_t index, size_t base) const
    {
        return (index * B) - base;
    }

    // returns the offset mask for any given index
    inline data_t buildMask(size_t index) const
    {
        return (index * B) % _width;
    }

    // inline size_t _getHigherPowerOfTwo(size_t data_size)
    // {
    //     double exponent = ceil(log(data_size, 2));
    //     return (size_t)pow(2, exponent);
    // }

};

#define BRANCH_FREE_LT(x,y) (((x & ~y) | ((~(x ^ y)) & x - y)) >>  63)


template<typename T, uint8_t B>
void BitCompressedVector<T, B>::mget(const size_t index, value_type_ptr data, size_t *actual) const
{
    /*
    // First get the initial values
    data_t pos = _getPos(index);

    // Running values for the loop
    data_t currentValue;
    data_t offset = _getOffset(index, pos * _width);
    data_t bounds = _width - offset;

    // Base Mask
    data_t baseMask = global_bit_masks[B];

    // Align the block according to the offset
    data_t block = _data[pos] >>  offset;

    size_t left = (_num_blocks * _width) / B;
    size_t current = (pos * _width + offset) / B;
    size_t upper = left < (_reserved - current) ? left : _reserved - current;

    size_t counter = 0;
    for( ; counter < upper; ++counter)
    {

        // Extract the value
        currentValue = (baseMask & block);

        if (bounds > B)
        {
            bounds -= B;
            block >>= B;

        } else {

            offset = B - bounds;
            data_t mask = global_bit_masks[offset];
            currentValue |= (mask & _data[++pos]) << bounds;

            // Assign new block
            block = _data[pos] >> offset;
            bounds = _width - offset;
        }

        // Append current value
        data[counter] = currentValue;
    }

    *actual = counter;
    */
    size_t pos = _getPos(index);

    // this does not work because offset needs to be an immediate
    // size_t offset = _getOffset(index, pos * _width);
    // __m128i val = _mm_alignr_epi8 (_data[pos], _data[pos+1], offset);

    __m128i val = _data[pos];

    data[0] = (unsigned char) _mm_extract_epi8(val, 0);
    data[1] = (unsigned char) _mm_extract_epi8(val, 1);
    data[2] = (unsigned char) _mm_extract_epi8(val, 2);
    data[3] = (unsigned char) _mm_extract_epi8(val, 3);
    data[4] = (unsigned char) _mm_extract_epi8(val, 4);
    data[5] = (unsigned char) _mm_extract_epi8(val, 5);
    data[6] = (unsigned char) _mm_extract_epi8(val, 6);
    data[7] = (unsigned char) _mm_extract_epi8(val, 7);
    data[8] = (unsigned char) _mm_extract_epi8(val, 8);
    data[9] = (unsigned char) _mm_extract_epi8(val, 9);
    data[10] = (unsigned char) _mm_extract_epi8(val, 10);
    data[11] = (unsigned char) _mm_extract_epi8(val, 11);
    data[12] = (unsigned char) _mm_extract_epi8(val, 12);
    data[13] = (unsigned char) _mm_extract_epi8(val, 13);
    data[14] = (unsigned char) _mm_extract_epi8(val, 14);
    data[15] = (unsigned char) _mm_extract_epi8(val, 15);
    *actual = 16;
}

template<typename T, uint8_t B>
void BitCompressedVector<T, B>::set(const size_t index, const value_type v)
{
    /*
    data_t pos = _getPos(index);
    data_t offset = _getOffset(index, pos * _width);
    data_t bounds = _width - offset;

    data_t mask, baseMask;
    baseMask = global_bit_masks[B];
    mask = ~(baseMask << offset);


    _data[pos] &= mask;
    _data[pos] = _data[pos] | ((data_t) v << offset);

    if (bounds < B)
    {
        mask = ~(baseMask << offset); // we have a an overflow here thatswhy we do not need to care about the original stuff

       _data[pos + 1] &= mask; // clear bits
       _data[pos + 1] |= v >> bounds; // set bits and shift by the number of bits we already inserted
    }
    */

    size_t pos = _getPos(index);
    size_t offset = _getOffset(index, pos * _width);
    //printf("offset: %d\n", offset);
    switch (offset / 8)
    {
        case 0:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 0);
            break;
        case 1:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 1);
            break;
        case 2:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 2);
            break;
        case 3:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 3);
            break;
        case 4:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 4);
            break;
        case 5:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 5);
            break;
        case 6:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 6);
            break;
        case 7:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 7);
            break;
        case 8:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 8);
            break;
        case 9:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 9);
            break;
        case 10:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 10);
            break;
        case 11:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 11);
            break;
        case 12:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 12);
            break;
        case 13:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 13);
            break;
        case 14:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 14);
            break;
        case 15:
            _data[pos] = _mm_insert_epi8 (_data[pos], v, 15);
            break;

    }
    //_data[pos] = _mm_set_epi8(v, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    //__m128i mask = 0x00;
    //__m128i shiftedVal = _mm_shuffle_epi8(val, mask);
    // printf("&data: %u \n", _data);
    // printf("pos: %d, %u\n", pos, &_data[pos]);
    //_mm_storeu_si128(&_data[pos], val);
}

template<typename T, uint8_t B>
typename BitCompressedVector<T, B>::value_type BitCompressedVector<T, B>::get(const size_t index) const
{
    /*
    value_type result;
    data_t mask;

    data_t pos = _getPos(index);
    data_t offset = _getOffset(index, pos * _width);
    data_t bounds = _width - offset; // This is almost static expression, that could be handled with a switch case

    mask = global_bit_masks[B];
    data_t block = _data[pos];
    block >>= offset;

    result = (mask & block);

    if (bounds < B)
    {
        data_t b = B - bounds;
        mask = global_bit_masks[b];

        result |= (mask & _data[pos + 1]) << bounds;
    }
    return result;
    */


    unsigned char ret;

    size_t pos = _getPos(index);
    size_t offset = _getOffset(index, pos * _width);

    __m128i val = _data[pos];

    switch (offset / 8)
    {
        case 0:
            ret = (unsigned char) _mm_extract_epi8(val, 0);
            break;
        case 1:
            ret = (unsigned char) _mm_extract_epi8(val, 1);
            break;
        case 2:
            ret = (unsigned char) _mm_extract_epi8(val, 2);
            break;
        case 3:
            ret = (unsigned char) _mm_extract_epi8(val, 3);
            break;
        case 4:
            ret = (unsigned char) _mm_extract_epi8(val, 4);
            break;
        case 5:
            ret = (unsigned char) _mm_extract_epi8(val, 5);
            break;
        case 6:
            ret = (unsigned char) _mm_extract_epi8(val, 6);
            break;
        case 7:
            ret = (unsigned char) _mm_extract_epi8(val, 7);
            break;
        case 8:
            ret = (unsigned char) _mm_extract_epi8(val, 8);
            break;
        case 9:
            ret = (unsigned char) _mm_extract_epi8(val, 9);
            break;
        case 10:
            ret = (unsigned char) _mm_extract_epi8(val, 10);
            break;
        case 11:
            ret = (unsigned char) _mm_extract_epi8(val, 11);
            break;
        case 12:
            ret = (unsigned char) _mm_extract_epi8(val, 12);
            break;
        case 13:
            ret = (unsigned char) _mm_extract_epi8(val, 13);
            break;
        case 14:
            ret = (unsigned char) _mm_extract_epi8(val, 14);
            break;
        case 15:
            ret = (unsigned char) _mm_extract_epi8(val, 15);
            break;
    }

    return ret;
}
