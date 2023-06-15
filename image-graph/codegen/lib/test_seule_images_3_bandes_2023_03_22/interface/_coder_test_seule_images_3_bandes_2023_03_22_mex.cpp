//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_test_seule_images_3_bandes_2023_03_22_mex.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 14-Jun-2023 17:07:38
//

// Include Files
#include "_coder_test_seule_images_3_bandes_2023_03_22_mex.h"
#include "_coder_test_seule_images_3_bandes_2023_03_22_api.h"

// Function Definitions
//
// Arguments    : int32_T nlhs
//                mxArray *plhs[]
//                int32_T nrhs
//                const mxArray *prhs[]
// Return Type  : void
//
void mexFunction(int32_T nlhs, mxArray *[], int32_T nrhs, const mxArray *[])
{
  mexAtExit(&test_seule_images_3_bandes_2023_03_22_atexit);
  // Module initialization.
  test_seule_images_3_bandes_2023_03_22_initialize();
  // Dispatch the entry-point.
  unsafe_test_seule_images_3_bandes_2023_03_22_mexFunction(nlhs, nrhs);
  // Module termination.
  test_seule_images_3_bandes_2023_03_22_terminate();
}

//
// Arguments    : void
// Return Type  : emlrtCTX
//
emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           nullptr, (const char_T *)"windows-1252", true);
  return emlrtRootTLSGlobal;
}

//
// Arguments    : int32_T nlhs
//                int32_T nrhs
// Return Type  : void
//
void unsafe_test_seule_images_3_bandes_2023_03_22_mexFunction(int32_T nlhs,
                                                              int32_T nrhs)
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 0, 4,
                        37, "test_seule_images_3_bandes_2023_03_22");
  }
  if (nlhs > 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 37,
                        "test_seule_images_3_bandes_2023_03_22");
  }
  // Call the function.
  test_seule_images_3_bandes_2023_03_22_api();
}

//
// File trailer for _coder_test_seule_images_3_bandes_2023_03_22_mex.cpp
//
// [EOF]
//
