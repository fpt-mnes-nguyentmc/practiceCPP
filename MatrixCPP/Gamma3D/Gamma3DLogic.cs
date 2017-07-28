using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gamma3DWrapper;

namespace Gamma3D.Content
{
    public class Gamma3DLogic
    {
        #region Variables
        /// <summary>
        /// Two Vol files have already been imported or not yet
        /// </summary>
        public bool isImportVolFile = false;

        /// <summary>
        /// 
        /// </summary>
        public bool isCheckDataProfileScreen = false;

        /// <summary>
        /// X value
        /// </summary>
        public int iXValue;

        /// <summary>
        /// Y value
        /// </summary>
        public int iYValue;

        /// <summary>
        /// Z value
        /// </summary>
        public int iZValue;

        /// <summary>
        /// Normalization Factor
        /// </summary>
        public double dNormalization;

        /// <summary>
        /// 
        /// </summary>
        public int tabCurrent = 0;

        #endregion

        #region Properties
        #endregion

        #region Unmanaged Wrapper
        /// <summary>
        /// The wrapper
        /// </summary>
        private CGamma3DWrapper wrapper = new CGamma3DWrapper();
        #endregion

        #region Initilize
        /// <summary>
        /// Initializes a new instance of the <see cref="Gamma3DLogic"/> class.
        /// </summary>
        public Gamma3DLogic()
        {
        }
        #endregion

        #region Events Hanlder
        #endregion

        #region Properties Changed
        #endregion

        #region Functions
        /// <summary>
        /// Sets the window handle.
        /// </summary>
        /// <param name="pHwnd">The p HWND.</param>
        public void SetImageHostHandle(IntPtr pHwnd)
        {
            this.wrapper.SetImageHostHandle(pHwnd);
        }

        /// <summary>
        /// Extracts the volume file.
        /// </summary>
        /// <param name="evalFilePath">The eval file path.</param>
        /// <param name="refFlePath">The reference fle path.</param>
        public void ExtractVolumeFile(string evalFilePath, string refFlePath)
        {
            this.wrapper.ExtractGammaVolume(evalFilePath, refFlePath);
        }

        /// <summary>
        /// Switches the tab.
        /// </summary>
        /// <param name="index">The index.</param>
        public void SwitchTab(int index)
        {
            
        }

        /// <summary>
        /// Inputs the gamma screen.
        /// </summary>
        public void SetGammaInputParam(GammaPlaneEnum plane, int posPlan, double maxDeltaDose,
            double maxDeltaPos, double radius, double thresholdDoseMin)
        {
            var gammaModel = new GammaInputModel();

            gammaModel.Plane = plane;
            gammaModel.PosPlan = posPlan;
            gammaModel.MaxDeltaDose = maxDeltaDose;
            gammaModel.MaxDeltaPos = maxDeltaPos;
            gammaModel.Radius = radius;
            gammaModel.ThresholdDoseMin = thresholdDoseMin;

            this.wrapper.SetGammaInputParam(gammaModel);
        }

        /// <summary>
        /// Determines the gamma plane.
        /// </summary>
        public void DetermineGammaPlane()
        {
            this.wrapper.DetermineGammaPlane();
        }

        /// <summary>
        /// Extract Gamma Volume To Axis.
        /// </summary>
        public void ExtractGammaVolumeToAxis(Int32 iXAxis, Int32 iYAxis, Int32 iZAxis, Double dNormalize)
        {
            this.wrapper.ExtractGammaVolumeToAxis(iXAxis, iYAxis, iZAxis, dNormalize);
        }
        #endregion
    }
}
