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
        /// Extracts the volume file.
        /// </summary>
        /// <param name="evalFilePath">The eval file path.</param>
        /// <param name="refFlePath">The reference fle path.</param>
        public void ExtractVolumeFile(string evalFilePath, string refFlePath)
        {
            wrapper.ExtractGammaVolume(evalFilePath, refFlePath);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        public void SwitchTab(int index)
        {
            
        }
        #endregion
    }
}
