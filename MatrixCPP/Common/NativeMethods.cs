using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Common
{
    /// <summary>
    /// 
    /// </summary>
    public static class Constants
    {
        /// <summary>
        /// The w m_ erasebkgnd
        /// </summary>
        public const int WM_ERASEBKGND = 0x14;

        /// <summary>
        /// The hos t_ identifier
        /// </summary>
        public const int HOST_ID = 0x00000002;

        /// <summary>
        /// The w s_ child
        /// </summary>
        public const int WS_CHILD = 0x40000000;

        /// <summary>
        /// The w m_ paint
        /// </summary>
        public const int WM_PAINT = 0x0f;

        /// <summary>
        /// The cl blue
        /// </summary>
        public const int CL_BLUE = 0xFF0000;
    }

    /// <summary>
    /// 
    /// </summary>
    public static class NativeMethods
    {
        #region WinAPI Structure

        /// <summary>
        /// 
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct PAINTSTRUCT
        {
            /// <summary>
            /// The HDC
            /// </summary>
            public IntPtr hdc;
            /// <summary>
            /// The f erase
            /// </summary>
            public bool fErase;
            /// <summary>
            /// The rc paint
            /// </summary>
            public RECT rcPaint;
            /// <summary>
            /// The f restore
            /// </summary>
            public bool fRestore;
            /// <summary>
            /// The f inc update
            /// </summary>
            public bool fIncUpdate;
            /// <summary>
            /// The RGB reserved
            /// </summary>
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
            public byte[] rgbReserved;
        }

        /// <summary>
        /// 
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            /// <summary>
            /// The left
            /// </summary>
            public int left;
            /// <summary>
            /// The top
            /// </summary>
            public int top;
            /// <summary>
            /// The right
            /// </summary>
            public int right;
            /// <summary>
            /// The bottom
            /// </summary>
            public int bottom;
        }
        #endregion

        #region WinAPI Native Methods

        /// <summary>
        /// Creates the window ex.
        /// </summary>
        /// <param name="dwExStyle">The dw ex style.</param>
        /// <param name="lpszClassName">Name of the LPSZ class.</param>
        /// <param name="lpszWindowName">Name of the LPSZ window.</param>
        /// <param name="style">The style.</param>
        /// <param name="x">The x.</param>
        /// <param name="y">The y.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <param name="hwndParent">The HWND parent.</param>
        /// <param name="hMenu">The h menu.</param>
        /// <param name="hInst">The h inst.</param>
        /// <param name="pvParam">The pv parameter.</param>
        /// <returns></returns>
        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateWindowEx(
            int dwExStyle,
            string lpszClassName,
            string lpszWindowName,
            int style,
            int x,
            int y,
            int width,
            int height,
            IntPtr hwndParent,
            IntPtr hMenu,
            IntPtr hInst,
            [MarshalAs(UnmanagedType.AsAny)] object pvParam);

        /// <summary>
        /// Destroys the window.
        /// </summary>
        /// <param name="hWnd">The h WND.</param>
        /// <returns></returns>
        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool DestroyWindow(IntPtr hWnd);

        /// <summary>
        /// Sets the parent.
        /// </summary>
        /// <param name="hWndChild">The h WND child.</param>
        /// <param name="hWndNewParent">The h WND new parent.</param>
        /// <returns></returns>
        [DllImport("user32.dll", SetLastError = true)]
        public static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        /// <summary>
        /// Begins the paint.
        /// </summary>
        /// <param name="hwnd">The HWND.</param>
        /// <param name="lpPaint">The lp paint.</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern IntPtr BeginPaint(IntPtr hwnd, out PAINTSTRUCT lpPaint);

        /// <summary>
        /// Ends the paint.
        /// </summary>
        /// <param name="hWnd">The h WND.</param>
        /// <param name="lpPaint">The lp paint.</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern bool EndPaint(IntPtr hWnd, [In] ref PAINTSTRUCT lpPaint);

        /// <summary>
        /// Gets the client rect.
        /// </summary>
        /// <param name="hWnd">The h WND.</param>
        /// <param name="lpRect">The lp rect.</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern bool GetClientRect(IntPtr hWnd, out RECT lpRect);

        /// <summary>
        /// Sets the color of the bk.
        /// </summary>
        /// <param name="hdc">The HDC.</param>
        /// <param name="crColor">Color of the cr.</param>
        /// <returns></returns>
        [DllImport("gdi32.dll")]
        public static extern uint SetBkColor(IntPtr hdc, int crColor);

        /// <summary>
        /// Exts the text out.
        /// </summary>
        /// <param name="hdc">The HDC.</param>
        /// <param name="X">The x.</param>
        /// <param name="Y">The y.</param>
        /// <param name="fuOptions">The fu options.</param>
        /// <param name="lprc">The LPRC.</param>
        /// <param name="lpString">The lp string.</param>
        /// <param name="cbCount">The cb count.</param>
        /// <param name="lpDx">The lp dx.</param>
        /// <returns></returns>
        [DllImport("gdi32.dll", EntryPoint = "ExtTextOutW")]
        public static extern bool ExtTextOut(IntPtr hdc, int X, int Y, uint fuOptions,
           [In] ref RECT lprc, [MarshalAs(UnmanagedType.LPWStr)] string lpString,
           uint cbCount, [In] IntPtr lpDx);

        /// <summary>
        /// Releases the dc.
        /// </summary>
        /// <param name="hWnd">The h WND.</param>
        /// <param name="hDC">The h dc.</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        public static extern IntPtr ReleaseDC(IntPtr hWnd, IntPtr hDC);

        /// <summary>
        /// Gets the dc.
        /// </summary>
        /// <param name="hWnd">The h WND.</param>
        /// <returns></returns>
        [DllImport("user32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern IntPtr GetDC(IntPtr hWnd);
        #endregion
    }

    /// <summary>
    /// 
    /// </summary>
    internal static class UnsafeNativeMethods
    {

    }
}
