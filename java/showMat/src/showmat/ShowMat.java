/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package showmat;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import static java.lang.System.out;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;
import org.opencv.imgcodecs.Imgcodecs;

/**
 *
 * @author Jasper
 */
public class ShowMat {

    
    public static void main(String[] args) {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        
        String filename = "..\\..\\test_data\\lena.png";
        Mat src = Imgcodecs.imread(filename);
        
        showMat("src", src);
    }
    
    
    private static void showMat(String frameName, Mat img) {
        
        MatOfByte matOfByte = new MatOfByte();
        Imgcodecs.imencode(".jpg", img, matOfByte);

        try ( InputStream in = new ByteArrayInputStream(matOfByte.toArray()) ) {
            BufferedImage bufImage = ImageIO.read(in);
            JFrame frame = new JFrame();
            frame.setTitle(frameName);
            frame.getContentPane().add(new JLabel(new ImageIcon(bufImage)));
            frame.pack();
            frame.setVisible(true);
        }catch (IOException ex) {
            out.println(ex.toString());
        }
    }
    
}
