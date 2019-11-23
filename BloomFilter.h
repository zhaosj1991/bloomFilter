#include <stdint.h>
#include <math.h>
#include "MurmurHash.h"

#define BYTE_BIT            8
#define HASH_H2_64(x)       ((x>>32)^x)
#define SET_BIT(x, y)       (x[y / BYTE_BIT] |= (1 << (y % BYTE_BIT)))
#define GET_BIT(x, y)       (x[y / BYTE_BIT] & (1 << (y % BYTE_BIT)))

class CBloomFilter
{
public:
    // ui32ItemNum:         数据总量大小，超出这个范围后也可以检测，但不能保证假阳率
    // dFalsePositiveRate:  期望的假阳率
    // ui32Seed:            MurmurHash的种子点，可以任意选取
    CBloomFilter(uint32_t ui32ItemNum = 1000000, double dFalsePositiveRate = 0.000001, uint32_t ui32Seed = 1314)
    : m_ui32ItemNum(ui32ItemNum),
      m_dFalsePositiveRate(dFalsePositiveRate),
      m_ui32Seed(ui32Seed)
    {
        m_ui32NumBits = ceil((ui32ItemNum * log(dFalsePositiveRate)) / log(1 / pow(2, log(2))));
        m_ui16NumHash = round((m_ui32NumBits / ui32ItemNum) * log(2));
        m_pHashRes = new uint8_t*[m_ui16NumHash];
        for (int i = 0; i < m_ui16NumHash; i++)
        {
            // 每个Res需要 m_ui32NumBits / BYTE_BIT 字节去存储 hash结果
            m_pHashRes[i] = new uint8_t[m_ui32NumBits / BYTE_BIT + 1];
        }
        
    }

    ~CBloomFilter()
    {
        for (int i = 0; i < m_ui16NumHash; i++)
        {
            delete[] m_pHashRes[i];
        }

        delete[] m_pHashRes;
        m_pHashRes = nullptr;
    }

    // Hash(key, i) = (H1(key) + i * H2(key)) % m
    int add(void* item, int nlen)
    {   
        uint64_t h1 = MurmurHash64A(item, nlen, m_ui32Seed);
        uint64_t h2 = HASH_H2_64(h1);
        uint64_t ui64_temp = 0;
        for (size_t i = 0; i < m_ui16NumHash; i++)
        {
            ui64_temp = (h1 + i * (h2)) % m_ui32NumBits;
            SET_BIT(m_pHashRes[i], ui64_temp);
        }
        
        return 0;
    }

    bool check(void* item, int nlen)
    {
        uint64_t h1 = MurmurHash64A(item, nlen, m_ui32Seed);
        uint64_t h2 = HASH_H2_64(h1);
        uint64_t ui64_temp = 0;
        for (size_t i = 0; i < m_ui16NumHash; i++)
        {
            ui64_temp = (h1 + i * (h2)) % m_ui32NumBits;
            if(!GET_BIT(m_pHashRes[i], ui64_temp))
                return false;
        }

        return true;
    }

private:
    uint32_t  m_ui32ItemNum;
    double    m_dFalsePositiveRate;
    uint32_t  m_ui32Seed;
    uint32_t  m_ui32NumBits;
    uint16_t  m_ui16NumHash;
    uint8_t   **m_pHashRes;
};
