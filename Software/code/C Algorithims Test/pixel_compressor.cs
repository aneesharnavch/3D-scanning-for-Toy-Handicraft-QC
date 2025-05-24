using System.Drawing;

public class ImageResizer
{
    public static Bitmap DownscaleTo720p(Bitmap originalImage)
    {
        int targetWidth = 1280;
        int targetHeight = 720;

        int originalWidth = originalImage.Width;
        int originalHeight = originalImage.Height;

        float widthFactor = (float)targetWidth / originalWidth;
        float heightFactor = (float)targetHeight / originalHeight;

        float scaleFactor = Math.Min(widthFactor, heightFactor);

        int newWidth = (int)(originalWidth * scaleFactor);
        int newHeight = (int)(originalHeight * scaleFactor);
        Bitmap resizedImage = new Bitmap(originalImage, new Size(newWidth, newHeight));

        return resizedImage;
    }
}
