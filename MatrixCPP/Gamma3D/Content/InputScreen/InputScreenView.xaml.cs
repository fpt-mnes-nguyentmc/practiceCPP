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
    /// Interaction logic for InputScreenView.xaml
    /// </summary>
    public partial class InputScreenView : UserControl
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
        public InputScreenView()
        {
            InitializeComponent();
        }
        #endregion          

        #region Events Hanlder
        /// <summary>
        /// Browse and select Evaluation VOL File
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnBrowseEvalFile_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog();

            // Default title
            dlg.Title = "Open Evaluation VOL File";
            // Default file name
            dlg.FileName = "Volume file";
            // Default file extension 
            dlg.DefaultExt = ".vol";
            // Filter files by extension
            dlg.Filter = "Volume file (.vol)|*.vol";

            // Process open file dialog box results 
            if (true == dlg.ShowDialog())
            {
                // Assign file path to txtEvaluationVOL
                this.txtEvalFile.Text = dlg.FileName.Trim();
            }
        }

        /// <summary>
        /// Browse and select Reference VOL File
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnBrowseRefFile_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog();

            // Default title
            dlg.Title = "Open Reference VOL File";
            // Default file name
            dlg.FileName = "Volume file";
            // Default file extension 
            dlg.DefaultExt = ".vol";
            // Filter files by extension
            dlg.Filter = "Volume file (.vol)|*.vol";

            // Process open file dialog box results 
            if (true == dlg.ShowDialog())
            {
                // Assign file path to txtReferenceVOL
                this.txtRefFile.Text = dlg.FileName.Trim();
            }
        }

        /// <summary>
        /// Handles the Click event of the btnNext_InputScreen control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="RoutedEventArgs"/> instance containing the event data.</param>
        private void btnNext_InputScreen_Click(object sender, RoutedEventArgs e)
        {
            this.Logic.isImportVolFile = checkImportVolFile();

            //this.Logic.ExtractVolumeFile(this.txtEvalFile.Text.Trim(), this.txtRefFile.Text.Trim());
        }

        #endregion               

        #region Properties Changed
        #endregion

        #region Functions
        /// <summary>
        /// Check that two Vol files have already been imported or not yet
        /// </summary>
        /// <returns></returns>
        private bool checkImportVolFile()
        {
            if (string.IsNullOrWhiteSpace(this.txtEvalFile.Text) || string.IsNullOrWhiteSpace(this.txtRefFile.Text))
            {
                MessageBox.Show("File Path must not be empty!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            if (System.IO.File.Exists(this.txtEvalFile.Text) && System.IO.Directory.Exists(this.txtRefFile.Text))
            {
                MessageBox.Show("File Path must exist!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }

            return true;
        }
        #endregion
    }
}
