using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Threading;
using System.Windows.Forms;

namespace Gamma3D.Content.CustomControls
{
    /// <summary>
    /// 
    /// </summary>
    public class ImageHost : HwndHost
    {
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

        /// <summary>
        /// 
        /// </summary>
        public delegate void AfterSizeChangedHandler();

        /// <summary>
        /// Occurs when [after size changed].
        /// </summary>
        public event AfterSizeChangedHandler AfterSizeChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="ImageHost"/> class.
        /// </summary>
        public ImageHost()
        {
        }

        /// <summary>
        /// When overridden in a derived class, accesses the window process (handle) of the hosted child window.
        /// </summary>
        /// <param name="hwnd">The window handle of the hosted window.</param>
        /// <param name="msg">The message to act upon.</param>
        /// <param name="wParam">Information that may be relevant to handling the message. This is typically used to store small pieces of information, such as flags.</param>
        /// <param name="lParam">Information that may be relevant to handling the message. This is typically used to reference an object.</param>
        /// <param name="handled">Whether events resulting should be marked handled.</param>
        /// <returns>
        /// The window handle of the child window.
        /// </returns>
        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            handled = false;

            if (hwnd.Equals(this.Handle) == false) return IntPtr.Zero;

            switch ((uint)msg)
            {
                case Common.Constants.WM_PAINT:
                    {
                        Common.NativeMethods.PAINTSTRUCT ps;
                        Common.NativeMethods.BeginPaint(hwnd, out ps);

						SetBackgroundColor();    

                        Common.NativeMethods.EndPaint(hwnd, ref ps);
                        break;
                    }
            }

            return IntPtr.Zero;
        }

        /// <summary>
        /// When overridden in a derived class, destroys the hosted window.
        /// </summary>
        /// <param name="hwnd">A structure that contains the window handle.</param>
        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            Common.NativeMethods.DestroyWindow(hwnd.Handle);
        }

        /// <summary>
        /// Raises the <see cref="E:System.Windows.FrameworkElement.SizeChanged" /> event, using the specified information as part of the eventual event data.
        /// </summary>
        /// <param name="sizeInfo">Details of the old and new size involved in the change.</param>
        protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
        {
            base.OnRenderSizeChanged(sizeInfo);

            if (AfterSizeChanged != null)
            {
                Dispatcher.BeginInvoke(
                    DispatcherPriority.Normal,
                    new Action(() => AfterSizeChanged()));
            }
        }

        /// <summary>
        /// When overridden in a derived class, creates the window to be hosted.
        /// </summary>
        /// <param name="hwndParent">The window handle of the parent window.</param>
        /// <returns>
        /// The handle to the child Win32 window to create.
        /// </returns>
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            var hwnd = Common.NativeMethods.CreateWindowEx(0, "static", "",
                                                           (int)(Common.Constants.WS_CHILD),
                                                           0, 0,
                                                           1, 1,
                                                           hwndParent.Handle,
                                                           (IntPtr)Common.Constants.HOST_ID,
                                                           IntPtr.Zero,
                                                           0);

            this.Logic.SetImageHostHandle(hwnd);

            Common.NativeMethods.SetParent(hwnd, hwndParent.Handle);
            return new HandleRef(this, hwnd);
        }

        /// <summary>
        /// Sets the color of the background.
        /// </summary>
        private void SetBackgroundColor()
        {
            if (this.Handle != null && this.Handle != IntPtr.Zero)
            {
                Common.NativeMethods.RECT rect;
                var hDC = Common.NativeMethods.GetDC(this.Handle);

                Common.NativeMethods.GetClientRect(this.Handle, out rect);
                Common.NativeMethods.SetBkColor(hDC, Common.Constants.CL_BLUE);
                Common.NativeMethods.ExtTextOut(hDC, 0, 0, 0x2, ref rect, null, 0, IntPtr.Zero);
                Common.NativeMethods.ReleaseDC(this.Handle, hDC);
            }
        }
    }
}
