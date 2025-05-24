using System;

public class AngleCalculation
{
    public (double thetaX, double thetaY) GetAngles(double xp, double yp, double fx, double fy, double cx, double cy)
    {
        double thetaX = Math.Atan((xp - cx) / fx);
        double thetaY = Math.Atan((yp - cy) / fy);
        return (thetaX, thetaY);
    }
}
public class Program
{
    public static void Main(string[] args)
    {
        double xp = 500, yp = 300;
        double fx = 1000, fy = 1000, cx = 640, cy = 480;

        AngleCalculation angleCalc = new AngleCalculation();
        var angles = angleCalc.GetAngles(xp, yp, fx, fy, cx, cy);
        Console.WriteLine($"Angles: ThetaX = {angles.thetaX} radians, ThetaY = {angles.thetaY} radians");
    }
}
