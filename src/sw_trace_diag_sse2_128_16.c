/**
 * @file
 *
 * @author jeffrey.daily@gmail.com
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdlib.h>

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <emmintrin.h>
#endif

#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_sse.h"

#define NEG_INF (INT16_MIN/(int16_t)(2))

static inline __m128i _mm_blendv_epi8_rpl(__m128i a, __m128i b, __m128i mask) {
    a = _mm_andnot_si128(mask, a);
    a = _mm_or_si128(a, _mm_and_si128(mask, b));
    return a;
}


static inline void arr_store_si128(
        int8_t *array,
        __m128i vWH,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[1LL*(i+0)*s2Len + (j-0)] = (int8_t)_mm_extract_epi16(vWH, 7);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[1LL*(i+1)*s2Len + (j-1)] = (int8_t)_mm_extract_epi16(vWH, 6);
    }
    if (0 <= i+2 && i+2 < s1Len && 0 <= j-2 && j-2 < s2Len) {
        array[1LL*(i+2)*s2Len + (j-2)] = (int8_t)_mm_extract_epi16(vWH, 5);
    }
    if (0 <= i+3 && i+3 < s1Len && 0 <= j-3 && j-3 < s2Len) {
        array[1LL*(i+3)*s2Len + (j-3)] = (int8_t)_mm_extract_epi16(vWH, 4);
    }
    if (0 <= i+4 && i+4 < s1Len && 0 <= j-4 && j-4 < s2Len) {
        array[1LL*(i+4)*s2Len + (j-4)] = (int8_t)_mm_extract_epi16(vWH, 3);
    }
    if (0 <= i+5 && i+5 < s1Len && 0 <= j-5 && j-5 < s2Len) {
        array[1LL*(i+5)*s2Len + (j-5)] = (int8_t)_mm_extract_epi16(vWH, 2);
    }
    if (0 <= i+6 && i+6 < s1Len && 0 <= j-6 && j-6 < s2Len) {
        array[1LL*(i+6)*s2Len + (j-6)] = (int8_t)_mm_extract_epi16(vWH, 1);
    }
    if (0 <= i+7 && i+7 < s1Len && 0 <= j-7 && j-7 < s2Len) {
        array[1LL*(i+7)*s2Len + (j-7)] = (int8_t)_mm_extract_epi16(vWH, 0);
    }
}

#define FNAME parasail_sw_trace_diag_sse2_128_16

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    /* declare local variables */
    int32_t N = 0;
    int32_t PAD = 0;
    int32_t PAD2 = 0;
    int32_t s1Len_PAD = 0;
    int32_t s2Len_PAD = 0;
    int16_t * restrict s1 = NULL;
    int16_t * restrict s2B = NULL;
    int16_t * restrict _H_pr = NULL;
    int16_t * restrict _F_pr = NULL;
    int16_t * restrict s2 = NULL;
    int16_t * restrict H_pr = NULL;
    int16_t * restrict F_pr = NULL;
    parasail_result_t *result = NULL;
    int32_t i = 0;
    int32_t j = 0;
    int32_t end_query = 0;
    int32_t end_ref = 0;
    int16_t score = 0;
    __m128i vNegInf;
    __m128i vNegInf0;
    __m128i vOpen;
    __m128i vGap;
    __m128i vZero;
    __m128i vOne;
    __m128i vN;
    __m128i vNegOne;
    __m128i vI;
    __m128i vJreset;
    __m128i vMaxH;
    __m128i vEndI;
    __m128i vEndJ;
    __m128i vILimit;
    __m128i vJLimit;
    __m128i vTDiag;
    __m128i vTIns;
    __m128i vTDel;
    __m128i vTZero;
    __m128i vTDiagE;
    __m128i vTInsE;
    __m128i vTDiagF;
    __m128i vTDelF;
    

    /* validate inputs */
    PARASAIL_CHECK_NULL(_s1);
    PARASAIL_CHECK_GT0(s1Len);
    PARASAIL_CHECK_NULL(_s2);
    PARASAIL_CHECK_GT0(s2Len);
    PARASAIL_CHECK_GE0(open);
    PARASAIL_CHECK_GE0(gap);
    PARASAIL_CHECK_NULL(matrix);
        
    /* initialize stack variables */
    N = 8; /* number of values in vector */
    PAD = N-1;
    PAD2 = PAD*2;
    s1Len_PAD = s1Len+PAD;
    s2Len_PAD = s2Len+PAD;
    i = 0;
    j = 0;
    end_query = 0;
    end_ref = 0;
    score = NEG_INF;
    vNegInf = _mm_set1_epi16(NEG_INF);
    vNegInf0 = _mm_srli_si128(vNegInf, 2); /* shift in a 0 */
    vOpen = _mm_set1_epi16(open);
    vGap  = _mm_set1_epi16(gap);
    vZero = _mm_set1_epi16(0);
    vOne = _mm_set1_epi16(1);
    vN = _mm_set1_epi16(N);
    vNegOne = _mm_set1_epi16(-1);
    vI = _mm_set_epi16(0,1,2,3,4,5,6,7);
    vJreset = _mm_set_epi16(0,-1,-2,-3,-4,-5,-6,-7);
    vMaxH = vNegInf;
    vEndI = vNegInf;
    vEndJ = vNegInf;
    vILimit = _mm_set1_epi16(s1Len);
    vJLimit = _mm_set1_epi16(s2Len);
    vTDiag = _mm_set1_epi16(PARASAIL_DIAG);
    vTIns = _mm_set1_epi16(PARASAIL_INS);
    vTDel = _mm_set1_epi16(PARASAIL_DEL);
    vTZero = _mm_set1_epi16(PARASAIL_ZERO);
    vTDiagE = _mm_set1_epi16(PARASAIL_DIAG_E);
    vTInsE = _mm_set1_epi16(PARASAIL_INS_E);
    vTDiagF = _mm_set1_epi16(PARASAIL_DIAG_F);
    vTDelF = _mm_set1_epi16(PARASAIL_DEL_F);
    

    /* initialize result */
    result = parasail_result_new_trace(s1Len, s2Len, 16, sizeof(int8_t));
    if (!result) return NULL;

    /* set known flags */
    result->flag |= PARASAIL_FLAG_SW | PARASAIL_FLAG_DIAG
        | PARASAIL_FLAG_TRACE
        | PARASAIL_FLAG_BITS_16 | PARASAIL_FLAG_LANES_8;

    /* initialize heap variables */
    s1 = parasail_memalign_int16_t(16, s1Len+PAD);
    s2B= parasail_memalign_int16_t(16, s2Len+PAD2);
    _H_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    _F_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    s2 = s2B+PAD; /* will allow later for negative indices */
    H_pr = _H_pr+PAD;
    F_pr = _F_pr+PAD;

    /* validate heap variables */
    if (!s1) return NULL;
    if (!s2B) return NULL;
    if (!_H_pr) return NULL;
    if (!_F_pr) return NULL;

    /* convert _s1 from char to int in range 0-23 */
    for (i=0; i<s1Len; ++i) {
        s1[i] = matrix->mapper[(unsigned char)_s1[i]];
    }
    /* pad back of s1 with dummy values */
    for (i=s1Len; i<s1Len_PAD; ++i) {
        s1[i] = 0; /* point to first matrix row because we don't care */
    }

    /* convert _s2 from char to int in range 0-23 */
    for (j=0; j<s2Len; ++j) {
        s2[j] = matrix->mapper[(unsigned char)_s2[j]];
    }
    /* pad front of s2 with dummy values */
    for (j=-PAD; j<0; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }
    /* pad back of s2 with dummy values */
    for (j=s2Len; j<s2Len_PAD; ++j) {
        s2[j] = 0; /* point to first matrix row because we don't care */
    }

    /* set initial values for stored row */
    for (j=0; j<s2Len; ++j) {
        H_pr[j] = 0;
        F_pr[j] = NEG_INF;
    }
    /* pad front of stored row values */
    for (j=-PAD; j<0; ++j) {
        H_pr[j] = NEG_INF;
        F_pr[j] = NEG_INF;
    }
    /* pad back of stored row values */
    for (j=s2Len; j<s2Len+PAD; ++j) {
        H_pr[j] = NEG_INF;
        F_pr[j] = NEG_INF;
    }

    /* iterate over query sequence */
    for (i=0; i<s1Len; i+=N) {
        __m128i vNH = vNegInf0;
        __m128i vWH = vNegInf0;
        __m128i vE = vNegInf;
        __m128i vE_opn = vNegInf;
        __m128i vE_ext = vNegInf;
        __m128i vF = vNegInf;
        __m128i vF_opn = vNegInf;
        __m128i vF_ext = vNegInf;
        __m128i vJ = vJreset;
        const int * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        const int * const restrict matrow2 = &matrix->matrix[matrix->size*s1[i+2]];
        const int * const restrict matrow3 = &matrix->matrix[matrix->size*s1[i+3]];
        const int * const restrict matrow4 = &matrix->matrix[matrix->size*s1[i+4]];
        const int * const restrict matrow5 = &matrix->matrix[matrix->size*s1[i+5]];
        const int * const restrict matrow6 = &matrix->matrix[matrix->size*s1[i+6]];
        const int * const restrict matrow7 = &matrix->matrix[matrix->size*s1[i+7]];
        __m128i vIltLimit = _mm_cmplt_epi16(vI, vILimit);
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            __m128i vMat;
            __m128i vNWH = vNH;
            vNH = _mm_srli_si128(vWH, 2);
            vNH = _mm_insert_epi16(vNH, H_pr[j], 7);
            vF = _mm_srli_si128(vF, 2);
            vF = _mm_insert_epi16(vF, F_pr[j], 7);
            vF_opn = _mm_sub_epi16(vNH, vOpen);
            vF_ext = _mm_sub_epi16(vF, vGap);
            vF = _mm_max_epi16(vF_opn, vF_ext);
            vE_opn = _mm_sub_epi16(vWH, vOpen);
            vE_ext = _mm_sub_epi16(vE, vGap);
            vE = _mm_max_epi16(vE_opn, vE_ext);
            vMat = _mm_set_epi16(
                    matrow0[s2[j-0]],
                    matrow1[s2[j-1]],
                    matrow2[s2[j-2]],
                    matrow3[s2[j-3]],
                    matrow4[s2[j-4]],
                    matrow5[s2[j-5]],
                    matrow6[s2[j-6]],
                    matrow7[s2[j-7]]
                    );
            vNWH = _mm_add_epi16(vNWH, vMat);
            vNWH = _mm_max_epi16(vNWH, vZero);
            vWH = _mm_max_epi16(vNWH, vE);
            vWH = _mm_max_epi16(vWH, vF);
            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                __m128i cond = _mm_cmpeq_epi16(vJ,vNegOne);
                vWH = _mm_andnot_si128(cond, vWH);
                vF = _mm_blendv_epi8_rpl(vF, vNegInf, cond);
                vE = _mm_blendv_epi8_rpl(vE, vNegInf, cond);
            }
            
            /* trace table */
            {
                __m128i cond_zero = _mm_cmpeq_epi16(vWH, vZero);
                __m128i case1 = _mm_cmpeq_epi16(vWH, vNWH);
                __m128i case2 = _mm_cmpeq_epi16(vWH, vF);
                __m128i vT = _mm_blendv_epi8_rpl(
                        _mm_blendv_epi8_rpl(vTIns, vTDel, case2),
                        _mm_blendv_epi8_rpl(vTDiag, vTZero, cond_zero),
                        case1);
                __m128i condE = _mm_cmpgt_epi16(vE_opn, vE_ext);
                __m128i condF = _mm_cmpgt_epi16(vF_opn, vF_ext);
                __m128i vET = _mm_blendv_epi8_rpl(vTInsE, vTDiagE, condE);
                __m128i vFT = _mm_blendv_epi8_rpl(vTDelF, vTDiagF, condF);
                vT = _mm_or_si128(vT, vET);
                vT = _mm_or_si128(vT, vFT);
                arr_store_si128(result->trace->trace_table, vT, i, s1Len, j, s2Len);
            }
            H_pr[j-7] = (int16_t)_mm_extract_epi16(vWH,0);
            F_pr[j-7] = (int16_t)_mm_extract_epi16(vF,0);
            /* as minor diagonal vector passes across table, extract
             * max values within the i,j bounds */
            {
                __m128i cond_valid_J = _mm_and_si128(
                        _mm_cmpgt_epi16(vJ, vNegOne),
                        _mm_cmplt_epi16(vJ, vJLimit));
                __m128i cond_valid_IJ = _mm_and_si128(cond_valid_J, vIltLimit);
                __m128i cond_eq = _mm_cmpeq_epi16(vWH, vMaxH);
                __m128i cond_max = _mm_cmpgt_epi16(vWH, vMaxH);
                __m128i cond_all = _mm_and_si128(cond_max, cond_valid_IJ);
                __m128i cond_Jlt = _mm_cmplt_epi16(vJ, vEndJ);
                vMaxH = _mm_blendv_epi8_rpl(vMaxH, vWH, cond_all);
                vEndI = _mm_blendv_epi8_rpl(vEndI, vI, cond_all);
                vEndJ = _mm_blendv_epi8_rpl(vEndJ, vJ, cond_all);
                cond_all = _mm_and_si128(cond_Jlt, cond_eq);
                cond_all = _mm_and_si128(cond_all, cond_valid_IJ);
                vEndI = _mm_blendv_epi8_rpl(vEndI, vI, cond_all);
                vEndJ = _mm_blendv_epi8_rpl(vEndJ, vJ, cond_all);
            }
            vJ = _mm_add_epi16(vJ, vOne);
        }
        vI = _mm_add_epi16(vI, vN);
    }

    /* alignment ending position */
    {
        int16_t *t = (int16_t*)&vMaxH;
        int16_t *i = (int16_t*)&vEndI;
        int16_t *j = (int16_t*)&vEndJ;
        int32_t k;
        for (k=0; k<N; ++k, ++t, ++i, ++j) {
            if (*t > score) {
                score = *t;
                end_query = *i;
                end_ref = *j;
            }
            else if (*t == score) {
                if (*j < end_ref) {
                    end_query = *i;
                    end_ref = *j;
                }
                else if (*j == end_ref && *i < end_query) {
                    end_query = *i;
                    end_ref = *j;
                }
            }
        }
    }

    

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;

    parasail_free(_F_pr);
    parasail_free(_H_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


