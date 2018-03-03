/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdlib.h>



#include "parasail.h"
#include "parasail/memory.h"
#include "parasail/internal_altivec.h"



#ifdef PARASAIL_TABLE
static inline void arr_store_si128(
        int *array,
        vec128i vWH,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (0 <= i+0 && i+0 < s1Len && 0 <= j-0 && j-0 < s2Len) {
        array[1LL*(i+0)*s2Len + (j-0)] = (int16_t)_mm_extract_epi16(vWH, 7);
    }
    if (0 <= i+1 && i+1 < s1Len && 0 <= j-1 && j-1 < s2Len) {
        array[1LL*(i+1)*s2Len + (j-1)] = (int16_t)_mm_extract_epi16(vWH, 6);
    }
    if (0 <= i+2 && i+2 < s1Len && 0 <= j-2 && j-2 < s2Len) {
        array[1LL*(i+2)*s2Len + (j-2)] = (int16_t)_mm_extract_epi16(vWH, 5);
    }
    if (0 <= i+3 && i+3 < s1Len && 0 <= j-3 && j-3 < s2Len) {
        array[1LL*(i+3)*s2Len + (j-3)] = (int16_t)_mm_extract_epi16(vWH, 4);
    }
    if (0 <= i+4 && i+4 < s1Len && 0 <= j-4 && j-4 < s2Len) {
        array[1LL*(i+4)*s2Len + (j-4)] = (int16_t)_mm_extract_epi16(vWH, 3);
    }
    if (0 <= i+5 && i+5 < s1Len && 0 <= j-5 && j-5 < s2Len) {
        array[1LL*(i+5)*s2Len + (j-5)] = (int16_t)_mm_extract_epi16(vWH, 2);
    }
    if (0 <= i+6 && i+6 < s1Len && 0 <= j-6 && j-6 < s2Len) {
        array[1LL*(i+6)*s2Len + (j-6)] = (int16_t)_mm_extract_epi16(vWH, 1);
    }
    if (0 <= i+7 && i+7 < s1Len && 0 <= j-7 && j-7 < s2Len) {
        array[1LL*(i+7)*s2Len + (j-7)] = (int16_t)_mm_extract_epi16(vWH, 0);
    }
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_rowcol(
        int *row,
        int *col,
        vec128i vWH,
        int32_t i,
        int32_t s1Len,
        int32_t j,
        int32_t s2Len)
{
    if (i+0 == s1Len-1 && 0 <= j-0 && j-0 < s2Len) {
        row[j-0] = (int16_t)_mm_extract_epi16(vWH, 7);
    }
    if (j-0 == s2Len-1 && 0 <= i+0 && i+0 < s1Len) {
        col[(i+0)] = (int16_t)_mm_extract_epi16(vWH, 7);
    }
    if (i+1 == s1Len-1 && 0 <= j-1 && j-1 < s2Len) {
        row[j-1] = (int16_t)_mm_extract_epi16(vWH, 6);
    }
    if (j-1 == s2Len-1 && 0 <= i+1 && i+1 < s1Len) {
        col[(i+1)] = (int16_t)_mm_extract_epi16(vWH, 6);
    }
    if (i+2 == s1Len-1 && 0 <= j-2 && j-2 < s2Len) {
        row[j-2] = (int16_t)_mm_extract_epi16(vWH, 5);
    }
    if (j-2 == s2Len-1 && 0 <= i+2 && i+2 < s1Len) {
        col[(i+2)] = (int16_t)_mm_extract_epi16(vWH, 5);
    }
    if (i+3 == s1Len-1 && 0 <= j-3 && j-3 < s2Len) {
        row[j-3] = (int16_t)_mm_extract_epi16(vWH, 4);
    }
    if (j-3 == s2Len-1 && 0 <= i+3 && i+3 < s1Len) {
        col[(i+3)] = (int16_t)_mm_extract_epi16(vWH, 4);
    }
    if (i+4 == s1Len-1 && 0 <= j-4 && j-4 < s2Len) {
        row[j-4] = (int16_t)_mm_extract_epi16(vWH, 3);
    }
    if (j-4 == s2Len-1 && 0 <= i+4 && i+4 < s1Len) {
        col[(i+4)] = (int16_t)_mm_extract_epi16(vWH, 3);
    }
    if (i+5 == s1Len-1 && 0 <= j-5 && j-5 < s2Len) {
        row[j-5] = (int16_t)_mm_extract_epi16(vWH, 2);
    }
    if (j-5 == s2Len-1 && 0 <= i+5 && i+5 < s1Len) {
        col[(i+5)] = (int16_t)_mm_extract_epi16(vWH, 2);
    }
    if (i+6 == s1Len-1 && 0 <= j-6 && j-6 < s2Len) {
        row[j-6] = (int16_t)_mm_extract_epi16(vWH, 1);
    }
    if (j-6 == s2Len-1 && 0 <= i+6 && i+6 < s1Len) {
        col[(i+6)] = (int16_t)_mm_extract_epi16(vWH, 1);
    }
    if (i+7 == s1Len-1 && 0 <= j-7 && j-7 < s2Len) {
        row[j-7] = (int16_t)_mm_extract_epi16(vWH, 0);
    }
    if (j-7 == s2Len-1 && 0 <= i+7 && i+7 < s1Len) {
        col[(i+7)] = (int16_t)_mm_extract_epi16(vWH, 0);
    }
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_nw_stats_table_diag_altivec_128_16
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_nw_stats_rowcol_diag_altivec_128_16
#else
#define FNAME parasail_nw_stats_diag_altivec_128_16
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict _s1, const int s1Len,
        const char * const restrict _s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    const int32_t N = 8; /* number of values in vector */
    const int32_t PAD = N-1;
    const int32_t PAD2 = PAD*2;
    const int32_t s1Len_PAD = s1Len+PAD;
    const int32_t s2Len_PAD = s2Len+PAD;
    int16_t * const restrict s1      = parasail_memalign_int16_t(16, s1Len+PAD);
    int16_t * const restrict s2B     = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _H_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _HM_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _HS_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _HL_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _F_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _FM_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _FS_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict _FL_pr = parasail_memalign_int16_t(16, s2Len+PAD2);
    int16_t * const restrict s2 = s2B+PAD; /* will allow later for negative indices */
    int16_t * const restrict H_pr = _H_pr+PAD;
    int16_t * const restrict HM_pr = _HM_pr+PAD;
    int16_t * const restrict HS_pr = _HS_pr+PAD;
    int16_t * const restrict HL_pr = _HL_pr+PAD;
    int16_t * const restrict F_pr = _F_pr+PAD;
    int16_t * const restrict FM_pr = _FM_pr+PAD;
    int16_t * const restrict FS_pr = _FS_pr+PAD;
    int16_t * const restrict FL_pr = _FL_pr+PAD;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table3(s1Len, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol3(s1Len, s2Len);
#else
    parasail_result_t *result = parasail_result_new_stats();
#endif
#endif
    int32_t i = 0;
    int32_t j = 0;
    int32_t end_query = s1Len-1;
    int32_t end_ref = s2Len-1;
    const int16_t NEG_LIMIT = (-open < matrix->min ?
        INT16_MIN + open : INT16_MIN - matrix->min) + 1;
    const int16_t POS_LIMIT = INT16_MAX - matrix->max - 1;
    int16_t score = NEG_LIMIT;
    int16_t matches = NEG_LIMIT;
    int16_t similar = NEG_LIMIT;
    int16_t length = NEG_LIMIT;
    vec128i vNegLimit = _mm_set1_epi16(NEG_LIMIT);
    vec128i vPosLimit = _mm_set1_epi16(POS_LIMIT);
    vec128i vSaturationCheckMin = vPosLimit;
    vec128i vSaturationCheckMax = vNegLimit;
    vec128i vNegInf = _mm_set1_epi16(NEG_LIMIT);
    vec128i vOpen = _mm_set1_epi16(open);
    vec128i vGap  = _mm_set1_epi16(gap);
    vec128i vZero = _mm_set1_epi16(0);
    vec128i vNegInf0 = _mm_insert_epi16(vZero, NEG_LIMIT, 7);
    vec128i vOne = _mm_set1_epi16(1);
    vec128i vN = _mm_set1_epi16(N);
    vec128i vGapN = _mm_set1_epi16(gap*N);
    vec128i vNegOne = _mm_set1_epi16(-1);
    vec128i vI = _mm_set_epi16(0,1,2,3,4,5,6,7);
    vec128i vJreset = _mm_set_epi16(0,-1,-2,-3,-4,-5,-6,-7);
    vec128i vMaxH = vNegInf;
    vec128i vMaxM = vNegInf;
    vec128i vMaxS = vNegInf;
    vec128i vMaxL = vNegInf;
    vec128i vILimit = _mm_set1_epi16(s1Len);
    vec128i vILimit1 = _mm_sub_epi16(vILimit, vOne);
    vec128i vJLimit = _mm_set1_epi16(s2Len);
    vec128i vJLimit1 = _mm_sub_epi16(vJLimit, vOne);
    vec128i vIBoundary = _mm_set_epi16(
            -open-0*gap,
            -open-1*gap,
            -open-2*gap,
            -open-3*gap,
            -open-4*gap,
            -open-5*gap,
            -open-6*gap,
            -open-7*gap);

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
        H_pr[j] = -open - j*gap;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = NEG_LIMIT;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    /* pad front of stored row values */
    for (j=-PAD; j<0; ++j) {
        H_pr[j] = 0;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = 0;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    /* pad back of stored row values */
    for (j=s2Len; j<s2Len+PAD; ++j) {
        H_pr[j] = 0;
        HM_pr[j] = 0;
        HS_pr[j] = 0;
        HL_pr[j] = 0;
        F_pr[j] = 0;
        FM_pr[j] = 0;
        FS_pr[j] = 0;
        FL_pr[j] = 0;
    }
    H_pr[-1] = 0; /* upper left corner */

    /* iterate over query sequence */
    for (i=0; i<s1Len; i+=N) {
        vec128i case1 = vZero;
        vec128i case2 = vZero;
        vec128i vNH = vZero;
        vec128i vNM = vZero;
        vec128i vNS = vZero;
        vec128i vNL = vZero;
        vec128i vWH = vZero;
        vec128i vWM = vZero;
        vec128i vWS = vZero;
        vec128i vWL = vZero;
        vec128i vE = vNegInf0;
        vec128i vE_opn = vNegInf;
        vec128i vE_ext = vNegInf;
        vec128i vEM = vZero;
        vec128i vES = vZero;
        vec128i vEL = vZero;
        vec128i vF = vNegInf0;
        vec128i vF_opn = vNegInf;
        vec128i vF_ext = vNegInf;
        vec128i vFM = vZero;
        vec128i vFS = vZero;
        vec128i vFL = vZero;
        vec128i vJ = vJreset;
        vec128i vs1 = _mm_set_epi16(
                s1[i+0],
                s1[i+1],
                s1[i+2],
                s1[i+3],
                s1[i+4],
                s1[i+5],
                s1[i+6],
                s1[i+7]);
        vec128i vs2 = vNegInf;
        const int * const restrict matrow0 = &matrix->matrix[matrix->size*s1[i+0]];
        const int * const restrict matrow1 = &matrix->matrix[matrix->size*s1[i+1]];
        const int * const restrict matrow2 = &matrix->matrix[matrix->size*s1[i+2]];
        const int * const restrict matrow3 = &matrix->matrix[matrix->size*s1[i+3]];
        const int * const restrict matrow4 = &matrix->matrix[matrix->size*s1[i+4]];
        const int * const restrict matrow5 = &matrix->matrix[matrix->size*s1[i+5]];
        const int * const restrict matrow6 = &matrix->matrix[matrix->size*s1[i+6]];
        const int * const restrict matrow7 = &matrix->matrix[matrix->size*s1[i+7]];
        vNH = _mm_insert_epi16(vNH, H_pr[-1], 7);
        vWH = _mm_insert_epi16(vWH, -open - i*gap, 7);
        H_pr[-1] = -open - (i+N)*gap;
        /* iterate over database sequence */
        for (j=0; j<s2Len+PAD; ++j) {
            vec128i vMat;
            vec128i vNWH = vNH;
            vec128i vNWM = vNM;
            vec128i vNWS = vNS;
            vec128i vNWL = vNL;
            vNH = _mm_srli_si128(vWH, 2);
            vNH = _mm_insert_epi16(vNH, H_pr[j], 7);
            vNM = _mm_srli_si128(vWM, 2);
            vNM = _mm_insert_epi16(vNM, HM_pr[j], 7);
            vNS = _mm_srli_si128(vWS, 2);
            vNS = _mm_insert_epi16(vNS, HS_pr[j], 7);
            vNL = _mm_srli_si128(vWL, 2);
            vNL = _mm_insert_epi16(vNL, HL_pr[j], 7);
            vF = _mm_srli_si128(vF, 2);
            vF = _mm_insert_epi16(vF, F_pr[j], 7);
            vFM = _mm_srli_si128(vFM, 2);
            vFM = _mm_insert_epi16(vFM, FM_pr[j], 7);
            vFS = _mm_srli_si128(vFS, 2);
            vFS = _mm_insert_epi16(vFS, FS_pr[j], 7);
            vFL = _mm_srli_si128(vFL, 2);
            vFL = _mm_insert_epi16(vFL, FL_pr[j], 7);
            vF_opn = _mm_sub_epi16(vNH, vOpen);
            vF_ext = _mm_sub_epi16(vF, vGap);
            vF = _mm_max_epi16(vF_opn, vF_ext);
            case1 = _mm_cmpgt_epi16(vF_opn, vF_ext);
            vFM = _mm_blendv_epi8(vFM, vNM, case1);
            vFS = _mm_blendv_epi8(vFS, vNS, case1);
            vFL = _mm_blendv_epi8(vFL, vNL, case1);
            vFL = _mm_add_epi16(vFL, vOne);
            vE_opn = _mm_sub_epi16(vWH, vOpen);
            vE_ext = _mm_sub_epi16(vE, vGap);
            vE = _mm_max_epi16(vE_opn, vE_ext);
            case1 = _mm_cmpgt_epi16(vE_opn, vE_ext);
            vEM = _mm_blendv_epi8(vEM, vWM, case1);
            vES = _mm_blendv_epi8(vES, vWS, case1);
            vEL = _mm_blendv_epi8(vEL, vWL, case1);
            vEL = _mm_add_epi16(vEL, vOne);
            vs2 = _mm_srli_si128(vs2, 2);
            vs2 = _mm_insert_epi16(vs2, s2[j], 7);
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
            vWH = _mm_max_epi16(vNWH, vE);
            vWH = _mm_max_epi16(vWH, vF);
            case1 = _mm_cmpeq_epi16(vWH, vNWH);
            case2 = _mm_cmpeq_epi16(vWH, vF);
            vWM = _mm_blendv_epi8(
                    _mm_blendv_epi8(vEM, vFM, case2),
                    _mm_add_epi16(vNWM,
                        _mm_and_si128(
                            _mm_cmpeq_epi16(vs1,vs2),
                            vOne)),
                    case1);
            vWS = _mm_blendv_epi8(
                    _mm_blendv_epi8(vES, vFS, case2),
                    _mm_add_epi16(vNWS,
                        _mm_and_si128(
                            _mm_cmpgt_epi16(vMat,vZero),
                            vOne)),
                    case1);
            vWL = _mm_blendv_epi8(
                    _mm_blendv_epi8(vEL, vFL, case2),
                    _mm_add_epi16(vNWL, vOne), case1);
            /* as minor diagonal vector passes across the j=-1 boundary,
             * assign the appropriate boundary conditions */
            {
                vec128i cond = _mm_cmpeq_epi16(vJ,vNegOne);
                vWH = _mm_blendv_epi8(vWH, vIBoundary, cond);
                vWM = _mm_andnot_si128(cond, vWM);
                vWS = _mm_andnot_si128(cond, vWS);
                vWL = _mm_andnot_si128(cond, vWL);
                vE = _mm_blendv_epi8(vE, vNegInf, cond);
                vEM = _mm_andnot_si128(cond, vEM);
                vES = _mm_andnot_si128(cond, vES);
                vEL = _mm_andnot_si128(cond, vEL);
            }
            vSaturationCheckMin = _mm_min_epi16(vSaturationCheckMin, vWH);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vWH);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vWM);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vWS);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vWL);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vWL);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vJ);
#ifdef PARASAIL_TABLE
            arr_store_si128(result->stats->tables->score_table, vWH, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->matches_table, vWM, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->similar_table, vWS, i, s1Len, j, s2Len);
            arr_store_si128(result->stats->tables->length_table, vWL, i, s1Len, j, s2Len);
#endif
#ifdef PARASAIL_ROWCOL
            arr_store_rowcol(result->stats->rowcols->score_row,   result->stats->rowcols->score_col, vWH, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->matches_row, result->stats->rowcols->matches_col, vWM, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->similar_row, result->stats->rowcols->similar_col, vWS, i, s1Len, j, s2Len);
            arr_store_rowcol(result->stats->rowcols->length_row,  result->stats->rowcols->length_col, vWL, i, s1Len, j, s2Len);
#endif
            H_pr[j-7] = (int16_t)_mm_extract_epi16(vWH,0);
            HM_pr[j-7] = (int16_t)_mm_extract_epi16(vWM,0);
            HS_pr[j-7] = (int16_t)_mm_extract_epi16(vWS,0);
            HL_pr[j-7] = (int16_t)_mm_extract_epi16(vWL,0);
            F_pr[j-7] = (int16_t)_mm_extract_epi16(vF,0);
            FM_pr[j-7] = (int16_t)_mm_extract_epi16(vFM,0);
            FS_pr[j-7] = (int16_t)_mm_extract_epi16(vFS,0);
            FL_pr[j-7] = (int16_t)_mm_extract_epi16(vFL,0);
            /* as minor diagonal vector passes across table, extract
               last table value at the i,j bound */
            {
                vec128i cond_valid_I = _mm_cmpeq_epi16(vI, vILimit1);
                vec128i cond_valid_J = _mm_cmpeq_epi16(vJ, vJLimit1);
                vec128i cond_all = _mm_and_si128(cond_valid_I, cond_valid_J);
                vMaxH = _mm_blendv_epi8(vMaxH, vWH, cond_all);
                vMaxM = _mm_blendv_epi8(vMaxM, vWM, cond_all);
                vMaxS = _mm_blendv_epi8(vMaxS, vWS, cond_all);
                vMaxL = _mm_blendv_epi8(vMaxL, vWL, cond_all);
            }
            vJ = _mm_add_epi16(vJ, vOne);
        }
        vI = _mm_add_epi16(vI, vN);
        vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vI);
        vIBoundary = _mm_sub_epi16(vIBoundary, vGapN);
    }

    /* max in vMaxH */
    for (i=0; i<N; ++i) {
        int16_t value;
        value = (int16_t) _mm_extract_epi16(vMaxH, 7);
        if (value > score) {
            score = value;
            matches = (int16_t) _mm_extract_epi16(vMaxM, 7);
            similar = (int16_t) _mm_extract_epi16(vMaxS, 7);
            length= (int16_t) _mm_extract_epi16(vMaxL, 7);
        }
        vMaxH = _mm_slli_si128(vMaxH, 2);
        vMaxM = _mm_slli_si128(vMaxM, 2);
        vMaxS = _mm_slli_si128(vMaxS, 2);
        vMaxL = _mm_slli_si128(vMaxL, 2);
    }

    if (_mm_movemask_epi8(_mm_or_si128(
            _mm_cmplt_epi16(vSaturationCheckMin, vNegLimit),
            _mm_cmpgt_epi16(vSaturationCheckMax, vPosLimit)))) {
        result->flag |= PARASAIL_FLAG_SATURATED;
        score = 0;
        matches = 0;
        similar = 0;
        length = 0;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->stats->matches = matches;
    result->stats->similar = similar;
    result->stats->length = length;
    result->flag |= PARASAIL_FLAG_NW | PARASAIL_FLAG_DIAG
        | PARASAIL_FLAG_STATS
        | PARASAIL_FLAG_BITS_16 | PARASAIL_FLAG_LANES_8;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(_FL_pr);
    parasail_free(_FS_pr);
    parasail_free(_FM_pr);
    parasail_free(_F_pr);
    parasail_free(_HL_pr);
    parasail_free(_HS_pr);
    parasail_free(_HM_pr);
    parasail_free(_H_pr);
    parasail_free(s2B);
    parasail_free(s1);

    return result;
}


