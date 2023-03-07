/* Microsoft Reference Implementation for TPM 2.0
 *
 *  The copyright in this software is being made available under the BSD License,
 *  included below. This software may be subject to other third party and
 *  contributor rights, including patent rights, and no such rights are granted
 *  under this license.
 *
 *  Copyright (c) Microsoft Corporation
 *
 *  All rights reserved.
 *
 *  BSD License
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *  Redistributions of source code must retain the above copyright notice, this list
 *  of conditions and the following disclaimer.
 *
 *  Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ""AS IS""
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "Tpm.h"
#include "PCR_Reset_fp.h"

#if CC_PCR_Reset  // Conditional expansion of this file

/*(See part 3 specification)
// Reset PCR
*/
//  Return Type: TPM_RC
//      TPM_RC_LOCALITY             current command locality is not allowed to
//                                  reset the PCR referenced by 'pcrHandle'
TPM_RC
TPM2_PCR_Reset(
    PCR_Reset_In    *in             // IN: input parameter list
    )
{
// Input Validation

    // Check if the reset operation is allowed by the current command locality
    if(!PCRIsResetAllowed(in->pcrHandle))
        return TPM_RC_LOCALITY;

    // If PCR is state saved and we need to update orderlyState, check NV
    // availability
    if(PCRIsStateSaved(in->pcrHandle))
        RETURN_IF_ORDERLY;

// Internal Data Update

    // Reset selected PCR in all banks to 0
    PCRSetValue(in->pcrHandle, 0);

    // Indicate that the PCR changed so that pcrCounter will be incremented if
    // necessary.
    PCRChanged(in->pcrHandle);

    return TPM_RC_SUCCESS;
}

#endif // CC_PCR_Reset