/*******************************************************************************
* Copyright (C) 2010 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the 'License');
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an 'AS IS' BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions
* and limitations under the License.
* 
*******************************************************************************/

/*
//
//  Purpose:
//     Intel(R) Integrated Performance Primitives. Cryptography Primitives.
//     EC over GF(p) Operations
//
//     Context:
//        ippsGFpECTstPointInSubgroup()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpgfpecstuff.h"
#include "pcphash.h"
#include "pcphash_rmf.h"


/*F*
// Name: ippsGFpECTstPointInSubgroup
//
// Purpose: Checks if a point belongs to a specified subgroup
//
// Returns:                   Reason:
//    ippStsNullPtrErr               pP == NULL
//                                   pEC == NULL
//                                   pResult == NULL
/                                    pScratchBuffer == NULL
//
//    ippStsContextMatchErr          invalid pEC->idCtx
//                                   pEC->subgroup == NULL
//                                   invalid pP->idCtx
//
//    ippStsOutOfRangeErr            ECP_POINT_FELEN(pP)!=GFP_FELEN()
//
//    ippStsNoErr                    no error
//
// Parameters:
//    pP              Pointer to the IppsGFpECPoint context
//    pEC             Pointer to the context of the elliptic curve
//    pResult         Pointer to the result of the check
//    pScratchBuffer  Pointer to the scratch buffer
//
*F*/
IPPFUN(IppStatus, ippsGFpECTstPointInSubgroup,(const IppsGFpECPoint* pP,
                                               IppECResult* pResult,
                                               IppsGFpECState* pEC,
                                               Ipp8u* pScratchBuffer))
{
   IPP_BAD_PTR4_RET(pP, pResult, pEC, pScratchBuffer);
   IPP_BADARG_RET( !VALID_ECP_ID(pEC), ippStsContextMatchErr );
   IPP_BADARG_RET(!ECP_SUBGROUP(pEC), ippStsContextMatchErr);
   IPP_BADARG_RET( !ECP_POINT_VALID_ID(pP), ippStsContextMatchErr );

   IPP_BADARG_RET( ECP_POINT_FELEN(pP)!=GFP_FELEN(GFP_PMA(ECP_GFP(pEC))), ippStsOutOfRangeErr);

   {
      IppECResult tstResult;
      ippsGFpECTstPoint(pP, &tstResult, pEC);

      if(ippECValid==tstResult) {
         IppsGFpECPoint T;
         cpEcGFpInitPoint(&T, cpEcGFpGetPool(1, pEC),0, pEC);

         gfec_MulPoint(&T, pP, MOD_MODULUS(ECP_MONT_R(pEC)), BITS_BNU_CHUNK(ECP_ORDBITSIZE(pEC)), /*0,*/ pEC, pScratchBuffer);
         tstResult = gfec_IsPointAtInfinity(&T)? ippECValid : ippECPointOutOfGroup;

         cpEcGFpReleasePool(1, pEC);
      }
      *pResult = tstResult;

      return ippStsNoErr;
   }
}
