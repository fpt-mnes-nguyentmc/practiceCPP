using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace DicomExtractor.Model
{
    public class ExtractorObject
    {
        /// <summary>
        /// The Scan Spot Metersets Delivered list
        /// </summary>
        public List<double> SpotMeterList { get; set; }

        /// <summary>
        /// The Nominal Beam Energy list
        /// </summary>
        public double NomBeamEnergy { get; set; }

        /// <summary>
        /// The Scan Spot Position Map list
        /// </summary>
        public List<Point> SpotPosition { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="ExtractorObject"/> class.
        /// </summary>
        public ExtractorObject()
        {
            this.NomBeamEnergy = default(double);
            this.SpotMeterList = new List<double>();
            this.SpotPosition = new List<Point>();
        }
    }
}
