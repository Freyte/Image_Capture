//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_seule_images_3_bandes_2023_03_22.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 14-Jun-2023 17:07:38
//

// Include Files
#include "test_seule_images_3_bandes_2023_03_22.h"
#include "coder_array.h"
#include "libmwjpegreader.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void test_seule_images_3_bandes_2023_03_22()
{
  static const double outDims[3]{730.0, 1460.0, 3.0};
  static const char fname[6]{"R.jpg"};
  static unsigned char out[3197400];
  coder::array<unsigned char, 3U> r;
  double libjpegMsgCode;
  char libjpegWarnBuffer[200];
  signed char errWarnType;
  signed char fileStatus;
  signed char libjpegReadDone;
  for (int i{0}; i < 200; i++) {
    libjpegWarnBuffer[i] = ' ';
  }
  jpegreader_uint8(&fname[0], &out[0], &outDims[0], 3.0, &fileStatus,
                   &libjpegReadDone, &libjpegMsgCode, &libjpegWarnBuffer[0],
                   &errWarnType);
  if ((fileStatus == -1) || (libjpegReadDone == 0) || (errWarnType == -1)) {
    r.set_size(0, 0, 0);
  } else {
    r.set_size(730, 1460, 3);
    for (int i{0}; i < 3197400; i++) {
      r[i] = out[i];
    }
  }
  // Hist_Hist_H_Bandes_2=ceil(Hist_H_Bandes_2/max(Hist_H_Bandes_2)*64);
  // Taux_Bul_H = round(Taux_Bul_H/2)*20;% variation de 20%
  //  variation de 10%
  // Taux_Bul_B = round(Taux_Bul_B/1)*20;% variation de 20%
  //  variation de 10%
  // disp_str = [filename,' - B_Max_diff_Hist: ',num2str(B_Max_diff_Hist),'
  // B_Min_diff_Hist: ',num2str(B_Max_diff_Hist+B_Min_diff_Hist-1)];
  // disp(disp_str)
  // imshow(double(I_init)/levels);
  // imshow(I_init,map);
  // filename2=filename; filename2(9)=' '; filename2(18)=' ';
  // disp_str = [filename,' - ind_max: ',num2str(ind_max),' Max_Hist_H:
  // ',num2str(Max_Hist_H)]; disp(disp_str) text(x,50, filename2 ,
  // 'FontSize',Tail_front,'Color',col)
}

//
// File trailer for test_seule_images_3_bandes_2023_03_22.cpp
//
// [EOF]
//
