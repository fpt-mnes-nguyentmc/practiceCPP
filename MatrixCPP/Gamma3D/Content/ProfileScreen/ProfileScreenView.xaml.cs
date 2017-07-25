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

namespace Gamma3D.Content.Controls
{
    /// <summary>
    /// Interaction logic for ProfileScreenView.xaml
    /// </summary>
    public partial class ProfileScreenView : UserControl
    {
        #region Variables
      
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
        public ProfileScreenView()
        {
            InitializeComponent();
        }
        #endregion
        
        #region Events Hanlder
        /// <summary>
        /// Handles the Click event of the btnApply_ProfileScreen control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnApply_ProfileScreen_Click(object sender, RoutedEventArgs e)
        {
            checkInputData_ProfileScreen();
        }

        /// <summary>
        /// Handles the Click event of the btnNext_ProfileScreen control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnNext_ProfileScreen_Click(object sender, RoutedEventArgs e)
        {
            checkInputData_ProfileScreen();
        }
        #endregion        

        #region Properties Changed
        #endregion

        #region Functions
        /// <summary>
        /// Check Input Data in Profile Screen
        /// </summary>
        /// <returns></returns>
        private bool checkInputData_ProfileScreen()
        {
            //if (false == this.Logic.isImportVolFile)
            //{
            //    MessageBox.Show("Two volume files have not been imported!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            //    return false;
            //}

            if (string.IsNullOrWhiteSpace(this.txtXValue.Text) || string.IsNullOrWhiteSpace(this.txtYValue.Text) || string.IsNullOrWhiteSpace(this.txtYValue.Text)
                || string.IsNullOrWhiteSpace(this.txtNormalizationFactor.Text))
            {
                MessageBox.Show("Input Data must not be empty!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (false == int.TryParse(this.txtXValue.Text.Trim(), out this.Logic.iXValue))
            {
                MessageBox.Show("X value must be Integer value!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (false == int.TryParse(this.txtYValue.Text.Trim(), out this.Logic.iYValue))
            {
                MessageBox.Show("Y value must be Integer value!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (false == int.TryParse(this.txtZValue.Text.Trim(), out this.Logic.iZValue))
            {
                MessageBox.Show("Z value must be Integer value!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (false == double.TryParse(this.txtNormalizationFactor.Text.Trim(), out this.Logic.dNormalization))
            {
                MessageBox.Show("Normalization Factor must be Double value!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            return true;
        }
        #endregion
    }
}
