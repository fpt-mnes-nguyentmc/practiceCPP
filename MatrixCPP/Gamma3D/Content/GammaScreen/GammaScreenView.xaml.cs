using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Gamma3DWrapper;

namespace Gamma3D.Content.Controls
{
    /// <summary>
    /// Interaction logic for GammaScreenView.xaml
    /// </summary>
    public partial class GammaScreenView : UserControl
    {
        #region Variables
        /// <summary>
        /// Z value or Y value or X value depend on choosing plane (Axial, Coronal, Sagital)
        /// </summary>
        private int iValue;
        #endregion

        #region Properties

        /// <summary>
        /// Gets the logic.
        /// </summary>
        /// <value>
        /// The logic.
        /// </value>
        public Gamma3DLogic Logic
        {
            get
            {
                return this.DataContext as Gamma3DLogic;
            }
        }

        #endregion

        #region Initilize

        /// <summary>
        /// Initializes a new instance of the <see cref="ProfileScreenView"/> class.
        /// </summary>
        public GammaScreenView()
        {
            InitializeComponent();
        }

        #endregion       

        #region Events Hanlder
        /// <summary>
        /// Handles the Click event of the btnApply_GammaScreen control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnApply_Click(object sender, RoutedEventArgs e)
        {
            var plan = GammaPlaneEnum.Axial;
            int posPlan = 0;
            double maxDeltaDose = 0;
            double maxDeltaPos = 0;
            double radius = 0;
            double thresholdDoseMin = 0;

            if (int.TryParse(this.txtPositionValue.Text.Trim(), out posPlan) == false)
            {
                // TODO: Abnormal case
            }

            if (double.TryParse(this.txtLargestDoseTolerance.Text.Trim(), out maxDeltaDose) == false)
            {
                // TODO: Abnormal case
            }

            if (double.TryParse(this.txtLargestPositionTolerance.Text.Trim(), out maxDeltaPos) == false)
            {
                // TODO: Abnormal case
            }

            if (double.TryParse(this.txtRadius.Text.Trim(), out radius) == false)
            {
                // TODO: Abnormal case
            }

            if (double.TryParse(this.txtThresholdDose.Text.Trim(), out thresholdDoseMin) == false)
            {
                // TODO: Abnormal case
            }

            this.Logic.SetGammaInputParam(plan, posPlan, maxDeltaDose, maxDeltaPos, radius, thresholdDoseMin);
        }

        /// <summary>
        /// Handles the Click event of the radioAxial control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radioAxial_Click(object sender, RoutedEventArgs e)
        {
            this.labelPosition.Content = "Z";
        }

        /// <summary>
        /// Handles the Click event of the radioCoronal control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radioCoronal_Click(object sender, RoutedEventArgs e)
        {
            this.labelPosition.Content = "Y";
        }

        /// <summary>
        /// Handles the Click event of the radioSagital control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radioSagital_Click(object sender, RoutedEventArgs e)
        {
            this.labelPosition.Content = "X";
        }
        
        #endregion

        #region Properties Changed
        #endregion

        #region Functions
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public bool checkInputData_GammaScreen()
        {
            return true;
        }
        #endregion        
    }
}
