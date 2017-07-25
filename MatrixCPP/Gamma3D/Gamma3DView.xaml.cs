using System.Windows;
using System.Windows.Forms;
using System.Windows.Interop;

namespace Gamma3D.Content
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Gamma3DView : Window
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
        /// Initializes a new instance of the <see cref="Gamma3DView"/> class.
        /// </summary>
        public Gamma3DView()
        {
            InitializeComponent();
        }
        #endregion

        #region Events Hanlder
        
        #endregion

        #region Properties Changed
        #endregion

        #region Override Functions

        /// <summary>
        /// When overridden in a derived class, is invoked whenever application code or internal processes call <see cref="M:System.Windows.FrameworkElement.ApplyTemplate" />.
        /// </summary>
        public override void OnApplyTemplate()
        {
            // Get handle main window
            var windowHwnd = (new WindowInteropHelper(this)).Handle;

            // Set up handle source
            var hwndSource = HwndSource.FromHwnd(windowHwnd);

            // Tracking main window handle
            Common.Variables.WindowHandle = hwndSource.Handle;

            base.OnApplyTemplate();
        }

        #endregion        

        #region Functions

        
        #endregion
    }
}
