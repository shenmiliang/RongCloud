#ifndef DEEP_TOOLKIT_MODELS_H
#define DEEP_TOOLKIT_MODELS_H

#include "toolkit/onnx/core/onnx_core.h"
#include "toolkit/onnx/core/onnx_utils.h"
#include "toolkit/onnx/cv/glint_arcface.h"
#include "toolkit/onnx/cv/mobile_facenet.h"
#include "toolkit/onnx/cv/rvm.h"
#include "toolkit/onnx/cv/scrfd.h"

namespace toolkit
{
    namespace onnx
    {
  
        typedef onnx_cv::GlintArcFace _GlintArcFace;
        typedef onnx_cv::MobileFaceNet _MobileFaceNet;
        typedef onnx_cv::SCRFD _SCRFD;
        typedef onnx_cv::RobustVideoMatting _RobustVideoMatting;
        
    
        namespace face
        {
           namespace detect
           {
                typedef _SCRFD SCRFD;
           } // namespace detect
           
        }   
       
        namespace faceid
        {
            typedef _GlintArcFace GlintArcFace;
            typedef _MobileFaceNet MobileFaceNet;
        }

        namespace matting
        {
            typedef _RobustVideoMatting RobustVideoMatting;
        } // namespace matting
        

    }
}

#endif