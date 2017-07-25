using System.Windows;

using gdcm;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using DicomExtractor.Model;
using System.Text;
using System.Linq;

namespace DicomExtractor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// <seealso cref="System.Windows.Window"/>
    public partial class MainWindow : Window
    {
        #region Variables

        /// <summary>
        /// The Treatment Session Ion Beam Sequence tag: (3008,0021)
        /// </summary>
        private readonly Tag beamSequeceSessionTag = new Tag(0x3008, 0x0021);

        /// <summary>
        /// The Ion Control Point Delivery Sequence tag: (3008,0041)
        /// </summary>
        private readonly Tag IonCPSequenceTag = new Tag(0x3008, 0x0041);

        /// <summary>
        /// The Scan Spot Metersets Delivered tag
        /// </summary>
        private readonly Tag scanSpotMeterTag = new Tag(0x3008, 0x0047);

        /// <summary>
        /// The Nominal Beam Energy tag
        /// </summary>
        private readonly Tag nomBeamEnergyTag = new Tag(0x300A, 0x0114);

        /// <summary>
        /// The Scan Spot Position Map tag
        /// </summary>
        private readonly Tag spotPositionTag = new Tag(0x300A, 0x0394);

        /// <summary>
        /// The flag to Cancel process
        /// </summary>
        private bool isCancel = false;

        /// <summary>
        /// The flag to Pause process
        /// </summary>
        private bool isPause = false;

        /// <summary>
        /// Constannt c
        /// </summary>
        private uint c = 150000;

        #endregion

        #region Properties
        #endregion

        #region Initilize
        /// <summary>
        /// Initializes a new instance of the <see cref="MainWindow"/> class.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();

            // Initialize control

            // Set default value for progress bar
            this.progressBar.Value = 0;

            // Disable Cancel button
            this.btnCancel.IsEnabled = false;


        }
        #endregion

        #region Events Hanlder
        /// <summary>
        /// Click button Browse Folder to Import DICOM
        /// </summary>
        ///  <param></param>       
        /// <returns></returns>
        private void btnImportDICOM_Click(object sender, RoutedEventArgs e)
        {
            var browserImportDICOM = new System.Windows.Forms.FolderBrowserDialog();
            var dialogResult = browserImportDICOM.ShowDialog();

            if (dialogResult == System.Windows.Forms.DialogResult.OK)
            {
                this.txtFolderDICOM.Text = browserImportDICOM.SelectedPath;
            }
        }

        /// <summary>
        /// Click button Browse Folder to Export CSV
        /// </summary>
        ///  <param></param>       
        /// <returns></returns>
        private void btnExportCSV_Click(object sender, RoutedEventArgs e)
        {
            var browserExportCSV = new System.Windows.Forms.FolderBrowserDialog();
            var dialogResult = browserExportCSV.ShowDialog();

            if (dialogResult == System.Windows.Forms.DialogResult.OK)
            {
                this.txtFolderCSV.Text = browserExportCSV.SelectedPath;
            }
        }

        /// <summary>
        /// Handles the Click event of the btnRun control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="RoutedEventArgs" /> instance containing the event data.</param>
        private void btnRun_Click(object sender, RoutedEventArgs e)
        {
            var dcmPath = this.txtFolderDICOM.Text.Trim();
            var csvPath = this.txtFolderCSV.Text.Trim();
            var dcmFiles = new List<string>();
            bool isSuccess = true;

            if (string.IsNullOrWhiteSpace(dcmPath) || string.IsNullOrWhiteSpace(csvPath))
            {
                MessageBox.Show("File/Folder Directory must not be empty!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if ((System.IO.File.Exists(dcmPath) == false && System.IO.Directory.Exists(dcmPath) == false) ||
                 System.IO.Directory.Exists(csvPath) == false)
            {
                MessageBox.Show("File/Folder does not exist!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            if (System.IO.File.Exists(dcmPath))
            {
                // Handle when dcmPath is a path of 1 file
                dcmFiles.Add(dcmPath);
            }
            else if (System.IO.Directory.Exists(dcmPath))
            {
                // Note: Processing top directory only files
                dcmFiles.AddRange(System.IO.Directory.GetFiles(dcmPath, "*", System.IO.SearchOption.TopDirectoryOnly));
            }

            // Nothing to do case:
            if (dcmFiles == null || dcmFiles.Count == 0)
            {
                MessageBox.Show("Do not have any file!", "Information", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }

            // Reset progress bar
            this.progressBar.Value = 0;
            this.txtFileCount.Text = string.Format("{0} File(s)", dcmFiles.Count.ToString());

            var step = (1.0 / dcmFiles.Count);

            // Enable Cancel button
            this.btnCancel.IsEnabled = true;
            this.isCancel = false;
            this.isPause = false;

            // Fire a thread to extract dicom file
            ThreadPool.QueueUserWorkItem(thread =>
            {
                try
                {
                    // Using parallel to boost performance here
                    //foreach(string file in dcmFiles)
                    Parallel.ForEach(dcmFiles, file =>                    
                    {
                        // Sleep at Palallel when showing dialog box
                        while (this.isPause)
                        {
                            Thread.Sleep(100);
                        }

                        // Exit Parallel if choose Yes on Cancel dialog
                        if (this.isCancel)
                        {
                            return;
                        }

                        try
                        {
                            // Extract dicom file
                            var result = ExtractDicomFile(file);

                            // Get file name from path
                            string fileName = System.IO.Path.GetFileName(file);

                            // Export to CSV SPOT file
                            ExportCsvSpotFile(result, fileName, csvPath);

                            // Export to CSV CP file
                            ExportCsvCpFile(result, fileName, csvPath);

                            // Export to mac file
                            ExportMacFile(result, fileName, csvPath);
                        }
                        catch (NullReferenceException ex)
                        {
                            // Sleep to display a single dialog at a time
                            while (this.isPause)
                            {
                                Thread.Sleep(100);
                            }

                            // Turn on isPause to display a single dialog at a time
                            this.isPause = true;

                            // Exit Parallel if choose isCancel = true
                            if (this.isCancel)
                            {
                                this.isPause = false;
                                return;
                            }                          

                            var dlgResult = new System.Windows.MessageBoxResult();

                            System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                            {
                                dlgResult = MessageBox.Show(this, string.Format("{0}\nDo you want to skip this file and continue processing?", ex.Message), "Confirm", MessageBoxButton.YesNo, MessageBoxImage.Warning);
                            }), System.Windows.Threading.DispatcherPriority.Send, null);

                            if (dlgResult == System.Windows.MessageBoxResult.Yes)
                            {
                                this.isPause = false;

                                //// Update percentage woking
                                //System.Windows.Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                //{
                                //    this.progressBar.Value += step;
                                //}), System.Windows.Threading.DispatcherPriority.Send, null);                              
                            }
                            else
                            {                                
                                this.isCancel = true;
                                this.isPause = false;
                                isSuccess = false;
                                return;
                            }                            
                        }
                        catch (Exception ex)
                        {
                            System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                            {
                                MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                            }), System.Windows.Threading.DispatcherPriority.Send, null);  

                            this.isPause = false;
                            this.isCancel = true;
                            isSuccess = false;

                            return;
                        }

                        // Update percentage woking
                        System.Windows.Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            this.progressBar.Value += step;
                        }), System.Windows.Threading.DispatcherPriority.Send, null);
                    //}
                    });

                    GC.Collect();
                }
                catch (Exception ex)
                {
                    System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                    {
                        MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }), System.Windows.Threading.DispatcherPriority.Send, null);   
                    
                    isSuccess = false;
                }

                // Disabl Cancel button after finishing process
                System.Windows.Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                {
                    this.btnCancel.IsEnabled = false;

                    if (!isSuccess)
                    {
                        this.progressBar.Value = 0;
                        this.txtFileCount.Text = string.Empty;                        
                    }
                }), System.Windows.Threading.DispatcherPriority.Send, null);
            });
        }

        /// <summary>
        /// Handles the Click event of the btnCancel control.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.isPause = true;

            var dlgResult = MessageBox.Show("Do you want to cancel process?", "Cancel Process", MessageBoxButton.YesNo, MessageBoxImage.Question);

            if (dlgResult == System.Windows.MessageBoxResult.Yes)
            {
                this.isPause = false;
                this.isCancel = true;

                this.progressBar.Value = 0;
                this.txtFileCount.Text = string.Empty;

                this.btnCancel.IsEnabled = false;
            }
            else
            {
                this.isPause = false;
            }
        }
        #endregion

        #region Properties Changed
        #endregion

        #region Functions

        /// <summary>
        /// Extracts the dicom file.
        /// </summary>
        /// <param name="path">The path.</param>
        private List<ExtractorObject> ExtractDicomFile(string path)
        {
            List<ExtractorObject> extractorObjs = null;

            // Initialize reader to read dicom file
            var reader = new Reader();

            // Set path for GDCM lib
            reader.SetFileName(path);

            // Start to read it
            if (reader.Read())
            {
                // Initialize DataSet of the dicom file
                var dcmFile = reader.GetFile();
                var dataset = dcmFile.GetDataSet();

                // Check Treatment Session Ion Beam Sequence  tag
                if (dataset.FindDataElement(beamSequeceSessionTag))
                {
                    var beamElement = dataset.GetDataElement(beamSequeceSessionTag);
                    var beamVR = DataSetHelper.ComputeVR(dcmFile, dataset, beamElement.GetTag());

                    // Check VR value of Treatment Session Ion Beam Sequence (3008,0021) is [SQ] type
                    if (beamVR.Compatible(new VR(VR.VRType.SQ)))
                    {
                        var beamSQ = beamElement.GetValueAsSQ();

                        extractorObjs = new List<ExtractorObject>();

                        // Loop all SQ items
                        for (uint i = 1, rLength = beamSQ.GetNumberOfItems(); i <= rLength; i++)
                        {
                            var beamDS = beamSQ.GetItem(i).GetNestedDataSet();  // item starts at 1, not 0

                            // Check Ion Control Point Sequence (3008,0041) tag
                            if (beamDS.FindDataElement(IonCPSequenceTag))
                            {
                                // Get Element & VR value
                                var ionCPElement = beamDS.GetDataElement(IonCPSequenceTag);
                                var ionCPVR = DataSetHelper.ComputeVR(dcmFile, beamDS, ionCPElement.GetTag());

                                // Check VR value of Ion Control Point Sequence (3008,0041) is [SQ] type
                                if (ionCPVR.Compatible(new VR(VR.VRType.SQ)))
                                {
                                    // Get Sequence item of Ion Control Point Squence tag
                                    var ionCPSQ = ionCPElement.GetValueAsSQ();

                                    for (uint j = 1, bLength = ionCPSQ.GetNumberOfItems(); j <= bLength; j++)
                                    {
                                        var ionCPDS = ionCPSQ.GetItem(j).GetNestedDataSet();  // item starts at 1, not 0
                                        var extractorObj = new ExtractorObject();

                                        // The Scan Spot Metersets Delivered tag case:
                                        if (ionCPDS.FindDataElement(scanSpotMeterTag))
                                        {
                                            // Get Element & VR value
                                            var scanSpotMeterElement = ionCPDS.GetDataElement(scanSpotMeterTag);
                                            var scanSpotMeterVR = DataSetHelper.ComputeVR(dcmFile, ionCPDS, scanSpotMeterElement.GetTag());

                                            // Check VR value of Scan Spot Metersets Delivered (3008,0047) is [FL] type
                                            if (scanSpotMeterVR.Compatible(new VR(VR.VRType.FL)))
                                            {
                                                var scanSpotMeterBytes = new byte[scanSpotMeterElement.GetValue().GetLength().GetValueLength()];

                                                // Get byte array data from tag
                                                scanSpotMeterElement.GetByteValue().GetBuffer(scanSpotMeterBytes, (uint)scanSpotMeterBytes.Length);

                                                // Extract data here
                                                for (int vm = 0, vmLength = scanSpotMeterBytes.Length, offSet = sizeof(float); vm < vmLength; vm += offSet)
                                                {
                                                    extractorObj.SpotMeterList.Add(BitConverter.ToSingle(scanSpotMeterBytes, vm));
                                                }
                                            }
                                            else
                                            {
                                                // Abnormal: Dose tag wrong type                                                         
                                                //continue;
                                                throw new NullReferenceException("Dose tag wrong type in file " + path);
                                            }
                                        }
                                        else
                                        {
                                            // Abnormal: Cannot find the Dose tag
                                            //continue;
                                            throw new NullReferenceException("Cannot find the Dose tag in file " + path);
                                        }

                                        // The Nominal Beam Energy tag case:
                                        if (ionCPDS.FindDataElement(nomBeamEnergyTag))
                                        {
                                            // Get Element & VR value
                                            var nomBeamEnergyElement = ionCPDS.GetDataElement(nomBeamEnergyTag);
                                            var nomBeamEnergyVR = DataSetHelper.ComputeVR(dcmFile, ionCPDS, nomBeamEnergyElement.GetTag());

                                            // Check VR value of Nominal Beam Energy (300a,0114) is [DS] type
                                            if (nomBeamEnergyVR.Compatible(new VR(VR.VRType.DS)))
                                            {
                                                var nomBeamEnergyValue = default(double);

                                                lock (this)
                                                {
                                                    // Extract data here
                                                    if (double.TryParse(nomBeamEnergyElement.GetValue().toString().Trim(), out nomBeamEnergyValue))
                                                    {
                                                        extractorObj.NomBeamEnergy = nomBeamEnergyValue;
                                                    }
                                                    else
                                                    {
                                                        // Abnormal: Cannot get enery value
                                                        //continue;
                                                        throw new NullReferenceException("Cannot get Energy value in file " + path);
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                // Abnormal: Energy tag wrong type
                                                //continue;
                                                throw new NullReferenceException("Energy tag wrong type in file " + path);
                                            }
                                        }
                                        else
                                        {
                                            // Abnormal: Cannot find the Energy tag
                                            //continue;
                                            throw new NullReferenceException("Cannot find the Energy tag in file " + path);
                                        }

                                        // The Scan Spot Position Map tag case:
                                        if (ionCPDS.FindDataElement(spotPositionTag))
                                        {
                                            // Get Element & VR value
                                            var spotElement = ionCPDS.GetDataElement(spotPositionTag);
                                            var spotVR = DataSetHelper.ComputeVR(dcmFile, ionCPDS, spotElement.GetTag());

                                            // Check VR value of Scan Spot Position Map (300a,0394) is [FL] type
                                            if (spotVR.Compatible(new VR(VR.VRType.FL)))
                                            {
                                                var spotBytes = new byte[spotElement.GetValue().GetLength().GetValueLength()];

                                                // Get byte array data from tag
                                                spotElement.GetByteValue().GetBuffer(spotBytes, (uint)spotBytes.Length);

                                                // Extract data here
                                                for (int vm = 0, vmLength = spotBytes.Length, offSet = 2 * sizeof(float); vm < vmLength; vm += offSet)
                                                {
                                                    extractorObj.SpotPosition.Add(new Point
                                                    {
                                                        X = BitConverter.ToSingle(spotBytes, vm),
                                                        Y = BitConverter.ToSingle(spotBytes, vm + sizeof(float))
                                                    });
                                                }
                                            }
                                            else
                                            {
                                                // Abnormal: Spot Position tag wrong type
                                                //continue;
                                                throw new NullReferenceException("Spot Position tag wrong type in file " + path);
                                            }
                                        }
                                        else
                                        {
                                            // Abnormal: Cannot find the Spot Position tag
                                            //continue;
                                            throw new NullReferenceException("Cannot find the Spot Position tag in file" + path);
                                        }

                                        extractorObjs.Add(extractorObj);
                                    }
                                }
                                else
                                {
                                    // Check VR value of Ion Control Point Sequence (3008,0041) is [SQ] type
                                    throw new NullReferenceException("VR value of Ion Control Point Sequence tag (3008,0041) is not [SQ] type in file " + path);
                                }
                            }
                            else 
                            {
                                throw new NullReferenceException("Can not find Ion Control Point Sequence tag (3008, 0041) in file " + path);
                            }
                        }
                    }
                    else
                    {
                        // Check VR value of Treatment Session Ion Beam Sequence (3008,0021) is [SQ] type
                        throw new NullReferenceException("VR value of Treatment Session Ion Beam Sequence tag (3008,0021) is not [SQ] type in file " + path);
                    }
                }
                else
                {
                    throw new NullReferenceException("Can not find Treatment Session Ion Beam Sequence tag (3008, 0021) in file " + path);
                }

                // Release file
                dcmFile.Dispose();
            }
            else
            {
                // Import file is not Dicom file
                string str = string.Format("File {0} is not a Dicom file or is empty", path);
                throw new NullReferenceException(str);
            }

            // Release reader
            reader.Dispose();

            return extractorObjs;
        }

        /// <summary>
        /// Export CSV SPOT file from DICOM file.
        /// </summary>
        /// <param name="lstExtractObject">The data Extract from Dicom file</param>
        /// <param name="file">The file name</param>
        /// <param name="pathCSVFoder">The folder directory to cxport CSV SPOT file </param>
        private void ExportCsvSpotFile(List<ExtractorObject> lstExtractObject, string file, string pathCSVFoder)
        {
            try
            {
                // Initialize strBuilder to contain data
                var strBuilder = new StringBuilder();

                if (file.Contains(".dcm"))
                    file = file.Replace(".dcm", "_SPOT.csv");
                else
                    file = file + "_SPOT.csv";

                string path = string.Format(@"{0}\{1}", pathCSVFoder, file);

                // Fill the data into strBuilder like template file
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3}", "G1", "Data Setting", "SP", "Px"));
                strBuilder.AppendLine(string.Format("{0}", "ID"));
                strBuilder.AppendLine(string.Format("{0}", 99));

                // Fill title into strBuilder like template file
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4}", "SP No", "Pos X", "Pos Y", "Weight", "MU"));

                // Create Order Number
                uint orderNumber = 0;

                // Sum of Spot Meter List in lstExtractObject
                double sum = lstExtractObject.Sum(obj => obj.SpotMeterList.Sum());

                // Fill the data into strBuilder
                for (int i = 0; i < lstExtractObject.Count; ++i)
                {
                    for (int j = 0; j < lstExtractObject[i].SpotMeterList.Count; ++j)
                    {
                        ++orderNumber;

                        if (0 == sum)
                        {
                            strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4}", orderNumber, lstExtractObject[i].SpotPosition[j].X, lstExtractObject[i].SpotPosition[j].Y, null, lstExtractObject[i].SpotMeterList[j]));
                        }
                        else
                        {
                            double weight = lstExtractObject[i].SpotMeterList[j] / sum;
                            strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4}", orderNumber, lstExtractObject[i].SpotPosition[j].X, lstExtractObject[i].SpotPosition[j].Y, weight, lstExtractObject[i].SpotMeterList[j]));
                        }
                    }
                }

                if (false == System.IO.File.Exists(path))
                {
                    // Export strBuider to Data.csv file
                    System.IO.File.WriteAllText(path, strBuilder.ToString());
                }
                else
                {
                    // Sleep to display a single dialog at a time. Waiting on all thread
                    while (this.isPause)
                    {
                        Thread.Sleep(100);
                    }     

                    // Turn on isPause to display a single dialog at a time
                    this.isPause = true;

                    // Exit export file if choose isCancel = true
                    if (this.isCancel)
                    {
                        this.isPause = false;
                        return;
                    }                 

                    var dlgResult = new System.Windows.MessageBoxResult();

                    System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                    {
                        dlgResult = MessageBox.Show(string.Format("File {0} already exists.\nDo you want to overwrite it?", path), "Confirm", MessageBoxButton.YesNo, MessageBoxImage.Question);
                    }), System.Windows.Threading.DispatcherPriority.Send, null);           
                   
                    if (dlgResult == System.Windows.MessageBoxResult.Yes)
                    {
                        // Overwrite
                        // Export strBuider to .csv file                        
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }
                    else
                    {
                        // Create new file for writing
                        // Export strBuider to .csv file
                        path = path.Replace(".csv", string.Format("_{0}{1}{2}_{3}{4}{5}.csv", DateTime.Now.Year, DateTime.Now.Month.ToString("00"), DateTime.Now.Day.ToString("00"), DateTime.Now.Hour.ToString("00"), DateTime.Now.Minute.ToString("00"), DateTime.Now.Second.ToString("00")));
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }

                    this.isPause = false;
                }
            }
            catch (System.UnauthorizedAccessException unauthorizedAccessException)
            {
                throw unauthorizedAccessException;
            }
            catch (System.Security.SecurityException securityException)
            {
                throw securityException;
            }
            catch (System.Exception ex)
            {
                throw ex;
            }
        }

        /// <summary>
        /// Export CSV CP file from DICOM file.
        /// </summary>
        /// <param name="lstExtractObject">The data Extract from Dicom file</param>
        /// <param name="file">The file name</param>
        /// <param name="pathCSVFoder">The folder directory to cxport CSV CP file </param>
        private void ExportCsvCpFile(List<ExtractorObject> lstExtractObject, string file, string pathCSVFoder)
        {
            try
            {
                // Initialize strBuilder to contain data
                var strBuilder = new StringBuilder();

                // Sum of Spot Meter List in lstExtractObject
                double sum = lstExtractObject.Sum(obj => obj.SpotMeterList.Sum());

                if (file.Contains(".dcm"))
                {
                    file = file.Replace(".dcm", "_CP.csv");
                }
                else
                {
                    file = file + "_CP.csv";
                }

                string path = string.Format(@"{0}\{1}", pathCSVFoder, file);

                // Fill the data into strBuilder like template file
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3}", "G1", "Data Setting", "SP", "Px"));
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3}", "ID", "Action", "Set MU", "Spot ID"));
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3}", 909, null, sum, 99));

                // Fill title into strBuilder like template file
                strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4},{5},{6}", "CP No", "Spot", "Energy", "SP", "Intensity", "Weight", "Remark"));

                // Fill the data into strBuilder
                for (int i = 0; i < lstExtractObject.Count; ++i)
                {
                    if (0 == sum)
                    {
                        strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4},{5},{6}", i + 1, lstExtractObject[i].SpotMeterList.Count, lstExtractObject[i].NomBeamEnergy, 3.8, 100, null, "hoge"));
                    }
                    else
                    {
                        double weight = lstExtractObject[i].SpotMeterList.Sum() / sum;
                        strBuilder.AppendLine(string.Format("{0},{1},{2},{3},{4},{5},{6}", i + 1, lstExtractObject[i].SpotMeterList.Count, lstExtractObject[i].NomBeamEnergy, 3.8, 100, weight, "hoge"));
                    }
                }

                if (false == System.IO.File.Exists(path))
                {
                    // Export strBuider to Data.csv file
                    System.IO.File.WriteAllText(path, strBuilder.ToString());
                }
                else
                {
                    // Sleep to display a single dialog at a time. Waiting on all thread
                    while (this.isPause)
                    {
                        Thread.Sleep(100);
                    }                                       

                    // Turn on isPause to display a single dialog at a time
                    this.isPause = true;

                    // Exit export file if choose isCancel = true
                    if (this.isCancel)
                    {
                        this.isPause = false;
                        return;
                    }

                    var dlgResult = new System.Windows.MessageBoxResult();
                    System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                    {
                        dlgResult = MessageBox.Show(string.Format("{0} already exists.\nDo you want to overwrite it?", path), "Confirm", MessageBoxButton.YesNo, MessageBoxImage.Question);
                    }), System.Windows.Threading.DispatcherPriority.Send, null);                            
                     

                    if (dlgResult == System.Windows.MessageBoxResult.Yes)
                    {
                        // Overwrite
                        // Export strBuider to Data.csv file
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }
                    else
                    {
                        // Create new file for writing
                        // Export strBuider to Data.csv file
                        path = path.Replace(".csv", string.Format("_{0}{1}{2}_{3}{4}{5}.csv", DateTime.Now.Year, DateTime.Now.Month.ToString("00"), DateTime.Now.Day.ToString("00"), DateTime.Now.Hour.ToString("00"), DateTime.Now.Minute.ToString("00"), DateTime.Now.Second.ToString("00")));
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }
                    this.isPause = false;
                }
            }
            catch (UnauthorizedAccessException unauthorizedAccessException)
            {
                throw unauthorizedAccessException;
            }
            catch (System.Security.SecurityException securityException)
            {
                throw securityException;
            }
            catch (System.Exception ex)
            {
                throw ex;
            }
        }

        /// <summary>
        /// Export MAC file from DICOM file.
        /// </summary>
        /// <param name="lstExtractObject">The data Extract from Dicom file</param>
        /// <param name="file">The file name</param>
        /// <param name="pathCSVFoder">The folder directory to cxport mac file </param>
        private void ExportMacFile(List<ExtractorObject> lstExtractObject, string file, string pathFoder)
        {
            try
            {
                // Initialize strBuilder to contain data
                var strBuilder = new StringBuilder();
                var str = new StringBuilder();

                if (file.Contains(".dcm"))
                {
                    file = file.Replace(".dcm", ".mac");
                }
                else
                {
                    file = file + ".mac";
                }

                string path = string.Format(@"{0}\{1}", pathFoder, file);

                file = file.Replace(".mac", ".root");

                // Fill the data into strBuilder like template file
                strBuilder.AppendLine("#Geant4 macro header #########################################");
                strBuilder.AppendLine("/control/verbose 1");
                strBuilder.AppendLine("/run/verbose 1");
                strBuilder.AppendLine("/tracking/storeTrajectory 0");
                strBuilder.AppendLine("/random/setSequence 1");
                strBuilder.AppendLine("#");
                strBuilder.AppendLine("# phantom parameter #####################################");
                strBuilder.AppendLine("/geometry/gantryAngle 0 deg");
                strBuilder.AppendLine("/geometry/patientOffset 0 0 230 mm");
                strBuilder.AppendLine("#/geometry/setPatient");
                strBuilder.AppendLine("/geometry/setPhantom");
                strBuilder.AppendLine("#");
                strBuilder.AppendLine("#File Name ###################################################");
                strBuilder.AppendLine(string.Format("/data/SetRootFileName ./data/HKDD_eval/{0}", file));
                strBuilder.AppendLine("#");
                strBuilder.AppendLine("#No of Spot  #################################################");

                uint count = 0;
                         
                // Fill the data into strBuilder
                for (int i = 0; i < lstExtractObject.Count; ++i)
                {
                    if (0 != lstExtractObject[i].SpotMeterList[lstExtractObject[i].SpotMeterList.Count - 1])
                    {
                        ++count;
                    }
                    for (int j = 0; j < lstExtractObject[i].SpotMeterList.Count; ++j)
                    {
                        str.AppendLine("#");
                        str.AppendLine(string.Format("/beam/meanKineticEnergy {0} MeV", lstExtractObject[i].NomBeamEnergy.ToString()));
                        str.AppendLine(string.Format("/scanning/crossline {0} mm", lstExtractObject[i].SpotPosition[j].X.ToString()));
                        str.AppendLine(string.Format("/scanning/inline {0} mm", lstExtractObject[i].SpotPosition[j].Y.ToString()));
                        str.AppendLine(string.Format("/run/beamOn {0}", lstExtractObject[i].SpotMeterList[j] * this.c));
                    }
                }

                strBuilder.AppendLine("/run/NoOfParticle " + count);
                strBuilder.AppendLine("/run/NoOfSpot " + lstExtractObject.Sum(obj => obj.SpotMeterList.Count));
                strBuilder.AppendLine("#");
                strBuilder.AppendLine("#Start #######################################################");

                strBuilder.Append(str);

                strBuilder.AppendLine("#");
                strBuilder.AppendLine("#End #########################################################");

                if (false == System.IO.File.Exists(path))
                {
                    // Export strBuider to Data.csv file
                    System.IO.File.WriteAllText(path, strBuilder.ToString());
                }
                else
                {
                    // Sleep to display a single dialog at a time. Waiting on all thread
                    while (this.isPause)
                    {
                        Thread.Sleep(100);
                    }

                    // Turn on isPause to display a single dialog at a time
                    this.isPause = true;

                    // Exit export file if choose isCancel = true
                    if (this.isCancel)
                    {
                        this.isPause = false;
                        return;
                    }

                    var dlgResult = new System.Windows.MessageBoxResult();
                    System.Windows.Application.Current.Dispatcher.Invoke(new Action(() =>
                    {
                        dlgResult = MessageBox.Show(string.Format("{0} already exists.\nDo you want to overwrite it?", path), "Confirm", MessageBoxButton.YesNo, MessageBoxImage.Question);
                    }), System.Windows.Threading.DispatcherPriority.Send, null);                            
                     

                    if (dlgResult == System.Windows.MessageBoxResult.Yes)
                    {
                        // Overwrite
                        // Export strBuider to Data.csv file
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }
                    else
                    {
                        // Create new file for writing
                        // Export strBuider to Data.csv file
                        path = path.Replace(".mac", string.Format("_{0}{1}{2}_{3}{4}{5}.mac", DateTime.Now.Year, DateTime.Now.Month.ToString("00"), DateTime.Now.Day.ToString("00"), DateTime.Now.Hour.ToString("00"), DateTime.Now.Minute.ToString("00"), DateTime.Now.Second.ToString("00")));
                        System.IO.File.WriteAllText(path, strBuilder.ToString());
                    }
                    this.isPause = false;
                }
            }
            catch (System.UnauthorizedAccessException unauthorizedAccessException)
            {
                throw unauthorizedAccessException;
            }
            catch (System.Security.SecurityException securityException)
            {
                throw securityException;
            }
            catch (System.Exception ex)
            {
                throw ex;
            }
        }

        #endregion

    }
}