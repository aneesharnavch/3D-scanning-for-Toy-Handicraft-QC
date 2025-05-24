using System;

public class CameraCalibration
{
    private double fx, fy, cx, cy, alphaC;
    public CameraCalibration(double fx, double fy, double cx, double cy, double alphaC = 0)
    {
        this.fx = fx;
        this.fy = fy;
        this.cx = cx;
        this.cy = cy;
        this.alphaC = alphaC;
    }

    public (double xPixel, double yPixel) GetPixelCoordinates(double xd, double yd)
    {
        double xp = fx * xd + alphaC * yd + cx;
        double yp = fy * yd + cy;
        return (xp, yp);
    }
}
public class Program
{
    public static void Main(string[] args)
    {
        CameraCalibration camera = new CameraCalibration(1000, 1000, 640, 480);
        var pixelCoords = camera.GetPixelCoordinates(0.1, 0.2);
        Console.WriteLine($"Pixel Coordinates: ({pixelCoords.xPixel}, {pixelCoords.yPixel})");
    }
}