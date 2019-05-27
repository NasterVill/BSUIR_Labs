package controller;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.TimeUnit;

import com.github.sarxos.webcam.Webcam;
import com.github.sarxos.webcam.WebcamResolution;
import com.xuggle.mediatool.IMediaWriter;
import com.xuggle.mediatool.ToolFactory;
import com.xuggle.xuggler.ICodec;

import javax.imageio.ImageIO;

public class WebcamController {
    private static final String outputVideoFileName = "outputVideo.mp4";
    private static final String outputPhotoName = "outputPhoto.png";

    private Webcam webcam;
    private Dimension size;
    private IMediaWriter writer;

    public WebcamController() {
        size = WebcamResolution.QVGA.getSize();
        webcam = Webcam.getDefault();
        webcam.setViewSize(size);
        webcam.open(true);
    }

    public String getName() {
        return webcam.getName();
    }


    public BufferedImage takePhoto() throws IOException {
        if (!webcam.isOpen()) return null;

        // Get image
        BufferedImage image = webcam.getImage();

        // Save image to PNG file
        ImageIO.write(image, "PNG", new File(outputPhotoName));

        return image;
    }


    public void recordVideo(final double duration, final int fps) {
        // Make an IMediaWriter to write the file.
        writer = ToolFactory.makeWriter(outputVideoFileName);

        // Tell the writer that we're going to add one video stream, with id is 0,
        // at position 0, and that it will have a fixed frame rate of FRAME_RATE.
        writer.addVideoStream(0, 0, ICodec.ID.CODEC_ID_MPEG4,
                size.width , size.height);

        long startTime = System.nanoTime();

        for (int index = 0; index < duration * fps; index++) {

            // Take the screen shot
            BufferedImage screen = webcam.getImage();

            // Convert to the right image type
            BufferedImage bgrScreen = convertToType(screen,
                    BufferedImage.TYPE_3BYTE_BGR);

            // Encode the image to stream #0
            writer.encodeVideo(0, bgrScreen, System.nanoTime() - startTime,
                    TimeUnit.NANOSECONDS);

            // Sleep for frame rate milliseconds
            try {
                Thread.sleep((long) (1000 / fps));
            } catch (InterruptedException e) {
                // ignore
            }

        }

        // Tell the writer to close and write the trailer if  needed
        writer.close();
    }

    private static BufferedImage convertToType(BufferedImage sourceImage, int targetType) {

        BufferedImage image;

        // If the source image is already the target type, return the source image
        if (sourceImage.getType() == targetType) {
            image = sourceImage;
        }
        // Otherwise create a new image of the target type and draw the new image
        else {
            image = new BufferedImage(sourceImage.getWidth(),
                    sourceImage.getHeight(), targetType);
            image.getGraphics().drawImage(sourceImage, 0, 0, null);
        }

        return image;
    }
}
