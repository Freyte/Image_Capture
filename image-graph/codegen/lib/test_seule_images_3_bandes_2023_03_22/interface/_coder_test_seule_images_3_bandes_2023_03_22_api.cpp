//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_test_seule_images_3_bandes_2023_03_22_api.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 14-Jun-2023 17:07:38
//

// Include Files
#include "_coder_test_seule_images_3_bandes_2023_03_22_api.h"
#include "_coder_test_seule_images_3_bandes_2023_03_22_mex.h"

// Variable Definitions
emlrtCTX emlrtRootTLSGlobal{nullptr};

emlrtContext emlrtContextGlobal{
    true,                                                 // bFirstTime
    false,                                                // bInitialized
    131626U,                                              // fVersionInfo
    nullptr,                                              // fErrorFunction
    "test_seule_images_3_bandes_2023_03_22",              // fFunctionName
    nullptr,                                              // fRTCallStack
    false,                                                // bDebugMode
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, // fSigWrd
    nullptr                                               // fSigMem
};

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void test_seule_images_3_bandes_2023_03_22_api()
{
  // Invoke the target function
  test_seule_images_3_bandes_2023_03_22();
}

//
// Arguments    : void
// Return Type  : void
//
void test_seule_images_3_bandes_2023_03_22_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  test_seule_images_3_bandes_2023_03_22_xil_terminate();
  test_seule_images_3_bandes_2023_03_22_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void test_seule_images_3_bandes_2023_03_22_initialize()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void test_seule_images_3_bandes_2023_03_22_terminate()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

//
// File trailer for _coder_test_seule_images_3_bandes_2023_03_22_api.cpp
//
// [EOF]
//
