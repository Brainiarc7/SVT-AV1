/*
* Copyright(c) 2019 Intel Corporation
* SPDX - License - Identifier: BSD - 2 - Clause - Patent
*/

#ifndef EbMotionEstimationLcuResults_h
#define EbMotionEstimationLcuResults_h

#include "EbDefinitions.h"
#include "EbDefinitions.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ME_PU_COUNT          209           // Sum of all the possible partitions which have both deminsions greater than 4.

#define ME_RES_CAND_MRP_MODE_0   23            // [Single Ref = 7] + [BiDir = 12 = 3*4 ] + [UniDir = 4 = 3+1]
#define ME_MV_MRP_MODE_0          7            // [7 = 4+3]

#define ME_RES_CAND_MRP_MODE_1   10            // [Single Ref = 4] + [UniDir = 4 = 2*2] + [UniDir = 2 = 1+1]
#define ME_MV_MRP_MODE_1          4            // [4 = 2+2]

#define MAX_SS_ME_PU_COUNT       (849 * 4 + 5) // Sum of all the possible partitions which have both deminsions greater or equal to 4.

    // i.e. no 4x4, 8x4, or 4x8 partitions
#define SQUARE_PU_COUNT           85
    typedef struct MeCandidate
    {
        unsigned    distortion : 20;     // 20-bits holds maximum SAD of 64x64 PU
        unsigned    direction  : 2;
        unsigned    ref_idx_l0 : 2;      // allows for up to 4 references
        unsigned    ref_idx_l1 : 2;
        unsigned    ref0_list : 1;
        unsigned    ref1_list : 1;
    } MeCandidate;
    typedef struct MvCandidate
    {
        signed short      x_mv;
        signed short      y_mv;
    } MvCandidate;
    // move this to a new file with ctor & dtor
    typedef struct MeLcuResults
    {
        uint32_t          lcu_distortion;
        uint8_t          *total_me_candidate_index;
        MeCandidate     **me_candidate;
        MeCandidate      *me_candidate_array;
        uint8_t          *me_nsq_0; // 2 Number of reference lists
        uint8_t          *me_nsq_1; // 2 Number of reference lists

        MvCandidate    **me_mv_array;
        // [PU][LAST, LAST2, LAST3, GOLD, BWD, ALT2, ALT] if MRP Mode 0,
        // [PU][LAST, LAST2, BWD, ALT2] if MRP Mode 1,
    } MeLcuResults;
#ifdef __cplusplus
}
#endif
#endif // EbMotionEstimationLcuResults_h
