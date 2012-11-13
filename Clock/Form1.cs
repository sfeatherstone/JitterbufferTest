/* Quote from 

Programming .NET Windows Applications

By Jesse Liberty, Dan Hurwitz
First Edition October 2003 
Pages: 1246 (More details)
*/


using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Timers;
using System.Windows.Forms;


namespace Clock3CS
{
  // Rename the class
  public class ClockFace : System.Windows.Forms.Form
  {
    // Required designer variable.
    private System.ComponentModel.Container components = null;
  
    private int FaceRadius = 450;    // size of the clock face
    private bool b24Hours = false;    // 24 hour clock face?
    private DateTime currentTime;    // used in more than one method

    public DateTime lastUpdate;

    // new
    private int xCenter;        // center of the clock
    private int yCenter;
    private static int DateRadius = 600; // outer circumference for date
    private static int Offset = 0;    // for moving the text 
    Font font = new Font("Arial", 40);
    private Label labelTime;  // use the same font throughout
    private StringDraw sdToday;      // the text to animate

    public ClockFace()
    {
      // Required for Windows Form Designer support
      InitializeComponent();

      // use the user's choice of colors
      BackColor = SystemColors.Window;
      ForeColor = SystemColors.WindowText;

    
      // *** begin new
      string today = System.DateTime.Now.ToLongDateString();
      today = " " + today.Replace(",","");
      
      // create a new stringdraw object with today's date
      sdToday = new StringDraw(today,this);
      currentTime = DateTime.Now;


      // set the current center based on the
      // client area
      xCenter = Width / 2;
      yCenter = Height / 2;


      // *** end new


      // update the clock by timer
/*      System.Timers.Timer timer = new System.Timers.Timer();
      timer.Elapsed += new System.Timers.ElapsedEventHandler(OnTimer);
      timer.Interval = 33;  // shorter interval - more movement
      timer.Enabled = true;
        */

    }

    protected override void OnPaint ( PaintEventArgs e )
    {
      base.OnPaint(e);
      Graphics g = e.Graphics;
      SetScale(g);
      DrawFace(g);
      DrawTime(g,true);  // force an update
    }

    // every time the timer event fires, update the clock
    public void OnTimer(Object source, ElapsedEventArgs e)
    {
        Animate(); 
    }

    public void Animate()
    {

        Graphics g = this.CreateGraphics();

        SetScale(g);
        DrawFace(g);
        DrawTime(g, false);
        DrawDate(g);
        if (lastUpdate != null)
        {
            int diff = (DateTime.Now - lastUpdate).Milliseconds;
            labelTime.Text = diff.ToString();
            lastUpdate = DateTime.Now;
        }
        else
        {
            lastUpdate = DateTime.Now;
        }
        g.Dispose();
    }


    #region Windows Form Designer generated code
    protected override void Dispose( bool disposing )
    {
      if( disposing )
      {
        if (components != null) 
        {
          components.Dispose();
        }
      }
      base.Dispose( disposing );
    }

    
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
            this.labelTime = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelTime
            // 
            this.labelTime.AutoSize = true;
            this.labelTime.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelTime.Location = new System.Drawing.Point(3, 3);
            this.labelTime.Name = "labelTime";
            this.labelTime.Size = new System.Drawing.Size(0, 20);
            this.labelTime.TabIndex = 0;
            // 
            // ClockFace
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.labelTime);
            this.Name = "ClockFace";
            this.Text = "Clock3CS";
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ClockFace_MouseDown);
            this.ResumeLayout(false);
            this.PerformLayout();

    }
    #endregion

/*    [STAThread]
    static void Main() 
    {
      Application.Run(new ClockFace());
    }*/

    private void SetScale(Graphics g)
    {
      // if the form is too small, do nothing
      if ( Width == 0 || Height == 0 )
        return;

      // set the origin at the center
      g.TranslateTransform(xCenter, yCenter);  // use the members vars

      // set inches to the minimum of the width 
      // or height dividedby the dots per inch  
      float inches = Math.Min(Width / g.DpiX, Height / g.DpiX);

      // set the scale to a grid of 2000 by 2000 units
      g.ScaleTransform(
        inches * g.DpiX / 2000, inches * g.DpiY / 2000);
    }

    private void DrawFace(Graphics g)
    {
      // numbers are in forecolor except flash number in green
      // as the seconds go by.
      Brush brush = new SolidBrush(ForeColor);
      //float x, y;

      // new code
/*      int numHours = b24Hours ? 24 : 12;
      int deg = 360 / numHours;
      
      // for each of the hours on the clock face
      for (int i = 1; i <= numHours; i++)
      {
        // i = hour  30 degrees = offset per hour  
        // +90 to make 12 straight up
        x = GetCos(i*deg + 90) * FaceRadius;
        y = GetSin(i*deg + 90) * FaceRadius;

        StringFormat format = new StringFormat();
        format.Alignment = StringAlignment.Center;
        format.LineAlignment = StringAlignment.Center;

        g.DrawString(
          i.ToString(), font, brush, -x, -y,format);
      
      }  // end for loop*/
    }    // end drawFace

    private void DrawTimeDiff(Graphics g, int delay)
    {
        var font = new Font("Microsoft Sans Serif", 8.25F);
            var text = delay.ToString();
            g.DrawString(text, font, Brushes.Black, 0, 0);
    }
    private void DrawTime(Graphics g, bool forceDraw)
    {

      //  length of the hands
      float hourLength = FaceRadius * 0.5f;
      float minuteLength = FaceRadius * 0.7f;
      float secondLength = FaceRadius * 0.9f;

      // set to back color to erase old hands first
      Pen hourPen = new Pen(BackColor);
      Pen minutePen = new Pen(BackColor);
      Pen secondPen = new Pen(BackColor);

      // set the arrow heads
      hourPen.EndCap = LineCap.ArrowAnchor;
      minutePen.EndCap = LineCap.ArrowAnchor;

      // hour hand is thicker
      hourPen.Width = 30;
      minutePen.Width = 20;

      // second hand 
      Brush secondBrush = new SolidBrush(BackColor);
      const int EllipseSize = 50;

      GraphicsState state;  // to to protect and to serve


      // 1 - delete the old time

      // delete the old second hand
      // figure out how far around to rotate to draw the second hand
      // save the current state, rotate, draw and then restore the state
      float rotation = GetSecondRotation();
      state = g.Save();
      g.RotateTransform(rotation);
      g.FillEllipse(
        secondBrush,
        -(EllipseSize/2),
        -secondLength,
        EllipseSize,
        EllipseSize);
      g.Restore(state);

      DateTime newTime = currentTime.AddSeconds(1.0).AddMinutes(1.0).AddHours(1.0);
      bool newMin = false;  // has the minute changed?

      // if the minute has changed, set the flag
      if ( newTime.Minute != currentTime.Minute )
        newMin = true;


      // if the minute has changed or you must draw anyway then you 
      // must first delete the old minute and hour hand
      if ( newMin  || forceDraw )
      {

        // figure out how far around to rotate to draw the minute hand
        // save the current state, rotate, draw and then restore the state
        rotation = GetMinuteRotation();
        state = g.Save();
        g.RotateTransform(rotation);
        g.DrawLine(minutePen,0,0,0,-minuteLength);
        g.Restore(state);

        // figure out how far around to rotate to draw the hour hand
        // save the current state, rotate, draw and then restore the state
        rotation = GetHourRotation();
        state = g.Save();
        g.RotateTransform(rotation);
        g.DrawLine(hourPen,0,0,0,-hourLength);
        g.Restore(state);
      }

      // step 2 - draw the new time
      currentTime = newTime;

      hourPen.Color = Color.Red;
      minutePen.Color = Color.Blue;
      secondPen.Color = Color.Green;
      secondBrush = new SolidBrush(Color.Green);

      // draw the new second hand
      // figure out how far around to rotate to draw the second hand
      // save the current state, rotate, draw and then restore the state
      state = g.Save();
      rotation = GetSecondRotation();
      g.RotateTransform(rotation);
      g.FillEllipse(
        secondBrush,
        -(EllipseSize/2),
        -secondLength,
        EllipseSize,
        EllipseSize);
      g.Restore(state);

      // if the minute has changed or you must draw anyway then you 
      // must draw the new minute and hour hand
      if ( newMin || forceDraw )
      {

        // figure out how far around to rotate to draw the minute hand
        // save the current state, rotate, draw and then restore the state
        state = g.Save();
        rotation = GetMinuteRotation();
        g.RotateTransform(rotation);
        g.DrawLine(minutePen,0,0,0,-minuteLength);
        g.Restore(state);

        // figure out how far around to rotate to draw the hour hand
        // save the current state, rotate, draw and then restore the state
        state = g.Save();
        rotation = GetHourRotation();
        g.RotateTransform(rotation);
        g.DrawLine(hourPen,0,0,0,-hourLength);
        g.Restore(state);
      }
    }
    
    // determine the rotation to draw the hour hand
    private float GetHourRotation()
    {
      // degrees depend on 24 vs. 12 hour clock
      float deg = b24Hours ? 15 : 30;
      float numHours = b24Hours ? 24 : 12;
      return( 360f * currentTime.Hour / numHours +
        deg * currentTime.Minute / 60f);
    }

    private float GetMinuteRotation()
    {
      return( 360f * currentTime.Minute / 60f ); 
    }

    private float GetSecondRotation()
    {
      return(360f * currentTime.Second / 60f);
    }
 
    private static float GetSin(float degAngle)
    {
      return (float) Math.Sin(Math.PI * degAngle / 180f);
    }

    private static float GetCos(float degAngle)
    {
      return (float) Math.Cos(Math.PI * degAngle / 180f);
    }

  /*  private void btnClockFormat_Click(object sender, System.EventArgs e)
    {
      btnClockFormat.Text = b24Hours ? "24 Hour" : "12 Hour";
      b24Hours = ! b24Hours;
      this.Invalidate();
    }*/

    private void DrawDate(Graphics g)
    {
      Brush brush = new SolidBrush(ForeColor);
      sdToday.DrawString(g,brush);
    }

    private void ClockFace_MouseDown(
      object sender, System.Windows.Forms.MouseEventArgs e)
    {
      xCenter = e.X;
      yCenter = e.Y;
      this.Invalidate();
    
    }


    // each letter in the outer string knows how to draw itself
    private class LtrDraw
    {
      char myChar;    // the actual letter i draw
      float x;      // current x coordinate
      float y;      // current y coordinate
      float oldx;      // old x coordinate (to delete)
      float oldy;      // old y coordinate (to delete)
      

      // constructor
      public LtrDraw(char c)
      {
        myChar = c;
      }

      // property for X coordinate
      public float X
      {
        get { return x; }
        set { oldx = x; x = value; }
      }

      // property for Y coordinate
      public float Y
      {
        get { return y; }
        set { oldy = y; y = value; }
      }

      // get total width of the string
      public float GetWidth(Graphics g, Font font)
      {
        SizeF stringSize = g.MeasureString(myChar.ToString(),font);
        return stringSize.Width;
      }

      // get total height of the string
      public float GetHeight(Graphics g, Font font)
      {
        SizeF stringSize = g.MeasureString(myChar.ToString(),font);
        return stringSize.Height;
      }


      // get the font from the control and draw the current character
      // First delete the old and then draw the new
      public void DrawString(Graphics g, Brush brush, ClockFace cf)
      {
        Font font = cf.font;
        Brush blankBrush = new SolidBrush(cf.BackColor);
        g.DrawString(myChar.ToString(),font,blankBrush,oldx,oldy);
        g.DrawString(myChar.ToString(),font,brush,x,y);
      }

    }

    // holds an array of LtrDraw objects
    // and knows how to tell them to draw
    private class StringDraw
    {
      ArrayList theString = new ArrayList();
      LtrDraw l;
      ClockFace theControl;

      // constructor takes a string, populates the array
      // and stashes away the calling control (ClockFace)
      public StringDraw(string s, ClockFace theControl)
      {
        this.theControl = theControl;
        foreach (char c in s)
        {
          l = new LtrDraw(c);
          theString.Add(l);
        }
      }

      // divide the circle by the number of letters
      // and draw each letter in position
      public void DrawString(Graphics g, Brush brush)
      {
        int angle = 360 / theString.Count;
        int counter = 0;

        foreach (LtrDraw theLtr in theString)
        {
          // 1. To find the X coordinate, take the Cosine of the angle
          // and multiply by the radius.
          // 2. To compute the angle, start with the base angle 
          // (360 divided by the number of letters)
          // and multiply by letter position.
          // Thus if each letter is 10 degrees, and this is the third
          // letter, you get 30 degrees. Add 90 to start at 12 O'clock.
          // Each time through, subtract the clockFace offset to move 
          // the entire string around the clock on each timer call
          float newX = GetCos(angle  * counter + 90 - ClockFace.Offset) * ClockFace.DateRadius ;
          float newY = GetSin(angle * counter + 90 - ClockFace.Offset) * ClockFace.DateRadius ;
          theLtr.X = 
            newX - (theLtr.GetWidth(g,theControl.font) / 2);
          theLtr.Y = 
            newY - (theLtr.GetHeight(g,theControl.font) / 2);
          counter++;
          theLtr.DrawString(g,brush,theControl);
        }
        ClockFace.Offset += 1;  // rotate the entire string
      }
    }

  }  // end class
}    // end namespace

/*
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Clock
{
  public partial class Form1 : Form
  {
    public Form1()
    {
      InitializeComponent();
    }
  }
}
*/